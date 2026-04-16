#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "vice_commands.h"

#include "ad5940_controller.h"

#include <stdatomic.h>
#include <stdint.h>

typedef struct
{
    volatile atomic_bool *allow_execute_flag;
    uint8_t *buffer;
    volatile atomic_uint_fast16_t *curr_len;
    uint16_t max_len;
} vice_commands_buffer_ctx;
typedef struct
{
    AD5940_CONTROLLER_TRIGGER_PARA *ad5940_controller_trigger_para;

    AD5940_CONTROLLER_CAL_PARA *ad5940_controller_cal_para;

    fImpPol_Type *HsRtiaCal;
    LPDACPara_Type *LpDacPara;
    fImpPol_Type *LpRtiaCal;

    volatile atomic_uint_fast16_t *ad5940_adc_target_len;
    volatile atomic_uint_fast16_t *ad5940_adc_curr_len;

    void (*ad5940_trigger_pre_event)(void);

    void (*delay_unit)();

    void (*log)(const char *format, ...);
    void *(*malloc)(size_t size);
    void (*free)(void *ptr);

    vice_commands_buffer_ctx *vice_commands_buffer_ctx;
} vice_commands_ctx;

void vice_commands_handler(const vice_commands_ctx *const ctx);

#ifdef __cplusplus
}
#endif
