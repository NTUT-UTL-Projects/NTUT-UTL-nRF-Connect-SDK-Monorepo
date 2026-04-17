#include "config.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SEQGenBuffLen 1000
uint32_t SEQGenBuff[SEQGenBuffLen];

AD5940_CONTROLLER_TRIGGER_PARA ad5940_controller_trigger_para = {
};

AD5940_CONTROLLER_CAL_PARA ad5940_controller_cal_para = {
};

fImpPol_Type HsRtiaCal;
LPDACPara_Type LpDacPara;
fImpPol_Type LpRtiaCal;

// --------------------------------------------------
// AD5940 ADC FIFO task

volatile atomic_uint_fast32_t ad5940_adc_target_len = 0;
volatile atomic_uint_fast32_t ad5940_adc_curr_len = 0;

// --------------------------------------------------
// Main Commands

#define VICE_BUFFER_LEN 8192
uint8_t vice_buffer[VICE_BUFFER_LEN] = {0};
volatile atomic_uint_fast16_t vice_commands_buff_curr_len = 0;
volatile atomic_bool vice_allow_execute_flag = false;

#define MAIN_BUFFER_LEN 8192
uint8_t main_buffer[MAIN_BUFFER_LEN] = {0};

void run_none(void) {}

vice_commands_buffer_ctx vice_commands_buff_ctx = {
    .allow_execute_flag = &vice_allow_execute_flag,
    .buffer = vice_buffer,
    .curr_len = &vice_commands_buff_curr_len,
    .max_len = VICE_BUFFER_LEN,
};

static void delay(void)
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1E2;
    // nanosleep suspends the calling thread
    nanosleep(&ts, NULL);
}

typedef void (*_log_ptr)(const char *format, ...);

vice_commands_ctx vice_ctx = {
	.ad5940_adc_curr_len = &ad5940_adc_curr_len,
	.ad5940_adc_target_len = &ad5940_adc_target_len,
	.ad5940_controller_cal_para = &ad5940_controller_cal_para,
	.ad5940_controller_trigger_para = &ad5940_controller_trigger_para,
    .ad5940_stop = run_none,
    .ad5940_trigger_pre_task = run_none,
	.delay_unit = delay,
    .delay_unit_pre_task = run_none,
    .free = free,
    .HsRtiaCal = &HsRtiaCal,
    .log = (_log_ptr) printf,
    .LpDacPara = &LpDacPara,
    .LpRtiaCal = &LpRtiaCal,
    .vice_commands_buffer_ctx = &vice_commands_buff_ctx,
    .malloc = malloc,
};

main_commands_buffer_ctx main_commands_buff_ctx = {
    .buffer = main_buffer,
    .len = MAIN_BUFFER_LEN,
};

main_commands_ctx main_ctx = {
	.ad5940_controller_trigger_para = &ad5940_controller_trigger_para,
    .ad5940_reset_option = AD5940_CONTROLLER_RESET_OPTION_HAREWARE,
    .ad5940_SEQGenBuff = SEQGenBuff,
    .ad5940_SEQGenBuffLen = SEQGenBuffLen,
	.ad5940_stop = run_none,
	.circuit_reboot = run_none,
    .log = (_log_ptr) printf,
    .main_commands_buffer_ctx = &main_commands_buff_ctx,
    .vice_commands_buffer_ctx = &vice_commands_buff_ctx,
};