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
// Vice Commands

#define VICE_BUFFER_LEN 8192
uint8_t vice_buffer[VICE_BUFFER_LEN] = {0};
volatile atomic_uint_fast16_t vice_commands_buff_curr_len = 0;
volatile atomic_bool vice_allow_execute_flag = false;

void run_none(void) {}

VICE_COMMANDS_buffer_ctx vice_commands_buffer_ctx = {
    .allow_execute_flag = &vice_allow_execute_flag,
    .buffer = vice_buffer,
    .curr_len = &vice_commands_buff_curr_len,
    .max_len = VICE_BUFFER_LEN,
};

volatile atomic_uint_fast32_t vice_commands_deadline = 0;
volatile atomic_bool vice_commands_is_working = false;
VICE_COMMANDS_state_ctx vice_commands_state_ctx = {
    .deadline = &vice_commands_deadline,
    .is_working = &vice_commands_is_working,
};

static void delay(void)
{
    struct timespec ts;
    ts.tv_sec = 1;
    ts.tv_nsec = 0;
    // nanosleep suspends the calling thread
    nanosleep(&ts, NULL);
}

static uint32_t get_monotonic_now(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

typedef void (*_log_ptr)(const char *format, ...);

VICE_COMMANDS_ctx vice_commands_ctx = {
	.ad5940_adc_curr_len = &ad5940_adc_curr_len,
	.ad5940_adc_target_len = &ad5940_adc_target_len,
	.ad5940_controller_cal_para = &ad5940_controller_cal_para,
	.ad5940_controller_trigger_para = &ad5940_controller_trigger_para,
    .ad5940_stop = run_none,
    .ad5940_trigger_pre_task = run_none,
	.delay = delay,
    .delay_time_to_ms = 0,
    .free = free,
    .get_monotonic_now = get_monotonic_now,
    .HsRtiaCal = &HsRtiaCal,
    .log = (_log_ptr) printf,
    .LpDacPara = &LpDacPara,
    .LpRtiaCal = &LpRtiaCal,
    .vice_commands_buffer_ctx = &vice_commands_buffer_ctx,
    .vice_commands_state_ctx = &vice_commands_state_ctx,
    .malloc = malloc,
};

// --------------------------------------------------
// Main Commands

#define MAIN_BUFFER_LEN 8192
uint8_t main_buffer[MAIN_BUFFER_LEN] = {0};

MAIN_COMMANDS_buffer_ctx main_commands_buffer_ctx = {
    .buffer = main_buffer,
    .len = MAIN_BUFFER_LEN,
};

volatile atomic_uint_fast32_t main_commands_deadline = 0;
volatile atomic_bool main_commands_is_working = false;
MAIN_COMMANDS_state_ctx main_commands_state_ctx = {
    .deadline = &main_commands_deadline,
    .is_working = &main_commands_is_working,
};

MAIN_COMMANDS_ctx main_commands_ctx = {
	.ad5940_controller_trigger_para = &ad5940_controller_trigger_para,
    .ad5940_reset_option = AD5940_CONTROLLER_RESET_OPTION_HAREWARE,
    .ad5940_SEQGenBuff = SEQGenBuff,
    .ad5940_SEQGenBuffLen = SEQGenBuffLen,
	.ad5940_stop = run_none,
	.circuit_reboot = run_none,
    .get_monotonic_now = get_monotonic_now,
    .log = (_log_ptr) printf,
    .main_commands_buffer_ctx = &main_commands_buffer_ctx,
    .main_commands_state_ctx = &main_commands_state_ctx,
    .vice_commands_buffer_ctx = &vice_commands_buffer_ctx,
};