#include "main_commands_handler.h"
#include "vice_commands_handler.h"

uint8_t vice_commands_buff[VICE_COMMANDS_BUFF_LEN];
volatile atomic_uint_fast16_t vice_commands_buff_final_len = 0;

#define VICE_COMMANDS_COUNTER_MAX_LEN 100

void vice_commands_handler(void)
{
    uint16_t vice_commands_buff_curr_len = atomic_load(&vice_commands_buff_final_len);
    uint8_t buffer[vice_commands_buff_curr_len];

    memcpy(
        buffer,
        vice_commands_buff,
        vice_commands_buff_curr_len
    );

    uint16_t vice_counter_index = 0;
    uint16_t vice_counters[VICE_COMMANDS_COUNTER_MAX_LEN] = {0};

    uint16_t curr_len = 0;

    while (curr_len < vice_commands_buff_curr_len)
    {
        if (atomic_load(&main_commands_flag[MAIN_COMMANDS_START_VICE_BUFF]) == false)
        {
            break;
        }

        VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) buffer[curr_len];
        curr_len++;
        switch (vice_command)
        {
            // --------------------------------------------------
            // write
            case VICE_COMMANDS_WRITE_AfeCtrlSet:
            {
                memcpy(
                    &ad5940_controller_trigger_para.AfeCtrlSet,
                    buffer + curr_len,
                    sizeof(ad5940_controller_trigger_para.AfeCtrlSet)
                );
                curr_len += sizeof(ad5940_controller_trigger_para.AfeCtrlSet);
                break;
            }
            case VICE_COMMANDS_WRITE_aferef_cfg:
            {
                memcpy(
                    &ad5940_controller_trigger_para.aferef_cfg,
                    buffer + curr_len,
                    sizeof(ad5940_controller_trigger_para.aferef_cfg)
                );
                curr_len += sizeof(ad5940_controller_trigger_para.aferef_cfg);
                break;
            }
            case VICE_COMMANDS_WRITE_dsp_cfg:
            {
                memcpy(
                    &ad5940_controller_trigger_para.dsp_cfg,
                    buffer + curr_len,
                    sizeof(ad5940_controller_trigger_para.dsp_cfg)
                );
                curr_len += sizeof(ad5940_controller_trigger_para.dsp_cfg);
                break;
            }
            case VICE_COMMANDS_WRITE_event:
            {
                memcpy(
                    &ad5940_controller_trigger_para.event,
                    buffer + curr_len,
                    sizeof(ad5940_controller_trigger_para.event)
                );
                curr_len += sizeof(ad5940_controller_trigger_para.event);
                break;
            }
            case VICE_COMMANDS_WRITE_hsloop_cfg:
            {
                memcpy(
                    &ad5940_controller_trigger_para.hsloop_cfg,
                    buffer + curr_len,
                    sizeof(ad5940_controller_trigger_para.hsloop_cfg)
                );
                curr_len += sizeof(ad5940_controller_trigger_para.hsloop_cfg);
                break;
            }
            case VICE_COMMANDS_WRITE_lploop_cfg:
            {
                memcpy(
                    &ad5940_controller_trigger_para.lploop_cfg,
                    buffer + curr_len,
                    sizeof(ad5940_controller_trigger_para.lploop_cfg)
                );
                curr_len += sizeof(ad5940_controller_trigger_para.lploop_cfg);
                break;
            }
            // --------------------------------------------------
            // features
            case VICE_COMMANDS_delay:
            {
                VICE_COMMANDS_DELAY_TYPE delay_number = buffer[curr_len];
                curr_len += sizeof(VICE_COMMANDS_DELAY_TYPE);
                for (size_t i = 0; i < sizeof(VICE_COMMANDS_DELAY_TYPE); i++)
                {
                    k_sleep(VICE_COMMANDS_DELAY_UNIT(delay_number));
                    if (atomic_load(&main_commands_flag[MAIN_COMMANDS_START_VICE_BUFF]) == false)
                    {
                        break;
                    }
                }
                break;
            }
            case VICE_COMMANDS_shift:
            {
                VICE_COMMANDS_SHIFT_TYPE shift_number = buffer[curr_len];
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
                    hardware.ad5940_controller_cal_para,
                    (AD5940_CONTROLLER_CAL_WRITE) {
                        .aferef_cfg = &ad5940_controller_trigger_para.aferef_cfg,
                        .dsp_cfg = &ad5940_controller_trigger_para.dsp_cfg,
                        .hsloop_cfg = &ad5940_controller_trigger_para.hsloop_cfg,
                        .lploop_cfg = &ad5940_controller_trigger_para.lploop_cfg,
                    },
                    &results
                );
                if (err != AD5940ERR_OK) break;
                LpRtiaCal = results.LpRtiaCal;
                HsRtiaCal = results.HsRtiaCal;
                LpDacPara = results.LpDacPara;
                break;
            }
            // --------------------------------------------------
            // trigger
            case VICE_COMMANDS_trigger_ad5940_controller:
            {
                atomic_store(&ad5940_adc_target_len, ad5940_controller_event_to_adc_number(
                    ad5940_controller_trigger_para.event
                ));
                atomic_store(&ad5940_adc_curr_len, 0);
                atomic_store(&bluetooth_to_ad5940_adc_curr_len, 0);
                AD5940Err err = AD5940_controller_trigger(
                    &ad5940_controller_trigger_para
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
    atomic_store(&main_commands_flag[MAIN_COMMANDS_START_VICE_BUFF], false);
}