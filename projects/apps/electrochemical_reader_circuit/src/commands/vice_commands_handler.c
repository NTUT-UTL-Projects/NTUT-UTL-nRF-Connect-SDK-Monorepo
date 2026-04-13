#include "vice_commands_handler.h"

#define VICE_COMMANDS_COUNTER_MAX_LEN 100

void vice_commands_execute(const vice_commands_ctx *const ctx)
{
    uint16_t len = atomic_load(ctx->vice_commands_buffer_ctx->curr_len);
    uint8_t _temp_buffer[len];

    memcpy(
        _temp_buffer,
        ctx->vice_commands_buffer_ctx->buffer,
        len
    );

    uint16_t vice_counter_index = 0;
    uint16_t vice_counters[VICE_COMMANDS_COUNTER_MAX_LEN] = {0};

    uint16_t curr_len = 0;

    while (curr_len < len)
    {
        if (atomic_load(ctx->vice_commands_buffer_ctx->allow_execute_flag) == false)
        {
            break;
        }

        VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) _temp_buffer[curr_len];
        curr_len++;
        switch (vice_command)
        {
            // --------------------------------------------------
            // write
            case VICE_COMMANDS_WRITE_AfeCtrlSet:
            {
                memcpy(
                    &ctx->ad5940_controller_trigger_para->AfeCtrlSet,
                    _temp_buffer + curr_len,
                    sizeof(ctx->ad5940_controller_trigger_para->AfeCtrlSet)
                );
                curr_len += sizeof(ctx->ad5940_controller_trigger_para->AfeCtrlSet);
                break;
            }
            case VICE_COMMANDS_WRITE_aferef_cfg:
            {
                memcpy(
                    &ctx->ad5940_controller_trigger_para->aferef_cfg,
                    _temp_buffer + curr_len,
                    sizeof(ctx->ad5940_controller_trigger_para->aferef_cfg)
                );
                curr_len += sizeof(ctx->ad5940_controller_trigger_para->aferef_cfg);
                break;
            }
            case VICE_COMMANDS_WRITE_dsp_cfg:
            {
                memcpy(
                    &ctx->ad5940_controller_trigger_para->dsp_cfg,
                    _temp_buffer + curr_len,
                    sizeof(ctx->ad5940_controller_trigger_para->dsp_cfg)
                );
                curr_len += sizeof(ctx->ad5940_controller_trigger_para->dsp_cfg);
                break;
            }
            case VICE_COMMANDS_WRITE_event:
            {
                memcpy(
                    &ctx->ad5940_controller_trigger_para->event,
                    _temp_buffer + curr_len,
                    sizeof(ctx->ad5940_controller_trigger_para->event)
                );
                curr_len += sizeof(ctx->ad5940_controller_trigger_para->event);
                break;
            }
            case VICE_COMMANDS_WRITE_hsloop_cfg:
            {
                memcpy(
                    &ctx->ad5940_controller_trigger_para->hsloop_cfg,
                    _temp_buffer + curr_len,
                    sizeof(ctx->ad5940_controller_trigger_para->hsloop_cfg)
                );
                curr_len += sizeof(ctx->ad5940_controller_trigger_para->hsloop_cfg);
                break;
            }
            case VICE_COMMANDS_WRITE_lploop_cfg:
            {
                memcpy(
                    &ctx->ad5940_controller_trigger_para->lploop_cfg,
                    _temp_buffer + curr_len,
                    sizeof(ctx->ad5940_controller_trigger_para->lploop_cfg)
                );
                curr_len += sizeof(ctx->ad5940_controller_trigger_para->lploop_cfg);
                break;
            }
            // --------------------------------------------------
            // features
            case VICE_COMMANDS_delay:
            {
                VICE_COMMANDS_DELAY_TYPE delay_number = _temp_buffer[curr_len];
                curr_len += sizeof(VICE_COMMANDS_DELAY_TYPE);
                for (size_t i = 0; i < delay_number; i++)
                {
                    ctx->delay_unit();
                    if (atomic_load(ctx->vice_commands_buffer_ctx->allow_execute_flag) == false)
                    {
                        break;
                    }
                }
                break;
            }
            case VICE_COMMANDS_shift:
            {
                VICE_COMMANDS_SHIFT_TYPE shift_number = _temp_buffer[curr_len];
                for (size_t i = 0; i < shift_number; i++)
                {
                    if (vice_counter_index <= 0) break;
                    curr_len -= vice_counters[vice_counter_index - 1];
                    vice_counter_index--;
                }
                break;
            }
            // --------------------------------------------------
            // calibration
            case VICE_COMMANDS_calibrate_ad5940:
            {
                AD5940_CONTROLLER_CAL_RESULTS results;
                AD5940Err err = AD5940_controller_cal(
                    *ctx->ad5940_controller_cal_para,
                    (AD5940_CONTROLLER_CAL_WRITE) {
                        .aferef_cfg = &ctx->ad5940_controller_trigger_para->aferef_cfg,
                        .dsp_cfg = &ctx->ad5940_controller_trigger_para->dsp_cfg,
                        .hsloop_cfg = &ctx->ad5940_controller_trigger_para->hsloop_cfg,
                        .lploop_cfg = &ctx->ad5940_controller_trigger_para->lploop_cfg,
                    },
                    &results
                );
                if (err != AD5940ERR_OK) break;
                *ctx->LpRtiaCal = results.LpRtiaCal;
                *ctx->HsRtiaCal = results.HsRtiaCal;
                *ctx->LpDacPara = results.LpDacPara;
                break;
            }
            // --------------------------------------------------
            // trigger
            case VICE_COMMANDS_trigger_ad5940_controller:
            {
                atomic_store(ctx->ad5940_adc_target_len, ad5940_controller_event_to_adc_number(
                    ctx->ad5940_controller_trigger_para->event
                ));
                atomic_store(ctx->ad5940_adc_curr_len, 0);
                ctx->ad5940_trigger_pre_event();
                AD5940Err err = AD5940_controller_trigger(
                    ctx->ad5940_controller_trigger_para
                );
                if (err != AD5940ERR_OK)
                break;
            }
            default:
            {
                break;
            }
        }
        vice_counters[vice_counter_index] = curr_len;
        vice_counter_index++;
    }
    atomic_store(ctx->vice_commands_buffer_ctx->allow_execute_flag, false);
}