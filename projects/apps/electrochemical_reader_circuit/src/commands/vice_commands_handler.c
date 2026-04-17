#include "vice_commands_handler.h"

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

void create_next_node(void *(*malloc)(size_t size), _node *curr_node, uint8_t *next_ptr)
{
    curr_node->next = malloc(sizeof(_node));
    curr_node->next->curr_ptr = next_ptr;
    curr_node->next->next = NULL;
    curr_node->next->prev = curr_node;
}

void vice_commands_handler(const vice_commands_ctx *const ctx)
{
    uint16_t len = atomic_load(ctx->vice_commands_buffer_ctx->curr_len);
    uint8_t *_temp_buffer = ctx->malloc(len);
    ctx->log("vice_command[%d]\n", len);

    memcpy(
        _temp_buffer,
        ctx->vice_commands_buffer_ctx->buffer,
        len
    );

    _node_head head = {
        .node = ctx->malloc(sizeof(_node)),
    };

    _node *curr_node = head.node;
    curr_node->curr_ptr = _temp_buffer;
    curr_node->prev = NULL;

    while (true)
    {
        if (!atomic_load(ctx->vice_commands_buffer_ctx->allow_execute_flag))
        {
            ctx->log("- NOT allowed.\n");
            break;
        }
        ctx->log("- Allowed.\n");

        ctx->log("- - - - %p\n", curr_node->curr_ptr);
        // if (curr_node->next != NULL) ctx->log("- - - - %p\n", curr_node->next->curr_ptr);
        // if (curr_node->prev != NULL) ctx->log("- - - - %p\n", curr_node->prev->curr_ptr);

        VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) curr_node->curr_ptr[0];
        ctx->log("- vice_command: %d\n", vice_command);

        switch (vice_command)
        {
            // --------------------------------------------------
            // write
            case VICE_COMMANDS_WRITE_AfeCtrlSet:
            {
                ctx->log("- - write AfeCtrlSet\n");
                ctx->log("- - - {");
                for (size_t i = 0; i < sizeof(ctx->ad5940_controller_trigger_para->AfeCtrlSet); i++)
                {
                    ctx->log("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                ctx->log("}\n");

                memcpy(
                    &ctx->ad5940_controller_trigger_para->AfeCtrlSet,
                    curr_node->curr_ptr + 1,
                    sizeof(ctx->ad5940_controller_trigger_para->AfeCtrlSet)
                );

                create_next_node(ctx->malloc, curr_node, curr_node->curr_ptr + 1 + sizeof(ctx->ad5940_controller_trigger_para->AfeCtrlSet));
                curr_node = curr_node->next;
                break;
            }
            case VICE_COMMANDS_WRITE_aferef_cfg:
            {
                ctx->log("- - write aferef_cfg\n");
                ctx->log("- - - {");
                for (size_t i = 0; i < sizeof(ctx->ad5940_controller_trigger_para->aferef_cfg); i++)
                {
                    ctx->log("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                ctx->log("}\n");

                memcpy(
                    &ctx->ad5940_controller_trigger_para->aferef_cfg,
                    curr_node->curr_ptr + 1,
                    sizeof(ctx->ad5940_controller_trigger_para->aferef_cfg)
                );

                create_next_node(ctx->malloc, curr_node, curr_node->curr_ptr + 1 + sizeof(ctx->ad5940_controller_trigger_para->aferef_cfg));
                curr_node = curr_node->next;
                break;
            }
            case VICE_COMMANDS_WRITE_dsp_cfg:
            {
                ctx->log("- - write dsp_cfg\n");
                ctx->log("- - - {");
                for (size_t i = 0; i < sizeof(ctx->ad5940_controller_trigger_para->dsp_cfg); i++)
                {
                    ctx->log("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                ctx->log("}\n");

                memcpy(
                    &ctx->ad5940_controller_trigger_para->dsp_cfg,
                    curr_node->curr_ptr + 1,
                    sizeof(ctx->ad5940_controller_trigger_para->dsp_cfg)
                );

                create_next_node(ctx->malloc, curr_node, curr_node->curr_ptr + 1 + sizeof(ctx->ad5940_controller_trigger_para->dsp_cfg));
                curr_node = curr_node->next;
                break;
            }
            case VICE_COMMANDS_WRITE_event:
            {
                ctx->log("- - write event\n");
                ctx->log("- - - {");
                for (size_t i = 0; i < sizeof(ctx->ad5940_controller_trigger_para->event); i++)
                {
                    ctx->log("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                ctx->log("}\n");

                memcpy(
                    &ctx->ad5940_controller_trigger_para->event,
                    curr_node->curr_ptr + 1,
                    sizeof(ctx->ad5940_controller_trigger_para->event)
                );

                create_next_node(ctx->malloc, curr_node, curr_node->curr_ptr + 1 + sizeof(ctx->ad5940_controller_trigger_para->event));
                curr_node = curr_node->next;
                break;
            }
            case VICE_COMMANDS_WRITE_hsloop_cfg:
            {
                ctx->log("- - write hsloop_cfg\n");
                ctx->log("- - - {");
                for (size_t i = 0; i < sizeof(ctx->ad5940_controller_trigger_para->hsloop_cfg); i++)
                {
                    ctx->log("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                ctx->log("}\n");

                memcpy(
                    &ctx->ad5940_controller_trigger_para->hsloop_cfg,
                    curr_node->curr_ptr + 1,
                    sizeof(ctx->ad5940_controller_trigger_para->hsloop_cfg)
                );

                create_next_node(ctx->malloc, curr_node, curr_node->curr_ptr + 1 + sizeof(ctx->ad5940_controller_trigger_para->hsloop_cfg));
                curr_node = curr_node->next;
                break;
            }
            case VICE_COMMANDS_WRITE_lploop_cfg:
            {
                ctx->log("- - write lploop_cfg\n");
                ctx->log("- - - {");
                for (size_t i = 0; i < sizeof(ctx->ad5940_controller_trigger_para->lploop_cfg); i++)
                {
                    ctx->log("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                ctx->log("}\n");

                memcpy(
                    &ctx->ad5940_controller_trigger_para->lploop_cfg,
                    curr_node->curr_ptr + 1,
                    sizeof(ctx->ad5940_controller_trigger_para->lploop_cfg)
                );

                create_next_node(ctx->malloc, curr_node, curr_node->curr_ptr + 1 + sizeof(ctx->ad5940_controller_trigger_para->lploop_cfg));
                curr_node = curr_node->next;
                break;
            }
            // --------------------------------------------------
            // features
            case VICE_COMMANDS_delay:
            {
                ctx->log("- - delay\n");
                ctx->log("- - - {");
                for (size_t i = 0; i < sizeof(VICE_COMMANDS_DELAY_TYPE); i++)
                {
                    ctx->log("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                ctx->log("}\n");

                VICE_COMMANDS_DELAY_TYPE delay_number;
                memcpy(
                    &delay_number,
                    curr_node->curr_ptr + 1,
                    sizeof(delay_number)
                );

                create_next_node(ctx->malloc, curr_node, curr_node->curr_ptr + 1 + sizeof(VICE_COMMANDS_DELAY_TYPE));
                curr_node = curr_node->next;

                for (size_t i = 0; i < delay_number; i++)
                {
                    ctx->delay_unit_pre_task();
                    ctx->delay_unit();
                    if (!atomic_load(ctx->vice_commands_buffer_ctx->allow_execute_flag))
                    {
                        break;
                    }
                }
                break;
            }
            case VICE_COMMANDS_shift:
            {
                ctx->log("- - shift\n");
                ctx->log("- - - {");
                for (size_t i = 0; i < sizeof(VICE_COMMANDS_SHIFT_TYPE); i++)
                {
                    ctx->log("0x%02x, ", curr_node->curr_ptr[1 + i]);
                }
                ctx->log("}\n");

                VICE_COMMANDS_SHIFT_TYPE shift_number;
                memcpy(
                    &shift_number,
                    curr_node->curr_ptr + 1,
                    sizeof(shift_number)
                );
                ctx->log("- - - - %d\n", shift_number);

                for (size_t i = 0; i < shift_number; i++)
                {
                    if (curr_node->prev == NULL) break;
                    curr_node = curr_node->prev;
                    ctx->free(curr_node->next);
                    curr_node->next = NULL;
                }
                break;
            }
            // --------------------------------------------------
            // calibration
            case VICE_COMMANDS_calibrate_ad5940:
            {
                ctx->log("- - calibrate_ad5940\n");

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
                    ctx->ad5940_controller_trigger_para->LFOSCClkFreq = results.LFOSCClkFreq;
                    ctx->ad5940_controller_trigger_para->RatioSys2AdcClk = results.RatioSys2AdcClk;
                }

                create_next_node(ctx->malloc, curr_node, curr_node->curr_ptr + 1);
                curr_node = curr_node->next;

                break;
            }
            // --------------------------------------------------
            // trigger
            case VICE_COMMANDS_trigger_ad5940_controller:
            {
                ctx->log("- - trigger_ad5940_controller\n");

                atomic_store(ctx->ad5940_adc_target_len, AD5940_controller_event_to_adc_number(
                    ctx->ad5940_controller_trigger_para->event
                ));
                atomic_store(ctx->ad5940_adc_curr_len, 0);
                ctx->ad5940_trigger_pre_task();
                AD5940Err err = AD5940_controller_trigger(
                    ctx->ad5940_controller_trigger_para
                );
                if (err == AD5940ERR_OK) {}

                create_next_node(ctx->malloc, curr_node, curr_node->curr_ptr + 1);
                curr_node = curr_node->next;

                break;
            }
            default:
            {
                break;
            }
        }
    }
    ctx->log("- END");

    while (curr_node->prev != NULL)
    {
        curr_node = curr_node->prev;
        ctx->free(curr_node->next);
        curr_node->next = NULL;
    }
    ctx->free(head.node);

    ctx->free(_temp_buffer);

    atomic_store(ctx->vice_commands_buffer_ctx->allow_execute_flag, false);
}