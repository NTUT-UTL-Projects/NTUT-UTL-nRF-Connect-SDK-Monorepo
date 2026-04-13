#include "main_commands_handler.h"
#include "vice_commands_handler.h"

volatile atomic_bool main_commands_flag[] = {
#define X(name) false,
	MAIN_COMMANDS_LIST(X)
#undef X
};

void main_commands_handler(uint8_t *buffer, uint16_t len)
{
    uint16_t curr_len = 0;

    while (curr_len < len)
    {
        MAIN_COMMANDS_ENUM main_command = (MAIN_COMMANDS_ENUM) buffer[curr_len];
        curr_len++;
        
        switch (main_command)
        {
        case MAIN_COMMANDS_REBOOT:
            AD5940_STOP();
            // TODO
            /* code */
            break;

        case MAIN_COMMANDS_RESET_AD5940:
            AD5940_STOP();
            AD5940_controller_reset(SEQGenBuff, SEQGenBuffLen, hardware.ad5940_controller_reset);
            break;

        case MAIN_COMMANDS_STOP_AD5940:
            AD5940_STOP();
            break;

        case MAIN_COMMANDS_CLEAR_VICE_BUFF:
            memset(vice_commands_buff, 0, VICE_COMMANDS_BUFF_LEN);
            atomic_store(&vice_commands_buff_final_len, 0);
            break;

        case MAIN_COMMANDS_WRITE_VICE_BUFF:
            VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) buffer[curr_len];
            uint16_t vice_commands_buff_curr_len = atomic_load(&vice_commands_buff_final_len);
            uint16_t vice_commands_write_len;
            switch (vice_command)
            {
                // --------------------------------------------------
                // write
                case VICE_COMMANDS_WRITE_AfeCtrlSet:
                {
                    vice_commands_write_len = sizeof(ad5940_controller_trigger_para.AfeCtrlSet);
                    break;
                }
                case VICE_COMMANDS_WRITE_aferef_cfg:
                {
                    vice_commands_write_len = sizeof(ad5940_controller_trigger_para.aferef_cfg);
                    break;
                }
                case VICE_COMMANDS_WRITE_dsp_cfg:
                {
                    vice_commands_write_len = sizeof(ad5940_controller_trigger_para.dsp_cfg);
                    break;
                }
                case VICE_COMMANDS_WRITE_event:
                {
                    vice_commands_write_len = sizeof(ad5940_controller_trigger_para.event);
                    break;
                }
                case VICE_COMMANDS_WRITE_hsloop_cfg:
                {
                    vice_commands_write_len = sizeof(ad5940_controller_trigger_para.hsloop_cfg);
                    break;
                }
                case VICE_COMMANDS_WRITE_lploop_cfg:
                {
                    vice_commands_write_len = sizeof(ad5940_controller_trigger_para.lploop_cfg);
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
                vice_commands_write_len++;
                memcpy(
                    vice_commands_buff + vice_commands_buff_curr_len,
                    buffer + curr_len,
                    vice_commands_write_len
                );
                curr_len += vice_commands_write_len;
                atomic_store(&vice_commands_buff_final_len, vice_commands_buff_curr_len + vice_commands_write_len);
            }
            break;

        case MAIN_COMMANDS_START_VICE_BUFF:
            atomic_store(&main_commands_flag[MAIN_COMMANDS_START_VICE_BUFF], true);
            break;
        
        default:
            break;
        }
    }
}
