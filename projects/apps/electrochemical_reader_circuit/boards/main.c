#include "ad5940_controller.h"
#include "commands.h"
#include "hardware.h"
#include "peripheral_nus.h"

#include <stdatomic.h>
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/services/nus.h>

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
uint32_t SEQGenBuff[SEQGenBuffLen];

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
#define ad5940_adc_fifo_task_runner_prio 4

static k_tid_t bluetooth_send_ad5940_adc_fifo_runner_tid;
static struct k_thread bluetooth_send_ad5940_adc_fifo_runner_thread;
K_THREAD_STACK_DEFINE(bluetooth_send_ad5940_adc_fifo_runner_stack, 16384);
#define bluetooth_send_ad5940_adc_fifo_runner_prio 3

static k_tid_t vice_commands_runner_tid;
static struct k_thread vice_commands_runner_thread;
K_THREAD_STACK_DEFINE(vice_commands_runner_stack, 16384);
#define vice_commands_runner_prio 5

// --------------------------------------------------

#define COMMANDS_DELAY K_USEC(10)

void AD5940_STOP(void) {
	atomic_store(&main_commands_flag[MAIN_COMMANDS_START_VICE_BUFF], false);
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

void bluetooth_to_main_commands_runner(void)
{
	while (true)
	{
		uint16_t len;
		peripheral_nus_received_data_get_len(&len);
		if (len == 0)
		{
			k_sleep(COMMANDS_DELAY);
			continue;
		}

		uint8_t buffer[len];
		peripheral_nus_received_data_get_data(buffer);
		main_commands_handler(buffer, len);
		k_sleep(COMMANDS_DELAY);
	}
	return;
}

// --------------------------------------------------
// AD5940 ADC FIFO task

#define AD5940_ADC_FIFO_BUFF_LEN 100
uint32_t ad5940_adc_fifo_buff[AD5940_ADC_FIFO_BUFF_LEN];
volatile atomic_uint_fast32_t ad5940_adc_target_len = 0;
volatile atomic_uint_fast32_t ad5940_adc_curr_len = 0;

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
			AD5940_STOP();
		}
	}
	return;
}

// --------------------------------------------------
// Bluetooth send AD5940 ADC FIFO

volatile atomic_uint_fast16_t bluetooth_to_ad5940_adc_curr_len = 0;

void bluetooth_send_ad5940_adc_fifo_runner(void)
{
	while (true)
	{
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

void vice_commands_runner(void)
{
	while (true)
	{
		if (atomic_load(&main_commands_flag[MAIN_COMMANDS_START_VICE_BUFF]) == false)
		{
			k_sleep(COMMANDS_DELAY);
			continue;
		}

		if (atomic_load(&vice_commands_buff_final_len) == 0)
		{
			k_sleep(COMMANDS_DELAY);
			continue;
		}
		vice_commands_handler();
	}
	return;
}

// ==================================================
// Main
// ==================================================

int main(void)
{
	// ========================================
	// Init hardware

	int err;

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

    return 0;
}