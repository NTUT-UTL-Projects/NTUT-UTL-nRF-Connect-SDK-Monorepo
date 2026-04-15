#include "vice_commands_handler.h"

#include <stdlib.h>

#if defined(CONFIG_MY_BUILD_FAKE_IMPL)
#include <stdio.h>
#define MY_LOG printf
#elif defined(CONFIG_MY_BUILD_ZEPHYR_BASE)
#include <zephyr/logging/log.h>
#define LOG_MODULE_NAME vice_commands_handler
LOG_MODULE_REGISTER(LOG_MODULE_NAME, LOG_LEVEL_INF);
#define MY_LOG LOG_INF
#else
#define MY_LOG(...)
#endif

// --------------------------------------------------
// Commands

typedef struct _node _node;
struct _node {
    uint8_t *curr_ptr;
    _node *next;
    _node *prev;
};

typedef struct {
    _node *node;
} _node_head;

void create_next_node(_node *curr_node, uint8_t *next_ptr)
{
    curr_node->next = malloc(sizeof(_node));
    curr_node->next->curr_ptr = next_ptr;
    curr_node->next->next = NULL;
    curr_node->next->prev = curr_node;
}

void vice_commands_handler(const vice_commands_ctx *const ctx)
{
    uint16_t len = atomic_load(ctx->vice_commands_buffer_ctx->curr_len);
    uint8_t _temp_buffer[len];
    MY_LOG("vice_command[%d]\n", len);

    memcpy(
        _temp_buffer,
        ctx->vice_commands_buffer_ctx->buffer,
        len
    );

    _node_head head = {
        .node = malloc(sizeof(_node)),
    };

    _node *curr_node = head.node;
    curr_node->curr_ptr = _temp_buffer;
    curr_node->prev = NULL;

    while (true)
    {
        if (atomic_load(ctx->vice_commands_buffer_ctx->allow_execute_flag) == false)
        {
            MY_LOG("- NOT allowed.\n");
            break;
        }
        MY_LOG("- Allowed.\n");

        MY_LOG("- - - - %p\n", curr_node->curr_ptr);
        if (curr_node->next != NULL) MY_LOG("- - - - %p\n", curr_node->next->curr_ptr);
        if (curr_node->prev != NULL) MY_LOG("- - - - %p\n", curr_node->prev->curr_ptr);

        VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) curr_node->curr_ptr[0];
        MY_LOG("- vice_command: %d\n", vice_command);

        switch (vice_command)
        {
            // --------------------------------------------------
            // write
            case VICE_COMMANDS_WRITE_AfeCtrlSet:
            {
                MY_LOG("- - write AfeCtrlSet\n");
                MY_LOG("- - - {");
                for (size_t i = 0; i < sizeof(ctx->ad5940_controller_trigger_para->AfeCtrlSet); i++)
                {
                    MY_LOG("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                MY_LOG("}\n");

                memcpy(
                    &ctx->ad5940_controller_trigger_para->AfeCtrlSet,
                    curr_node->curr_ptr + 1,
                    sizeof(ctx->ad5940_controller_trigger_para->AfeCtrlSet)
                );

                create_next_node(curr_node, curr_node->curr_ptr + 1 + sizeof(ctx->ad5940_controller_trigger_para->AfeCtrlSet));
                curr_node = curr_node->next;
                break;
            }
            case VICE_COMMANDS_WRITE_aferef_cfg:
            {
                MY_LOG("- - write aferef_cfg\n");
                MY_LOG("- - - {");
                for (size_t i = 0; i < sizeof(ctx->ad5940_controller_trigger_para->aferef_cfg); i++)
                {
                    MY_LOG("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                MY_LOG("}\n");

                memcpy(
                    &ctx->ad5940_controller_trigger_para->aferef_cfg,
                    curr_node->curr_ptr + 1,
                    sizeof(ctx->ad5940_controller_trigger_para->aferef_cfg)
                );

                create_next_node(curr_node, curr_node->curr_ptr + 1 + sizeof(ctx->ad5940_controller_trigger_para->aferef_cfg));
                curr_node = curr_node->next;
                break;
            }
            case VICE_COMMANDS_WRITE_dsp_cfg:
            {
                MY_LOG("- - write dsp_cfg\n");
                MY_LOG("- - - {");
                for (size_t i = 0; i < sizeof(ctx->ad5940_controller_trigger_para->dsp_cfg); i++)
                {
                    MY_LOG("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                MY_LOG("}\n");

                memcpy(
                    &ctx->ad5940_controller_trigger_para->dsp_cfg,
                    curr_node->curr_ptr + 1,
                    sizeof(ctx->ad5940_controller_trigger_para->dsp_cfg)
                );

                create_next_node(curr_node, curr_node->curr_ptr + 1 + sizeof(ctx->ad5940_controller_trigger_para->dsp_cfg));
                curr_node = curr_node->next;
                break;
            }
            case VICE_COMMANDS_WRITE_event:
            {
                MY_LOG("- - write event\n");
                MY_LOG("- - - {");
                for (size_t i = 0; i < sizeof(ctx->ad5940_controller_trigger_para->event); i++)
                {
                    MY_LOG("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                MY_LOG("}\n");

                memcpy(
                    &ctx->ad5940_controller_trigger_para->event,
                    curr_node->curr_ptr + 1,
                    sizeof(ctx->ad5940_controller_trigger_para->event)
                );

                create_next_node(curr_node, curr_node->curr_ptr + 1 + sizeof(ctx->ad5940_controller_trigger_para->event));
                curr_node = curr_node->next;
                break;
            }
            case VICE_COMMANDS_WRITE_hsloop_cfg:
            {
                MY_LOG("- - write hsloop_cfg\n");
                MY_LOG("- - - {");
                for (size_t i = 0; i < sizeof(ctx->ad5940_controller_trigger_para->hsloop_cfg); i++)
                {
                    MY_LOG("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                MY_LOG("}\n");

                memcpy(
                    &ctx->ad5940_controller_trigger_para->hsloop_cfg,
                    curr_node->curr_ptr + 1,
                    sizeof(ctx->ad5940_controller_trigger_para->hsloop_cfg)
                );

                create_next_node(curr_node, curr_node->curr_ptr + 1 + sizeof(ctx->ad5940_controller_trigger_para->hsloop_cfg));
                curr_node = curr_node->next;
                break;
            }
            case VICE_COMMANDS_WRITE_lploop_cfg:
            {
                MY_LOG("- - write lploop_cfg\n");
                MY_LOG("- - - {");
                for (size_t i = 0; i < sizeof(ctx->ad5940_controller_trigger_para->lploop_cfg); i++)
                {
                    MY_LOG("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                MY_LOG("}\n");

                memcpy(
                    &ctx->ad5940_controller_trigger_para->lploop_cfg,
                    curr_node->curr_ptr + 1,
                    sizeof(ctx->ad5940_controller_trigger_para->lploop_cfg)
                );

                create_next_node(curr_node, curr_node->curr_ptr + 1 + sizeof(ctx->ad5940_controller_trigger_para->lploop_cfg));
                curr_node = curr_node->next;
                break;
            }
            // --------------------------------------------------
            // features
            case VICE_COMMANDS_delay:
            {
                MY_LOG("- - delay\n");
                MY_LOG("- - - {");
                for (size_t i = 0; i < sizeof(VICE_COMMANDS_DELAY_TYPE); i++)
                {
                    MY_LOG("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                MY_LOG("}\n");

                VICE_COMMANDS_DELAY_TYPE delay_number;
                memcpy(
                    &delay_number,
                    curr_node->curr_ptr + 1,
                    sizeof(delay_number)
                );

                create_next_node(curr_node, curr_node->curr_ptr + 1 + sizeof(VICE_COMMANDS_DELAY_TYPE));
                curr_node = curr_node->next;

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
                MY_LOG("- - shift\n");
                MY_LOG("- - - {");
                for (size_t i = 0; i < sizeof(VICE_COMMANDS_SHIFT_TYPE); i++)
                {
                    MY_LOG("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                MY_LOG("}\n");

                VICE_COMMANDS_SHIFT_TYPE shift_number;
                memcpy(
                    &shift_number,
                    curr_node->curr_ptr + 1,
                    sizeof(shift_number)
                );
                MY_LOG("- - - - %d\n", shift_number);

                for (size_t i = 0; i < shift_number; i++)
                {
                    if (curr_node->prev == NULL) break;
                    curr_node = curr_node->prev;
                    free(curr_node->next);
                    curr_node->next = NULL;
                }
                break;
            }
            // --------------------------------------------------
            // calibration
            case VICE_COMMANDS_calibrate_ad5940:
            {
                MY_LOG("- - calibrate_ad5940\n");

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
                if (err == AD5940ERR_OK)
                {
                    *ctx->LpRtiaCal = results.LpRtiaCal;
                    *ctx->HsRtiaCal = results.HsRtiaCal;
                    *ctx->LpDacPara = results.LpDacPara;
                }

                create_next_node(curr_node, curr_node->curr_ptr + 1);
                curr_node = curr_node->next;

                break;
            }
            // --------------------------------------------------
            // trigger
            case VICE_COMMANDS_trigger_ad5940_controller:
            {
                MY_LOG("- - trigger_ad5940_controller\n");

                atomic_store(ctx->ad5940_adc_target_len, ad5940_controller_event_to_adc_number(
                    ctx->ad5940_controller_trigger_para->event
                ));
                atomic_store(ctx->ad5940_adc_curr_len, 0);
                ctx->ad5940_trigger_pre_event();
                AD5940Err err = AD5940_controller_trigger(
                    ctx->ad5940_controller_trigger_para
                );
                if (err == AD5940ERR_OK) {}

                create_next_node(curr_node, curr_node->curr_ptr + 1);
                curr_node = curr_node->next;

                break;
            }
            default:
            {
                break;
            }
        }
    }
    MY_LOG("- END");

    while (curr_node->prev != NULL)
    {
        *curr_node = *curr_node->prev;
        free(curr_node->next);
        curr_node->next = NULL;
    }
    free(head.node);

    atomic_store(ctx->vice_commands_buffer_ctx->allow_execute_flag, false);
}