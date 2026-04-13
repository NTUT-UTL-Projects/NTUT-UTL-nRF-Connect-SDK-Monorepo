#include "config.h"

#define MAIN_BUFFER_LEN 8192
uint8_t main_buffer[MAIN_BUFFER_LEN] = {0};

#define DEMO_USE_LPDAC
#define DEMO_USE_HSTIA

int main(void)
{
    MAIN_COMMANDS_ENUM main_command = (MAIN_COMMANDS_ENUM) MAIN_COMMANDS_WRITE_VICE_BUFF;
    uint16_t len = 0;
    {
        VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_AfeCtrlSet;
        main_buffer[len] = main_command;
        len++;
        main_buffer[len] = vice_command;
        len++;

        // Refers to https://github.com/XIAN-SHENG-576692/ad5940_applications/blob/8e6cea71d2572c06becd124dddce352dc61521c2/application/electrochemical/utils/afe_dac_tia/ad5940_electrochemical_utils_afe_dac_tia.c
        // if use LPTIA
        // uint32_t AfeCtrlSet = 0;
        // if use HSTIA
        uint32_t AfeCtrlSet = 0 | AFECTRL_HSTIAPWR;

        memcpy(
            main_buffer + len,
            &AfeCtrlSet,
            sizeof(AfeCtrlSet)
        );
        len += sizeof(AfeCtrlSet);
    }
    {
        VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_aferef_cfg;
        main_buffer[len] = main_command;
        len++;
        main_buffer[len] = vice_command;
        len++;

        // Refers to https://github.com/XIAN-SHENG-576692/ad5940_applications/blob/8e6cea71d2572c06becd124dddce352dc61521c2/application/electrochemical/utils/afe_dac_tia/ad5940_electrochemical_utils_afe_dac_tia.c
        AFERefCfg_Type aferef_cfg = {
            /**
             * Enable the high-precision voltage references.
             * Refer to page 25 and Figure 37 (page 87) of the datasheet.
             */
            .HpBandgapEn = bTRUE,

            /**
             * Recommended high-precision buffer settings.
             * Refer to page 87 of the datasheet for details.
             */
            .Hp1V1BuffEn = bTRUE,
            .Hp1V8BuffEn = bTRUE,
            .Disc1V1Cap = bFALSE,
            .Disc1V8Cap = bFALSE,
            .Hp1V8ThemBuff = bFALSE,
            .Hp1V8Ilimit = bFALSE,
            .Lp1V1BuffEn = bFALSE,
            .Lp1V8BuffEn = bFALSE,
            
            // LP reference control - turn off them to save power
            .LpRefBoostEn = bFALSE,

            // @see page 34 of the datasheet, With bias voltage
            #if defined(DEMO_USE_LPDAC)
            .LpBandgapEn = bTRUE,
            .LpRefBufEn = bTRUE,
            #else
            .LpBandgapEn = bFALSE,
            .LpRefBufEn = bFALSE,
            #endif
        };

        memcpy(
            main_buffer + len,
            &aferef_cfg,
            sizeof(aferef_cfg)
        );
        len += sizeof(aferef_cfg);
    }
    {
        VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_dsp_cfg;
        main_buffer[len] = main_command;
        len++;
        main_buffer[len] = vice_command;
        len++;

        // Refers to https://github.com/XIAN-SHENG-576692/ad5940_applications/blob/8e6cea71d2572c06becd124dddce352dc61521c2/application/electrochemical/utils/dac_tia_adc/ad5940_electrochemical_utils_dac_tia_adc.c
        // Refers to https://github.com/analogdevicesinc/ad5940-examples/blob/master/examples/AD5940_SqrWaveVoltammetry/SqrWaveVoltammetry.c
        DSPCfg_Type dsp_cfg = {
            #if defined(DEMO_USE_LPTIA)
            .ADCBaseCfg.ADCMuxN = ADCMUXN_LPTIA0_N,
            .ADCBaseCfg.ADCMuxP = ADCMUXP_LPTIA0_P,
            #else if defined(DEMO_USE_HSTIA)
            .ADCBaseCfg.ADCMuxN = ADCMUXN_HSTIA_N,
            .ADCBaseCfg.ADCMuxP = ADCMUXP_HSTIA_P,
            #endif

            .ADCBaseCfg.ADCPga = ADCPGA_1P5,
            
            .ADCFilterCfg.ADCSinc3Osr = ADCSINC3OSR_4,
            .ADCFilterCfg.ADCRate = ADCRATE_800KHZ,         /* ADC runs at 16MHz clock in this example, sample rate is 800kHz */
            .ADCFilterCfg.BpSinc3 = bFALSE,                 /* We use data from SINC3 filter */
            .ADCFilterCfg.Sinc2NotchEnable = bTRUE,
            .ADCFilterCfg.BpNotch = bTRUE,
            .ADCFilterCfg.ADCSinc2Osr = ADCSINC2OSR_1067,   /* Don't care */
            .ADCFilterCfg.ADCAvgNum = ADCAVGNUM_2,          /* Don't care because it's disabled */
        };

        memcpy(
            main_buffer + len,
            &dsp_cfg,
            sizeof(dsp_cfg)
        );
        len += sizeof(dsp_cfg);
    }
    {
        VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_event;
        main_buffer[len] = main_command;
        len++;
        main_buffer[len] = vice_command;
        len++;

        AD5940_CONTROLLER_EVENT event = {
            .event = AD5940_CONTROLLER_EVENT_DPV,
            .param = {
                .dpv = {
                    .e_begin = 0.2,
                    .e_pulse = 0.05,
                    .e_step = 0.01,
                    .t_pulse = 0.01,
                    .t_step = 0.02,
                    .n_step = 10,
                }
            }
        };

        memcpy(
            main_buffer + len,
            &event,
            sizeof(event)
        );
        len += sizeof(event);
    }
    {
        VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_hsloop_cfg;
        main_buffer[len] = main_command;
        len++;
        main_buffer[len] = vice_command;
        len++;

        // Refers to https://github.com/XIAN-SHENG-576692/ad5940_applications/blob/8e6cea71d2572c06becd124dddce352dc61521c2/application/electrochemical/utils/dac_tia_adc/ad5940_electrochemical_utils_dac_tia_adc.c
        HSLoopCfg_Type hsloop_cfg = {
            #if defined(DEMO_USE_HSDAC)
            .HsDacCfg = {0},
            .WgCfg = {0},
            #endif
            #if defined(DEMO_USE_HSTIA)
            .HsTiaCfg = {
                .DiodeClose = utility_type->DiodeClose,
                .ExtRtia = utility_type->ExtRtia,
                .HstiaCtia = utility_type->HstiaCtia,
                .HstiaDeRload = utility_type->HstiaDeRload,
                .HstiaDeRtia = utility_type->HstiaDeRtia,
                .HstiaRtiaSel = utility_type->HstiaRtiaSel,
                #if defined(CHIPSEL_M355)
                .HstiaDe1Rtia
                .HstiaDe1Rload
                #endif

                // @see page 45 of the datasheet, With bias voltage */
                #if defined(DEMO_USE_LPDAC)
                .HstiaBias = HSTIABIAS_VZERO0,
                #else
                .HstiaBias = HSTIABIAS_1P1,
                #endif
            },
            .SWMatCfg = {
                #if defined(DEMO_USE_HSDAC)
                .Dswitch,
                .Nswitch,
                .Pswitch,
                #endif
                #if defined(DEMO_USE_HSTIA)
                .Tswitch = SWT_SE0LOAD | SWT_TRTIA,
                #endif
            },
            #endif
        };

        memcpy(
            main_buffer + len,
            &hsloop_cfg,
            sizeof(hsloop_cfg)
        );
        len += sizeof(hsloop_cfg);
    }
    {
        VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_lploop_cfg;
        main_buffer[len] = main_command;
        len++;
        main_buffer[len] = vice_command;
        len++;

        // Refers to https://github.com/XIAN-SHENG-576692/ad5940_applications/blob/8e6cea71d2572c06becd124dddce352dc61521c2/application/electrochemical/utils/dac_tia_adc/ad5940_electrochemical_utils_dac_tia_adc.c
        LPLoopCfg_Type lploop_cfg = {
            
            #if defined(CHIPSEL_M355)
            .LpAmpSel = lpdac_cfg->LpAmpSel,
            #else
            .LpAmpSel = LPAMP0,
            #endif
            .LpAmpPwrMod = lpdac_cfg->LpAmpPwrMod,
            .LpPaPwrEn = bTRUE,
            .LpTiaPwrEn = bFALSE,
            .LpTiaSW = 0
                | LPTIASW(2)
                | LPTIASW(4)

                // When using LPDAC with HATIA to start the electrochemical reaction, 
                // close LPTIASW(6) to prevent the signal from being generated on SE0.
                | LPTIASW(6)

                | LPTIASW(7)
        };
        
        memcpy(
            main_buffer + len,
            &lploop_cfg,
            sizeof(lploop_cfg)
        );
        len += sizeof(lploop_cfg);
    }

    main_commands_handler();

    return 0;
}