#include "main_commands_handler.h"

// --------------------------------------------------
// Commands

#define _SAFETY_AD5940_DELAY 5000
#define _SAFETY_COMMON_DELAY 3000

void MAIN_COMMANDS_handler(const MAIN_COMMANDS_ctx *const ctx)
{
    uint16_t len    = ctx->main_commands_buffer_ctx->len;
    uint8_t *buffer = ctx->main_commands_buffer_ctx->buffer;

    uint16_t curr_len = 0;

    atomic_store(
        ctx->main_commands_state_ctx->deadline,
        ctx->get_monotonic_now() + _SAFETY_COMMON_DELAY
    );
    atomic_store(ctx->main_commands_state_ctx->is_working, true);

    while (curr_len < len)
    {
        MAIN_COMMANDS_ENUM main_command = (MAIN_COMMANDS_ENUM)buffer[curr_len];
        curr_len++;
        ctx->log("main_command: %d\n", main_command);

        switch (main_command)
        {
            case MAIN_COMMANDS_REBOOT:
                ctx->log("- reboot\n");

                atomic_store(
                    ctx->main_commands_state_ctx->deadline,
                    ctx->get_monotonic_now() + _SAFETY_AD5940_DELAY
                );

                atomic_store(
                    ctx->vice_commands_buffer_ctx->allow_execute_flag,
                    false
                );
                atomic_store(
                    ctx->vice_commands_buffer_ctx->allow_execute_flag,
                    false
                );
                ctx->ad5940_stop();
                ctx->circuit_reboot();
                break;

            case MAIN_COMMANDS_RESET_AD5940:
                ctx->log("- reset AD5940\n");

                atomic_store(
                    ctx->main_commands_state_ctx->deadline,
                    ctx->get_monotonic_now() + _SAFETY_AD5940_DELAY
                );

                atomic_store(
                    ctx->vice_commands_buffer_ctx->allow_execute_flag,
                    false
                );
                ctx->ad5940_stop();
                AD5940_controller_reset(
                    ctx->ad5940_SEQGenBuff,
                    ctx->ad5940_SEQGenBuffLen,
                    ctx->ad5940_reset_option
                );
                break;

            case MAIN_COMMANDS_STOP_AD5940:
                ctx->log("- stop AD5940\n");

                atomic_store(
                    ctx->main_commands_state_ctx->deadline,
                    ctx->get_monotonic_now() + _SAFETY_AD5940_DELAY
                );

                atomic_store(
                    ctx->vice_commands_buffer_ctx->allow_execute_flag,
                    false
                );
                ctx->ad5940_stop();
                break;

            case MAIN_COMMANDS_CLEAR_VICE_BUFF:
                ctx->log("- clear vice_commands_buff\n");

                atomic_store(
                    ctx->main_commands_state_ctx->deadline,
                    ctx->get_monotonic_now() + _SAFETY_COMMON_DELAY
                );

                memset(
                    ctx->vice_commands_buffer_ctx->buffer,
                    0,
                    ctx->vice_commands_buffer_ctx->max_len
                );
                atomic_store(ctx->vice_commands_buffer_ctx->curr_len, 0);
                break;

            case MAIN_COMMANDS_WRITE_VICE_BUFF:
                ctx->log("- write vice_commands_buff\n");

                atomic_store(
                    ctx->main_commands_state_ctx->deadline,
                    ctx->get_monotonic_now() + _SAFETY_COMMON_DELAY
                );

                VICE_COMMANDS_ENUM vice_command =
                    (VICE_COMMANDS_ENUM)buffer[curr_len];
                ctx->log("- - vice_command[%d]: %d\n", curr_len, vice_command);

                uint16_t vice_commands_buff_curr_len =
                    atomic_load(ctx->vice_commands_buffer_ctx->curr_len);
                uint16_t vice_commands_write_len = 0;

                switch (vice_command)
                {
                    // --------------------------------------------------
                    // write
                    case VICE_COMMANDS_WRITE_AfeCtrlSet:
                    {
                        vice_commands_write_len = sizeof(
                            ctx->ad5940_controller_trigger_para->AfeCtrlSet
                        );
                        break;
                    }
                    case VICE_COMMANDS_WRITE_aferef_cfg:
                    {
                        vice_commands_write_len = sizeof(
                            ctx->ad5940_controller_trigger_para->aferef_cfg
                        );
                        break;
                    }
                    case VICE_COMMANDS_WRITE_dsp_cfg:
                    {
                        vice_commands_write_len = sizeof(
                            ctx->ad5940_controller_trigger_para->dsp_cfg
                        );
                        break;
                    }
                    case VICE_COMMANDS_WRITE_event:
                    {
                        vice_commands_write_len =
                            sizeof(ctx->ad5940_controller_trigger_para->event);
                        break;
                    }
                    case VICE_COMMANDS_WRITE_hsloop_cfg:
                    {
                        vice_commands_write_len = sizeof(
                            ctx->ad5940_controller_trigger_para->hsloop_cfg
                        );
                        break;
                    }
                    case VICE_COMMANDS_WRITE_lploop_cfg:
                    {
                        vice_commands_write_len = sizeof(
                            ctx->ad5940_controller_trigger_para->lploop_cfg
                        );
                        break;
                    }
                    // --------------------------------------------------
                    // features
                    case VICE_COMMANDS_delay:
                    {
                        vice_commands_write_len = sizeof(COMMANDS_DELAY_TYPE);
                        break;
                    }
                    case VICE_COMMANDS_shift:
                    {
                        vice_commands_write_len =
                            sizeof(VICE_COMMANDS_SHIFT_TYPE);
                        break;
                    }
                    // --------------------------------------------------
                    // calibration
                    case VICE_COMMANDS_calibrate_ad5940:
                        break;
                    // --------------------------------------------------
                    // trigger
                    case VICE_COMMANDS_trigger_ad5940_controller:
                        break;
                    default:
                    {
                        break;
                    }
                }
                vice_commands_write_len++;
                memcpy(
                    ctx->vice_commands_buffer_ctx->buffer +
                        vice_commands_buff_curr_len,
                    buffer + curr_len,
                    vice_commands_write_len
                );
                ctx->log("- - - {");
                for (size_t i = 0; i < vice_commands_write_len; i++)
                {
                    ctx->log(
                        "0x%02x, ",
                        ctx->vice_commands_buffer_ctx
                            ->buffer[vice_commands_buff_curr_len + i]
                    );
                }
                ctx->log("}\n");

                curr_len += vice_commands_write_len;
                atomic_store(
                    ctx->vice_commands_buffer_ctx->curr_len,
                    vice_commands_buff_curr_len + vice_commands_write_len
                );
                break;

            case MAIN_COMMANDS_START_VICE_BUFF:
                ctx->log("- start vice_commands_buff\n");

                atomic_store(
                    ctx->main_commands_state_ctx->deadline,
                    ctx->get_monotonic_now() + _SAFETY_COMMON_DELAY
                );

                atomic_store(
                    ctx->vice_commands_buffer_ctx->allow_execute_flag,
                    true
                );
                break;

            default:
                break;
        }
    }
    atomic_store(ctx->main_commands_state_ctx->is_working, false);
}
