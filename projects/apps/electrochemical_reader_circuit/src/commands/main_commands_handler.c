#include "main_commands_handler.h"

#if defined(CONFIG_MY_BUILD_FAKE_IMPL)
#include <stdio.h>
#define MY_LOG printf
#elif defined(CONFIG_MY_BUILD_ZEPHYR_BASE)
#include <zephyr/logging/log.h>
#define LOG_MODULE_NAME main_commands_handler
LOG_MODULE_REGISTER(LOG_MODULE_NAME, LOG_LEVEL_INF);
#define MY_LOG LOG_INF
#else
#define MY_LOG(...)
#endif

// --------------------------------------------------
// Commands

void main_commands_handler(const main_commands_ctx *const ctx)
{
    uint16_t len = ctx->main_commands_buffer_ctx->len;
    uint8_t *buffer = ctx->main_commands_buffer_ctx->buffer;

    uint16_t curr_len = 0;

    while (curr_len < len)
    {
        MAIN_COMMANDS_ENUM main_command = (MAIN_COMMANDS_ENUM) buffer[curr_len];
        curr_len++;
        MY_LOG("main_command: %d\n", main_command);
        
        switch (main_command)
        {
        case MAIN_COMMANDS_REBOOT:
            MY_LOG("- reboot\n");

            ctx->ad5940_stop();
            ctx->circuit_reboot();
            break;

        case MAIN_COMMANDS_RESET_AD5940:
            MY_LOG("- reset AD5940\n");

            ctx->ad5940_stop();
            AD5940_controller_reset(ctx->ad5940_SEQGenBuff, ctx->ad5940_SEQGenBuffLen, ctx->ad5940_reset_option);
            break;

        case MAIN_COMMANDS_STOP_AD5940:
            MY_LOG("- stop AD5940\n");

            ctx->ad5940_stop();
            break;

        case MAIN_COMMANDS_CLEAR_VICE_BUFF:
            MY_LOG("- clear vice_commands_buff\n");

            memset(ctx->vice_commands_buffer_ctx->buffer, 0, ctx->vice_commands_buffer_ctx->max_len);
            atomic_store(ctx->vice_commands_buffer_ctx->curr_len, 0);
            break;

        case MAIN_COMMANDS_WRITE_VICE_BUFF:
            MY_LOG("- write vice_commands_buff\n");

            VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) buffer[curr_len];
            MY_LOG("- - vice_command[%d]: %d\n", curr_len, vice_command);

            uint16_t vice_commands_buff_curr_len = atomic_load(ctx->vice_commands_buffer_ctx->curr_len);
            uint16_t vice_commands_write_len = 0;

            switch (vice_command)
            {
                // --------------------------------------------------
                // write
                case VICE_COMMANDS_WRITE_AfeCtrlSet:
                {
                    vice_commands_write_len = sizeof(ctx->ad5940_controller_trigger_para->AfeCtrlSet);
                    break;
                }
                case VICE_COMMANDS_WRITE_aferef_cfg:
                {
                    vice_commands_write_len = sizeof(ctx->ad5940_controller_trigger_para->aferef_cfg);
                    break;
                }
                case VICE_COMMANDS_WRITE_dsp_cfg:
                {
                    vice_commands_write_len = sizeof(ctx->ad5940_controller_trigger_para->dsp_cfg);
                    break;
                }
                case VICE_COMMANDS_WRITE_event:
                {
                    vice_commands_write_len = sizeof(ctx->ad5940_controller_trigger_para->event);
                    break;
                }
                case VICE_COMMANDS_WRITE_hsloop_cfg:
                {
                    vice_commands_write_len = sizeof(ctx->ad5940_controller_trigger_para->hsloop_cfg);
                    break;
                }
                case VICE_COMMANDS_WRITE_lploop_cfg:
                {
                    vice_commands_write_len = sizeof(ctx->ad5940_controller_trigger_para->lploop_cfg);
                    break;
                }
                // --------------------------------------------------
                // features
                case VICE_COMMANDS_delay:
                {
                    vice_commands_write_len = sizeof(VICE_COMMANDS_DELAY_TYPE);
                    break;
                }
                case VICE_COMMANDS_shift:
                {
                    vice_commands_write_len = sizeof(VICE_COMMANDS_SHIFT_TYPE);
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
                ctx->vice_commands_buffer_ctx->buffer + vice_commands_buff_curr_len,
                buffer + curr_len,
                vice_commands_write_len
            );
            MY_LOG("- - - {");
            for (size_t i = 0; i < vice_commands_write_len; i++)
            {
                MY_LOG("0x%02x, ", ctx->vice_commands_buffer_ctx->buffer[vice_commands_buff_curr_len + i]);
            }
            MY_LOG("}\n");

            curr_len += vice_commands_write_len;
            atomic_store(ctx->vice_commands_buffer_ctx->curr_len, vice_commands_buff_curr_len + vice_commands_write_len);
            break;

        case MAIN_COMMANDS_START_VICE_BUFF:
            MY_LOG("- start vice_commands_buff\n");

            atomic_store(ctx->vice_commands_buffer_ctx->allow_execute_flag, true);
            break;
        
        default:
            break;
        }
    }
}
