#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "main_commands.h"
#include "vice_commands.h"

#include "ad5940.h"

#include "hardware.h"

#include <stdatomic.h>
#include <stdint.h>

#define VICE_COMMANDS_BUFF_LEN 2048
extern uint8_t vice_commands_buff[VICE_COMMANDS_BUFF_LEN];
extern volatile atomic_uint_fast16_t vice_commands_buff_final_len;

extern fImpPol_Type HsRtiaCal;
extern LPDACPara_Type LpDacPara;
extern fImpPol_Type LpRtiaCal;

extern uint32_t ad5940_adc_fifo_buff[];
extern volatile atomic_uint_fast16_t ad5940_adc_target_len;
extern volatile atomic_uint_fast16_t ad5940_adc_curr_len;

extern volatile atomic_uint_fast16_t bluetooth_to_ad5940_adc_curr_len;

void vice_commands_handler(void);

#ifdef __cplusplus
}
#endif
