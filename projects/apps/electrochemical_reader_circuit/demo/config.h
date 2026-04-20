#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940_controller.h"
#include "commands.h"
#include "hardware.h"

#include <stdatomic.h>

#define SEQGenBuffLen 1000
extern uint32_t SEQGenBuff[SEQGenBuffLen];

extern AD5940_CONTROLLER_TRIGGER_PARA ad5940_controller_trigger_para;

extern AD5940_CONTROLLER_CAL_PARA ad5940_controller_cal_para;

extern fImpPol_Type HsRtiaCal;
extern LPDACPara_Type LpDacPara;
extern fImpPol_Type LpRtiaCal;

// --------------------------------------------------
// AD5940 ADC FIFO task

extern volatile atomic_uint_fast32_t ad5940_adc_target_len;
extern volatile atomic_uint_fast32_t ad5940_adc_curr_len;

// --------------------------------------------------
// Main Commands

#define VICE_BUFFER_LEN 8192
extern uint8_t vice_buffer[VICE_BUFFER_LEN];
extern volatile atomic_uint_fast16_t vice_commands_buff_curr_len;
extern volatile atomic_bool vice_allow_execute_flag;

#define MAIN_BUFFER_LEN 8192
extern uint8_t main_buffer[MAIN_BUFFER_LEN];

extern void run_none(void);

extern VICE_COMMANDS_buffer_ctx vice_commands_buffer_ctx;

extern VICE_COMMANDS_ctx vice_commands_ctx;

extern MAIN_COMMANDS_buffer_ctx main_commands_buffer_ctx;

extern MAIN_COMMANDS_ctx main_commands_ctx;

#ifdef __cplusplus
}
#endif
