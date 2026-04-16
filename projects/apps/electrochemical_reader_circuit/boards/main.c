#include "ad5940_controller.h"
#include "commands.h"
#include "hardware.h"
#include "peripheral_nus.h"
#include "watchdog.h"

#include <stdatomic.h>
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/services/nus.h>

// ==================================================
// Hardware
// ==================================================

extern hardware_t hardware;

// ==================================================
// Commands
// ==================================================

static main_commands_ctx main_ctx;
static vice_commands_ctx vice_ctx;

#define VICE_COMMANDS_BUFF_LEN 2048
static uint8_t vice_commands_buff[VICE_COMMANDS_BUFF_LEN];
static volatile atomic_uint_fast16_t vice_commands_buff_curr_len = 0;

static volatile atomic_bool vice_allow_execute_flag = false;

static vice_commands_buffer_ctx vice_commands_buff_ctx = {
	.allow_execute_flag = &vice_allow_execute_flag,
	.buffer = vice_commands_buff,
	.curr_len = &vice_commands_buff_curr_len,
	.max_len = VICE_COMMANDS_BUFF_LEN,
};

#define VICE_COMMANDS_DELAY_UNIT K_MSEC(1)

void commands_delay_unit(void)
{
	k_sleep(VICE_COMMANDS_DELAY_UNIT);
	return;
}

// TODO
void todo(void){}

// ==================================================
// Bluetooth
// ==================================================

static uint8_t __bluetooth_data_buffer[2048];

static struct bt_nus_cb nus_listener = {
	.received = peripheral_nus_received_data_listener,
};

// ==================================================
// AD5940
// ==================================================

#define SEQGenBuffLen 1000
static uint32_t SEQGenBuff[SEQGenBuffLen];

#define AD5940_DataType DATATYPE_SINC2

#define AD5940_FifoSrc FIFOSRC_SINC2NOTCH

AD5940_CONTROLLER_TRIGGER_PARA ad5940_controller_trigger_para = {
	.DataType = AD5940_DataType,
	.EnterSleepEn = bFALSE,
	.FifoSrc = AD5940_FifoSrc,
};

fImpPol_Type HsRtiaCal;
LPDACPara_Type LpDacPara;
fImpPol_Type LpRtiaCal;

// ==================================================
// Threads
// ==================================================

static k_tid_t bluetooth_to_main_commands_runner_tid;
static struct k_thread bluetooth_to_main_commands_runner_thread;
K_THREAD_STACK_DEFINE(bluetooth_to_main_commands_runner_stack, 16384);
#define bluetooth_to_main_commands_runner_prio 4

static k_tid_t ad5940_adc_fifo_task_runner_tid;
static struct k_thread ad5940_adc_fifo_task_runner_thread;
K_THREAD_STACK_DEFINE(ad5940_adc_fifo_task_runner_stack, 16384);
#define ad5940_adc_fifo_task_runner_prio 3

static k_tid_t bluetooth_send_ad5940_adc_fifo_runner_tid;
static struct k_thread bluetooth_send_ad5940_adc_fifo_runner_thread;
K_THREAD_STACK_DEFINE(bluetooth_send_ad5940_adc_fifo_runner_stack, 16384);
#define bluetooth_send_ad5940_adc_fifo_runner_prio 4

static k_tid_t vice_commands_runner_tid;
static struct k_thread vice_commands_runner_thread;
K_THREAD_STACK_DEFINE(vice_commands_runner_stack, 16384);
#define vice_commands_runner_prio 5

static k_tid_t watch_dog_runner_tid;
static struct k_thread watch_dog_runner_thread;
K_THREAD_STACK_DEFINE(watch_dog_runner_stack, 2048);
#define watch_dog_runner_prio 0

// --------------------------------------------------

#define COMMANDS_DELAY K_USEC(10)

void ad5940_stop(void) {
	atomic_store(&vice_allow_execute_flag, false);
	// Refer to page 107 of the datasheet
	// Enable AFE to enter sleep mode.
	AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK); /* Unlock so sequencer can put AD5940 to sleep */
	/* Stop Wupt right now */
	AD5940_WUPTCtrl(bFALSE);
	/* For detailed reference on the chip's shutdown procedure, see page 129 of the datasheet.*/
	AD5940_ShutDownS();
	return;
}

// --------------------------------------------------
// Bluetooth to Main Commands

static volatile atomic_bool bluetooth_to_main_commands_runner_alived = true;

void bluetooth_to_main_commands_runner(void)
{
	while (true)
	{
		atomic_store(&bluetooth_to_main_commands_runner_alived, true);

		uint16_t len;
		peripheral_nus_received_data_get_len(&len);
		if (len == 0)
		{
			k_sleep(COMMANDS_DELAY);
			continue;
		}

		uint8_t buffer[len];
		peripheral_nus_received_data_get_data(buffer, len);

		main_ctx.main_commands_buffer_ctx = &(main_commands_buffer_ctx) {
			.buffer = buffer,
			.len = len,
		};
		main_commands_handler(&main_ctx);
		
		k_sleep(COMMANDS_DELAY);
	}
	return;
}

// --------------------------------------------------
// AD5940 ADC FIFO task

#define AD5940_ADC_FIFO_BUFF_LEN 100
static uint32_t ad5940_adc_fifo_buff[AD5940_ADC_FIFO_BUFF_LEN];
static volatile atomic_uint_fast32_t ad5940_adc_target_len = 0;
static volatile atomic_uint_fast32_t ad5940_adc_curr_len = 0;

void ad5940_adc_fifo_task_runner(void)
{
	while (true)
	{
		hardware.ad5940_intc_wait();

		uint16_t buffCount;
		AD5940_DATAFIFOISR(
			(AD5940_DATAFIFOISR_PARA) {
				.BufferSize = AD5940_ADC_FIFO_BUFF_LEN,
				.Enable = bTRUE,
				.FIFOThresh = AD5940_DATAFIFOTHRESH_UNCHANGE,
			},
			(AD5940_DATAFIFOISR_RET) {
				.pBuff = ad5940_adc_fifo_buff,
				.pBuffCount = &buffCount,
			}
		);

		atomic_fetch_add(&ad5940_adc_curr_len, buffCount);
		if(atomic_load(&ad5940_adc_curr_len) >= atomic_load(&ad5940_adc_target_len)) {
			ad5940_stop();
		}
	}
	return;
}

// --------------------------------------------------
// Bluetooth send AD5940 ADC FIFO

static volatile atomic_uint_fast16_t bluetooth_to_ad5940_adc_curr_len = 0;

void reset_bluetooth_to_ad5940_adc_curr_len(void)
{
	atomic_store(&bluetooth_to_ad5940_adc_curr_len, 0);
}

static volatile atomic_bool bluetooth_send_ad5940_adc_fifo_runner_alived = true;

void bluetooth_send_ad5940_adc_fifo_runner(void)
{
	while (true)
	{
		atomic_store(&bluetooth_send_ad5940_adc_fifo_runner_alived, true);

		if (atomic_load(&ad5940_adc_curr_len) == 0)
		{
			k_sleep(COMMANDS_DELAY);
			continue;
		}

		uint16_t ad5940_adc_curr_len = atomic_load(&ad5940_adc_curr_len);
		uint16_t bluetooth_to_ad5940_adc_curr_len = atomic_load(&bluetooth_to_ad5940_adc_curr_len);

		size_t curr = bluetooth_to_ad5940_adc_curr_len;
		size_t final = ad5940_adc_curr_len;

		atomic_store(&bluetooth_to_ad5940_adc_curr_len, final);
		
		int err;
		uint32_t ADCcode;
		for (size_t i = curr; i < final; i+=sizeof(ADCcode))
		{
			memcpy(
				&ADCcode,
				ad5940_adc_fifo_buff + i,
				sizeof(ADCcode)
			);
			float volts = AD5940_ADCCode_to_volts(
				ADCcode,
				ad5940_controller_trigger_para.dsp_cfg.ADCBaseCfg.ADCPga,
				hardware.ad5940_controller_cal_para.VRef1p82
			);
			err = bt_nus_send(NULL, &volts, sizeof(volts));
			printk("Data send - Result: %d\n", err);
		}
	}
	return;
}

// --------------------------------------------------
// Vice Commands

static volatile atomic_bool vice_commands_runner_alived = true;

void vice_commands_runner(void)
{
	while (true)
	{
		atomic_store(&vice_commands_runner_alived, true);

		if (atomic_load(&vice_allow_execute_flag) == false)
		{
			k_sleep(COMMANDS_DELAY);
			continue;
		}

		if (atomic_load(vice_commands_buff_ctx.curr_len) == 0)
		{
			k_sleep(COMMANDS_DELAY);
			continue;
		}
		vice_commands_handler(&vice_ctx);
	}
	return;
}

// --------------------------------------------------
// Watch Dog

#define WDT_MAX_WINDOW 500U
#define WDT_MIN_WINDOW 0000U

#define WDG_FEED_INTERVAL (WDT_MIN_WINDOW + ((WDT_MAX_WINDOW - WDT_MIN_WINDOW) / 4))

void watch_dog_runner(void)
{
	while (true)
	{
		if(!atomic_load(&bluetooth_to_main_commands_runner_alived)) break;
		atomic_store(&bluetooth_to_main_commands_runner_alived, false);

		if(!atomic_load(&bluetooth_send_ad5940_adc_fifo_runner_alived)) break;
		atomic_store(&bluetooth_send_ad5940_adc_fifo_runner_alived, false);

		if(!atomic_load(&vice_commands_runner_alived)) break;
		atomic_store(&vice_commands_runner_alived, false);

		/* Feeding watchdog. */
		printk("Feeding watchdog...\n");
		watchdog0_feed();
		k_sleep(K_MSEC(WDG_FEED_INTERVAL));
	}
	return;
}

// ==================================================
// Main
// ==================================================

// TODO
// Test AD5940 Task
uint8_t buffer[] = {0x05, 0x01, 0x00, 0x08, 0x00, 0x00, 0x05, 0x02, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x04, 0x02, 0x00, 0x00, 0x00, 0xcd, 0xcc, 0x4c, 0x3e, 0xcd, 0xcc, 0x4c, 0x3d, 0x0a, 0xd7, 0x23, 0x3c, 0x0a, 0xd7, 0x23, 0x3c, 0x0a, 0xd7, 0xa3, 0x3c, 0x0a, 0x00, 0x00, 0x00, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd4, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x07, 0x60, 0xea, 0x00, 0x00, 0x05, 0x08, 0x02, };
uint16_t len = sizeof(buffer);

int main(void)
{
	// ========================================
	// Init commands
	vice_ctx = (vice_commands_ctx) {
		.ad5940_adc_curr_len = &ad5940_adc_curr_len,
		.ad5940_adc_target_len = &ad5940_adc_target_len,
		.ad5940_controller_cal_para = &hardware.ad5940_controller_cal_para,
		.ad5940_controller_trigger_para = &ad5940_controller_trigger_para,
		.ad5940_trigger_pre_event = reset_bluetooth_to_ad5940_adc_curr_len,
		.delay_unit = commands_delay_unit,
		.free = k_free,
		.HsRtiaCal = &HsRtiaCal,
		.log = printk,
		.LpDacPara = &LpDacPara,
		.LpRtiaCal = &LpRtiaCal,
		.malloc = k_malloc,
		.vice_commands_buffer_ctx = &vice_commands_buff_ctx,
	};

	main_ctx = (main_commands_ctx) {
		.ad5940_controller_trigger_para = &ad5940_controller_trigger_para,
		.ad5940_reset_option = hardware.ad5940_controller_reset,
		.ad5940_SEQGenBuff = SEQGenBuff,
		.ad5940_SEQGenBuffLen = SEQGenBuffLen,
		.ad5940_stop = ad5940_stop,
		.circuit_reboot = todo,
		.log = printk,
		// .main_commands_buffer // Use Bluetooth data as a buffer instead of the initial settings..
		.vice_commands_buffer_ctx = vice_ctx.vice_commands_buffer_ctx,
	};

	// ========================================
	// Init hardware

	int err;

	/* Initialize watchdog. */
    err = watchdog0_init(WDT_MIN_WINDOW, WDT_MAX_WINDOW);
    if (err) {
        printk("Watchdog init failed\n");
        return err;
    }

	err = hardware.ad5940_intc_init();
	if (err) return err;
	err = hardware.ad5940_gpio_init();
	if (err) return err;
	err = hardware.ad5940_spi_init();
	if (err) return err;

	err = AD5940_controller_reset(
		SEQGenBuff,
		SEQGenBuffLen,
		hardware.ad5940_controller_reset
	);
	if (err) return err;

	ad5940_controller_trigger_para.gpio_cfg = hardware.ad5940_gpio_cfg;

	// ========================================
	// Bluetooth Peripheral NUS
	peripheral_nus_received_data_ring_buffer_init(__bluetooth_data_buffer, sizeof(__bluetooth_data_buffer));

    err = peripheral_nus_init(&nus_listener);
    
	if (err) {
		printk("Failed to peripheral_nus_init callback: %d\n", err);
		return err;
	}

	// ========================================
	// Watch Dog Feed

	watchdog0_feed();

	// ========================================
	// Threads

	bluetooth_to_main_commands_runner_tid = k_thread_create(
		&bluetooth_to_main_commands_runner_thread,
		bluetooth_to_main_commands_runner_stack,
		K_THREAD_STACK_SIZEOF(bluetooth_to_main_commands_runner_stack),
		bluetooth_to_main_commands_runner,
		NULL, NULL, NULL,
		bluetooth_to_main_commands_runner_prio, 0,
		K_NO_WAIT
	);

	ad5940_adc_fifo_task_runner_tid = k_thread_create(
		&ad5940_adc_fifo_task_runner_thread,
		ad5940_adc_fifo_task_runner_stack,
		K_THREAD_STACK_SIZEOF(ad5940_adc_fifo_task_runner_stack),
		ad5940_adc_fifo_task_runner,
		NULL, NULL, NULL,
		ad5940_adc_fifo_task_runner_prio, 0,
		K_NO_WAIT
	);

	bluetooth_send_ad5940_adc_fifo_runner_tid = k_thread_create(
		&bluetooth_send_ad5940_adc_fifo_runner_thread,
		bluetooth_send_ad5940_adc_fifo_runner_stack,
		K_THREAD_STACK_SIZEOF(bluetooth_send_ad5940_adc_fifo_runner_stack),
		bluetooth_send_ad5940_adc_fifo_runner,
		NULL, NULL, NULL,
		bluetooth_send_ad5940_adc_fifo_runner_prio, 0,
		K_NO_WAIT
	);

	vice_commands_runner_tid = k_thread_create(
		&vice_commands_runner_thread,
		vice_commands_runner_stack,
		K_THREAD_STACK_SIZEOF(vice_commands_runner_stack),
		vice_commands_runner,
		NULL, NULL, NULL,
		vice_commands_runner_prio, 0,
		K_NO_WAIT
	);

	watch_dog_runner_tid = k_thread_create(
		&watch_dog_runner_thread,
		watch_dog_runner_stack,
		K_THREAD_STACK_SIZEOF(watch_dog_runner_stack),
		watch_dog_runner,
		NULL, NULL, NULL,
		watch_dog_runner_prio, 0,
		K_NO_WAIT
	);

	// ========================================

	// TODO
	// Test AD5940 Task
	{
		main_ctx.main_commands_buffer_ctx = &(main_commands_buffer_ctx) {
			.buffer = buffer,
			.len = len,
		};
		main_commands_handler(&main_ctx);
	}

    return 0;
}