#include "ad5940_controller.h"
#include "commands.h"
#include "hardware.h"
#include "peripheral_nus.h"
#include "watchdog.h"

#include <stdatomic.h>
#include <zephyr/bluetooth/services/nus.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>

// ==================================================
// Basic
// ==================================================

uint32_t get_monotonic_now(void) { return k_uptime_get_32(); }

void reboot(void) { sys_reboot(SYS_REBOOT_COLD); }

// ==================================================
// Hardware
// ==================================================

extern hardware_t hardware;

// ==================================================
// Commands
// ==================================================

#define COMMANDS_DELAY_TIMEOUT K_MSEC(100)

void commands_delay(void)
{
    k_sleep(COMMANDS_DELAY_TIMEOUT);
    return;
}

// --------------------------------------------------
// Vice Commands

static VICE_COMMANDS_ctx vice_commands_ctx;

#define VICE_COMMANDS_BUFF_LEN 2048
static uint8_t                       vice_commands_buff[VICE_COMMANDS_BUFF_LEN];
static volatile atomic_uint_fast16_t vice_commands_buff_curr_len = 0;

static volatile atomic_bool vice_allow_execute_flag = false;

static VICE_COMMANDS_buffer_ctx vice_commands_buffer_ctx = {
    .allow_execute_flag = &vice_allow_execute_flag,
    .buffer             = vice_commands_buff,
    .curr_len           = &vice_commands_buff_curr_len,
    .max_len            = VICE_COMMANDS_BUFF_LEN,
};

volatile atomic_uint_fast32_t vice_commands_deadline   = 1000;
volatile atomic_bool          vice_commands_is_working = false;
VICE_COMMANDS_state_ctx       vice_commands_state_ctx  = {
           .deadline   = &vice_commands_deadline,
           .is_working = &vice_commands_is_working,
};

// --------------------------------------------------
// Main Commands

static MAIN_COMMANDS_ctx main_commands_ctx;

volatile atomic_uint_fast32_t main_commands_deadline   = 1000;
volatile atomic_bool          main_commands_is_working = false;
MAIN_COMMANDS_state_ctx       main_commands_state_ctx  = {
           .deadline   = &main_commands_deadline,
           .is_working = &main_commands_is_working,
};

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
    .DataType     = AD5940_DataType,
    .EnterSleepEn = bFALSE,
    .FifoSrc      = AD5940_FifoSrc,
};

fImpPol_Type   HsRtiaCal;
LPDACPara_Type LpDacPara;
fImpPol_Type   LpRtiaCal;

// ==================================================
// Threads
// ==================================================

static k_tid_t         bluetooth_to_main_commands_runner_tid;
static struct k_thread bluetooth_to_main_commands_runner_thread;
K_THREAD_STACK_DEFINE(bluetooth_to_main_commands_runner_stack, 16384);
#define bluetooth_to_main_commands_runner_prio 4

static k_tid_t         ad5940_adc_fifo_task_runner_tid;
static struct k_thread ad5940_adc_fifo_task_runner_thread;
K_THREAD_STACK_DEFINE(ad5940_adc_fifo_task_runner_stack, 16384);
#define ad5940_adc_fifo_task_runner_prio 4

static k_tid_t         bluetooth_send_ad5940_adc_fifo_runner_tid;
static struct k_thread bluetooth_send_ad5940_adc_fifo_runner_thread;
K_THREAD_STACK_DEFINE(bluetooth_send_ad5940_adc_fifo_runner_stack, 16384);
#define bluetooth_send_ad5940_adc_fifo_runner_prio 4

static k_tid_t         vice_commands_runner_tid;
static struct k_thread vice_commands_runner_thread;
K_THREAD_STACK_DEFINE(vice_commands_runner_stack, 16384);
#define vice_commands_runner_prio 4

static k_tid_t         watch_dog_runner_tid;
static struct k_thread watch_dog_runner_thread;
K_THREAD_STACK_DEFINE(watch_dog_runner_stack, 16384);
#define watch_dog_runner_prio 0

// --------------------------------------------------

#define COMMANDS_DELAY K_USEC(10)

void ad5940_stop(void)
{
    // Refer to page 107 of the datasheet
    // Enable AFE to enter sleep mode.
    AD5940_SleepKeyCtrlS(
        SLPKEY_UNLOCK
    ); /* Unlock so sequencer can put AD5940 to sleep */
    /* Stop Wupt right now */
    AD5940_WUPTCtrl(bFALSE);
    /* For detailed reference on the chip's shutdown procedure, see page 129 of
     * the datasheet.*/
    AD5940_ShutDownS();
    return;
}

// --------------------------------------------------
// Bluetooth to Main Commands

void bluetooth_to_main_commands_runner(void)
{
    while (true)
    {
        peripheral_nus_received_data_wait_for_data();

        uint16_t len;
        peripheral_nus_received_data_get_len(&len);
        if (len == 0)
        {
            k_sleep(COMMANDS_DELAY);
            continue;
        }
        uint8_t *data = k_malloc(len);

        peripheral_nus_received_data_get_data(data, len);

        main_commands_ctx.main_commands_buffer_ctx =
            &(MAIN_COMMANDS_buffer_ctx){
                .buffer = data,
                .len    = len,
            };
        MAIN_COMMANDS_handler(&main_commands_ctx);

        k_free(data);
    }
    return;
}

// --------------------------------------------------
// AD5940 ADC FIFO task

#define AD5940_ADC_FIFO_BUFF_LEN 100
static uint32_t ad5940_adc_fifo_buff[AD5940_ADC_FIFO_BUFF_LEN];
static volatile atomic_uint_fast16_t ad5940_adc_target_len = 0;
static volatile atomic_uint_fast16_t ad5940_adc_curr_len   = 0;

void ad5940_adc_fifo_task_runner(void)
{
    while (true)
    {
        hardware.ad5940_intc_wait();

        uint16_t buffCount;
        AD5940_DATAFIFOISR(
            (AD5940_DATAFIFOISR_PARA){
                .BufferSize = AD5940_ADC_FIFO_BUFF_LEN,
                .Enable     = bTRUE,
                .FIFOThresh = AD5940_DATAFIFOTHRESH_UNCHANGE,
            },
            (AD5940_DATAFIFOISR_RET){
                .pBuff      = ad5940_adc_fifo_buff,
                .pBuffCount = &buffCount,
            }
        );

        atomic_fetch_add(&ad5940_adc_curr_len, buffCount);
        if (atomic_load(&ad5940_adc_curr_len) >=
            atomic_load(&ad5940_adc_target_len))
        {
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

static volatile atomic_uint_fast32_t
    bluetooth_send_ad5940_adc_fifo_runner_deadline = 1000;

void bluetooth_send_ad5940_adc_fifo_runner(void)
{
    while (true)
    {
        atomic_store(
            &bluetooth_send_ad5940_adc_fifo_runner_deadline,
            k_uptime_get_32() + 1000
        );

        uint16_t _ad5940_adc_curr_len = atomic_load(&ad5940_adc_curr_len);
        if (_ad5940_adc_curr_len == 0)
        {
            k_sleep(COMMANDS_DELAY);
            continue;
        }

        uint16_t curr  = atomic_load(&bluetooth_to_ad5940_adc_curr_len);
        uint16_t final = _ad5940_adc_curr_len;

        atomic_store(&bluetooth_to_ad5940_adc_curr_len, final);

        int      err;
        uint8_t  _buff[sizeof(float) + sizeof(LpRtiaCal) + sizeof(HsRtiaCal)];
        uint32_t ADCcode;
        for (size_t i = curr; i < final; i++)
        {
            atomic_store(
                &bluetooth_send_ad5940_adc_fifo_runner_deadline,
                k_uptime_get_32() + 1000
            );

            // volt
            memcpy(&ADCcode, ad5940_adc_fifo_buff + i, sizeof(ADCcode));
            float volts = AD5940_ADCCode_to_volts(
                ADCcode,
                ad5940_controller_trigger_para.dsp_cfg.ADCBaseCfg.ADCPga,
                hardware.ad5940_controller_cal_para.VRef1p82
            );

            memcpy(_buff, &volts, sizeof(volts));
            memcpy(_buff + sizeof(volts), &LpRtiaCal, sizeof(LpRtiaCal));
            memcpy(
                _buff + sizeof(volts) + sizeof(LpRtiaCal),
                &HsRtiaCal,
                sizeof(HsRtiaCal)
            );
            err = bt_nus_send(NULL, _buff, sizeof(_buff));
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
        if (atomic_load(&vice_allow_execute_flag) == false)
        {
            k_sleep(COMMANDS_DELAY);
            continue;
        }

        if (atomic_load(vice_commands_buffer_ctx.curr_len) == 0)
        {
            k_sleep(COMMANDS_DELAY);
            continue;
        }

        VICE_COMMANDS_handler(&vice_commands_ctx);
    }
    return;
}

// --------------------------------------------------
// Watch Dog

/**
 * @note
 * The WDT_MAX_WINDOW must set higher than AD5940 calibration operations
 * execution time.
 * - AD5940_LFOSCMeasure
 */
/**
 * TODO
 * Set high WDT_MAX_WINDOW to prevent vice_commands_runner delay crush,
 * But it cause WDT_MAX_WINDOW too high.
 */
#define WDT_MAX_WINDOW 3000U
#define WDT_MIN_WINDOW 0000U

#define WDG_FEED_INTERVAL                                                      \
    (WDT_MIN_WINDOW + ((WDT_MAX_WINDOW - WDT_MIN_WINDOW) / 4))
#define WDG_TIMEOUT_TICKS (WDG_FEED_INTERVAL * 2)
#define WDG_MAX_TICKS     0x80000000U

void watch_dog_runner(void)
{
    while (true)
    {
        uint32_t now = k_uptime_get_32();

        if (atomic_load(main_commands_ctx.main_commands_state_ctx->is_working))
        {
            uint32_t deadline = atomic_load(
                main_commands_ctx.main_commands_state_ctx->deadline
            );
            uint32_t difference = (uint32_t)(now - deadline);
            if (difference > WDG_TIMEOUT_TICKS && difference < WDG_MAX_TICKS)
                break;
        }

        if (atomic_load(vice_commands_ctx.vice_commands_state_ctx->is_working))
        {
            uint32_t deadline = atomic_load(
                vice_commands_ctx.vice_commands_state_ctx->deadline
            );
            uint32_t difference = (uint32_t)(now - deadline);
            if (difference > WDG_TIMEOUT_TICKS && difference < WDG_MAX_TICKS)
                break;
        }

        {
            uint32_t deadline =
                atomic_load(&bluetooth_send_ad5940_adc_fifo_runner_deadline);
            uint32_t difference = (uint32_t)(now - deadline);
            if (difference > WDG_TIMEOUT_TICKS && difference < WDG_MAX_TICKS)
                break;
        }

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
uint8_t buffer[] = {
    0x05, 0x01, 0x00, 0x08, 0x00, 0x00, 0x05, 0x02, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x03, 0x01,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x04, 0x02, 0x00, 0x00,
    0x00, 0xcd, 0xcc, 0x4c, 0x3e, 0xcd, 0xcc, 0x4c, 0x3d, 0x0a, 0xd7, 0x23,
    0x3c, 0x0a, 0xd7, 0x23, 0x3c, 0x0a, 0xd7, 0xa3, 0x3c, 0x0a, 0x00, 0x00,
    0x00, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
    0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xd4, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x05, 0x09, 0x05, 0x0b, 0x05, 0x07, 0x58, 0x02, 0x00, 0x00, 0x05,
    0x08, 0x02,
};
uint16_t len = sizeof(buffer);

int main(void)
{
    // ========================================
    // Init commands
    vice_commands_ctx = (VICE_COMMANDS_ctx){
        .ad5940_adc_curr_len            = &ad5940_adc_curr_len,
        .ad5940_adc_target_len          = &ad5940_adc_target_len,
        .ad5940_controller_cal_para     = &hardware.ad5940_controller_cal_para,
        .ad5940_controller_trigger_para = &ad5940_controller_trigger_para,
        .ad5940_stop                    = ad5940_stop,
        .ad5940_trigger_pre_task = reset_bluetooth_to_ad5940_adc_curr_len,
        .delay                   = commands_delay,
        .delay_time_to_ms        = COMMANDS_DELAY_TIMEOUT.ticks /
                            (CONFIG_SYS_CLOCK_TICKS_PER_SEC / 1E3),
        .free                     = k_free,
        .get_monotonic_now        = get_monotonic_now,
        .HsRtiaCal                = &HsRtiaCal,
        .log                      = printk,
        .LpDacPara                = &LpDacPara,
        .LpRtiaCal                = &LpRtiaCal,
        .malloc                   = k_malloc,
        .vice_commands_buffer_ctx = &vice_commands_buffer_ctx,
        .vice_commands_state_ctx  = &vice_commands_state_ctx,
    };

    main_commands_ctx = (MAIN_COMMANDS_ctx){
        .ad5940_controller_trigger_para = &ad5940_controller_trigger_para,
        .ad5940_reset_option            = hardware.ad5940_controller_reset,
        .ad5940_SEQGenBuff              = SEQGenBuff,
        .ad5940_SEQGenBuffLen           = SEQGenBuffLen,
        .ad5940_stop                    = ad5940_stop,
        .circuit_reboot                 = reboot,
        .get_monotonic_now              = get_monotonic_now,
        .log                            = printk,
        // .main_commands_buffer // Use Bluetooth data as a buffer instead of
        // the initial settings..
        .main_commands_state_ctx  = &main_commands_state_ctx,
        .vice_commands_buffer_ctx = vice_commands_ctx.vice_commands_buffer_ctx,
    };

    // ========================================
    // Init hardware

    int err;

    /* Initialize watchdog. */
    err = watchdog0_init(WDT_MIN_WINDOW, WDT_MAX_WINDOW);
    if (err)
    {
        printk("Watchdog init failed\n");
        return err;
    }

    err = hardware.ad5940_intc_init();
    if (err)
        return err;
    err = hardware.ad5940_gpio_init();
    if (err)
        return err;
    err = hardware.ad5940_spi_init();
    if (err)
        return err;

    err = AD5940_controller_reset(
        SEQGenBuff,
        SEQGenBuffLen,
        hardware.ad5940_controller_reset
    );
    if (err)
        return err;

    ad5940_controller_trigger_para.gpio_cfg = hardware.ad5940_gpio_cfg;

    // ========================================
    // Bluetooth Peripheral NUS
    peripheral_nus_received_data_ring_buffer_init(
        __bluetooth_data_buffer,
        sizeof(__bluetooth_data_buffer)
    );

    err = peripheral_nus_init(&nus_listener);

    if (err)
    {
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
        NULL,
        NULL,
        NULL,
        bluetooth_to_main_commands_runner_prio,
        0,
        K_NO_WAIT
    );

    ad5940_adc_fifo_task_runner_tid = k_thread_create(
        &ad5940_adc_fifo_task_runner_thread,
        ad5940_adc_fifo_task_runner_stack,
        K_THREAD_STACK_SIZEOF(ad5940_adc_fifo_task_runner_stack),
        ad5940_adc_fifo_task_runner,
        NULL,
        NULL,
        NULL,
        ad5940_adc_fifo_task_runner_prio,
        0,
        K_NO_WAIT
    );

    bluetooth_send_ad5940_adc_fifo_runner_tid = k_thread_create(
        &bluetooth_send_ad5940_adc_fifo_runner_thread,
        bluetooth_send_ad5940_adc_fifo_runner_stack,
        K_THREAD_STACK_SIZEOF(bluetooth_send_ad5940_adc_fifo_runner_stack),
        bluetooth_send_ad5940_adc_fifo_runner,
        NULL,
        NULL,
        NULL,
        bluetooth_send_ad5940_adc_fifo_runner_prio,
        0,
        K_NO_WAIT
    );

    vice_commands_runner_tid = k_thread_create(
        &vice_commands_runner_thread,
        vice_commands_runner_stack,
        K_THREAD_STACK_SIZEOF(vice_commands_runner_stack),
        vice_commands_runner,
        NULL,
        NULL,
        NULL,
        vice_commands_runner_prio,
        0,
        K_NO_WAIT
    );

    watch_dog_runner_tid = k_thread_create(
        &watch_dog_runner_thread,
        watch_dog_runner_stack,
        K_THREAD_STACK_SIZEOF(watch_dog_runner_stack),
        watch_dog_runner,
        NULL,
        NULL,
        NULL,
        watch_dog_runner_prio,
        0,
        K_NO_WAIT
    );

    // ========================================

    // TODO
    // Test AD5940 Task
    {
        main_commands_ctx.main_commands_buffer_ctx =
            &(MAIN_COMMANDS_buffer_ctx){
                .buffer = buffer,
                .len    = len,
            };
        MAIN_COMMANDS_handler(&main_commands_ctx);
    }

    return 0;
}