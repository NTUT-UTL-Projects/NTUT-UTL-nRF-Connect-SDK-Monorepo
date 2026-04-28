#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940_controller.h"
#include "vice_commands.h"

#include <stdatomic.h>
#include <stdint.h>

    typedef struct
    {
        volatile atomic_bool          *allow_execute_flag;
        uint8_t                       *buffer;
        volatile atomic_uint_fast16_t *curr_len;
        uint16_t                       max_len;
    } VICE_COMMANDS_buffer_ctx;

    typedef struct
    {
        volatile atomic_uint_fast32_t *deadline; // deadline in ms
        volatile atomic_bool          *is_working;
    } VICE_COMMANDS_state_ctx;
    typedef struct
    {
        AD5940_CONTROLLER_TRIGGER_PARA *ad5940_controller_trigger_para;

        AD5940_CONTROLLER_CAL_PARA *ad5940_controller_cal_para;

        fImpPol_Type   *HsRtiaCal;
        LPDACPara_Type *LpDacPara;
        fImpPol_Type   *LpRtiaCal;

        volatile atomic_uint_fast16_t *ad5940_adc_target_len;
        volatile atomic_uint_fast16_t *ad5940_adc_curr_len;

        void (*ad5940_trigger_pre_task)(void);
        void (*ad5940_stop)(void);

        void (*delay)(void);
        uint32_t delay_time_to_ms;
        uint32_t (*get_monotonic_now)(void); // timer in ms
        void (*log)(const char *format, ...);
        void *(*malloc)(size_t size);
        void (*free)(void *ptr);

        VICE_COMMANDS_buffer_ctx *vice_commands_buffer_ctx;
        VICE_COMMANDS_state_ctx  *vice_commands_state_ctx;
    } VICE_COMMANDS_ctx;

    void VICE_COMMANDS_handler(const VICE_COMMANDS_ctx *const ctx);

#ifdef __cplusplus
}
#endif
