#include "config.h"
#include "setting.h"

#include <stdio.h>
#include <string.h>

// --------------------------------------------------
// ADC

#define DEMO_ADCPga ADCPGA_1P5

#define DEMO_ADCSinc3Osr ADCSINC3OSR_4  /* Refer to page 57 of the datasheet: `It is recommended to use a decimation rate of 4.`. */
#define DEMO_ADCRate ADCRATE_800KHZ         /* ADC runs at 16MHz clock in this example, sample rate is 800kHz */
#define DEMO_BpSinc3 bFALSE                 /* We use data from SINC3 filter */
#define DEMO_Sinc2NotchEnable bTRUE
#define DEMO_BpNotch bTRUE
#define DEMO_ADCSinc2Osr ADCSINC2OSR_22   /* Refer to page 57 of the datasheet for more details. */
#define DEMO_ADCAvgNum ADCAVGNUM_2          /* Don't care because it's disabled */

// --------------------------------------------------
// AD5940 Controller

static AD5940_CONTROLLER_EVENT ad5940_controller_event;

// --------------------------------------------------
// HS Loop

// This parameter is chosen arbitrarily.
#define DEMO_DiodeClose bFALSE

// We didn't use External Rtia
#define DEMO_ExtRtia 0

/**
 * Refer to page 52 of the datasheet.
 * 31pF + 2pF
 */
#define DEMO_HstiaCtia 31

/**
 * Refer to Figure 28 (page 50) and Figure 36 (page 76) of the datasheet.
 * To streamline the working electrode configuration by ensuring all electrode_routing use the same RTIA value, ignore HstiaDeRload.
 */
#define DEMO_HstiaDeRload HSTIADERLOAD_0R

/**
 * Refer to Figure 28 (page 50) and Figure 36 (page 76) of the datasheet.
 * To streamline the working electrode configuration by ensuring all electrode_routing use the same RTIA value, ignore HstiaDeRtia.
 */
#define DEMO_HstiaDeRtia HSTIADERTIA_OPEN

// Refers to page 76 of the datasheet.
#define DEMO_HstiaRtiaSel HSTIARTIA_10K

#define DEMO_Dswitch 0
#define DEMO_Nswitch 0
#define DEMO_Pswitch 0
#define DEMO_Tswitch (SWT_SE0LOAD | SWT_TRTIA)

// --------------------------------------------------
// LP Loop

#define DEMO_LpAmpPwrMod LPAMPPWR_NORM

#define DEMO_LpTiaRf LPTIARF_SHORT
#define DEMO_LpTiaRload LPTIARLOAD_10R
#define DEMO_LpTiaRtia LPTIARTIA_10K

#define DEMO_LpDacRef LPDACREF_2P5
#define DEMO_LpDacSrc LPDACSRC_MMR              // Use MMR data, we use LPDAC to generate bias voltage for LPTIA - the V_zero

// --------------------------------------------------
// Others

#define DEMO_PGACalType PGACALTYPE_OFFSETGAIN
#define DEMO_VRef1p11 1.11F

/**
 * Refer to pgae 87 of the datasheet.
 * This application use internal reference voltage.
 */
#define DEMO_VRef1p82 1.82F
#define DEMO_ADCRefVolt DEMO_VRef1p82

#define DEMO_DftNum DFTNUM_16
#define DEMO_DftSrc DFTSRC_SINC2NOTCH
#define DEMO_HanWinEn bTRUE

#define DEMO_bWithCtia bTRUE

#define DEMO_DataType DATATYPE_SINC2

#define DEMO_FifoSrc FIFOSRC_SINC2NOTCH

// --------------------------------------------------
// Commands

#define DEMO_delay 60 * 1E1

// --------------------------------------------------
// Main

Setting setting = {0};

int main(void)
{
    MAIN_COMMANDS_ENUM main_command = (MAIN_COMMANDS_ENUM) MAIN_COMMANDS_WRITE_VICE_BUFF;
    uint16_t len = 0;

    // // TODO
    // // Johnny board needs to trigger GPIO multiple time at started to make the following task work.
    // {
    //     {
    //         setting.dsp_cfg.ADCBaseCfg.ADCPga = DEMO_ADCPga;
    //         /* Initialize ADC filters */
    //         setting.dsp_cfg.ADCFilterCfg.ADCAvgNum = DEMO_ADCAvgNum;
    //         setting.dsp_cfg.ADCFilterCfg.ADCRate = DEMO_ADCRate;
    //         setting.dsp_cfg.ADCFilterCfg.ADCSinc2Osr = DEMO_ADCSinc2Osr;
    //         setting.dsp_cfg.ADCFilterCfg.ADCSinc3Osr = DEMO_ADCSinc3Osr;
    //         setting.dsp_cfg.ADCFilterCfg.BpNotch = DEMO_BpNotch;
    //         setting.dsp_cfg.ADCFilterCfg.BpSinc3 = DEMO_BpSinc3;
    //         setting.dsp_cfg.ADCFilterCfg.Sinc2NotchEnable = DEMO_Sinc2NotchEnable;

    //         setting_temperature(&setting);

    //         ad5940_controller_event = (AD5940_CONTROLLER_EVENT) {
    //             .event = AD5940_CONTROLLER_EVENT_TEMPERATURE,
    //             .param = {
    //                 .temperature = {
    //                     .n_step = 1,
    //                     .t_interval = 0.01,
    //                     .TEMPSENS = 0,
    //                 },
    //             },
    //         };
    //     }

    //     {
    //         VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_AfeCtrlSet;
    //         main_buffer[len] = main_command;
    //         len++;
    //         main_buffer[len] = vice_command;
    //         len++;

    //         memcpy(
    //             main_buffer + len,
    //             &setting.AfeCtrlSet,
    //             sizeof(setting.AfeCtrlSet)
    //         );
    //         len += sizeof(setting.AfeCtrlSet);
    //     }
    //     {
    //         VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_aferef_cfg;
    //         main_buffer[len] = main_command;
    //         len++;
    //         main_buffer[len] = vice_command;
    //         len++;

    //         memcpy(
    //             main_buffer + len,
    //             &setting.aferef_cfg,
    //             sizeof(setting.aferef_cfg)
    //         );
    //         len += sizeof(setting.aferef_cfg);
    //     }
    //     {
    //         VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_dsp_cfg;
    //         main_buffer[len] = main_command;
    //         len++;
    //         main_buffer[len] = vice_command;
    //         len++;

    //         memcpy(
    //             main_buffer + len,
    //             &setting.dsp_cfg,
    //             sizeof(setting.dsp_cfg)
    //         );
    //         len += sizeof(setting.dsp_cfg);
    //     }
    //     {
    //         VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_event;
    //         main_buffer[len] = main_command;
    //         len++;
    //         main_buffer[len] = vice_command;
    //         len++;

    //         memcpy(
    //             main_buffer + len,
    //             &ad5940_controller_event,
    //             sizeof(ad5940_controller_event)
    //         );
    //         len += sizeof(ad5940_controller_event);
    //     }
    //     {
    //         VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_hsloop_cfg;
    //         main_buffer[len] = main_command;
    //         len++;
    //         main_buffer[len] = vice_command;
    //         len++;

    //         memcpy(
    //             main_buffer + len,
    //             &setting.hsloop_cfg,
    //             sizeof(setting.hsloop_cfg)
    //         );
    //         len += sizeof(setting.hsloop_cfg);
    //     }
    //     {
    //         VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_lploop_cfg;
    //         main_buffer[len] = main_command;
    //         len++;
    //         main_buffer[len] = vice_command;
    //         len++;
            
    //         memcpy(
    //             main_buffer + len,
    //             &setting.lploop_cfg,
    //             sizeof(setting.lploop_cfg)
    //         );
    //         len += sizeof(setting.lploop_cfg);
    //     }
    //     {
    //         VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_calibrate_ad5940;
    //         main_buffer[len] = main_command;
    //         len++;
    //         main_buffer[len] = vice_command;
    //         len++;
    //     }
    //     for (size_t i = 0; i < 10; i++)
    //     {
    //         {
    //             VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_trigger_ad5940_controller;
    //             main_buffer[len] = main_command;
    //             len++;
    //             main_buffer[len] = vice_command;
    //             len++;
    //         }
    //         {
    //             VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_delay;
    //             main_buffer[len] = main_command;
    //             len++;
    //             main_buffer[len] = vice_command;
    //             len++;

    //             COMMANDS_DELAY_TYPE delay = 1.0;

    //             memcpy(
    //                 main_buffer + len,
    //                 &delay,
    //                 sizeof(delay)
    //             );
    //             len += sizeof(delay);
    //         }
    //     }
    //     {
    //         VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_stop_ad5940;
    //         main_buffer[len] = main_command;
    //         len++;
    //         main_buffer[len] = vice_command;
    //         len++;
    //     }
    // }

    {
        {
            setting.dsp_cfg.ADCBaseCfg.ADCPga = DEMO_ADCPga;
            /* Initialize ADC filters */
            setting.dsp_cfg.ADCFilterCfg.ADCAvgNum = DEMO_ADCAvgNum;
            setting.dsp_cfg.ADCFilterCfg.ADCRate = DEMO_ADCRate;
            setting.dsp_cfg.ADCFilterCfg.ADCSinc2Osr = DEMO_ADCSinc2Osr;
            setting.dsp_cfg.ADCFilterCfg.ADCSinc3Osr = DEMO_ADCSinc3Osr;
            setting.dsp_cfg.ADCFilterCfg.BpNotch = DEMO_BpNotch;
            setting.dsp_cfg.ADCFilterCfg.BpSinc3 = DEMO_BpSinc3;
            setting.dsp_cfg.ADCFilterCfg.Sinc2NotchEnable = DEMO_Sinc2NotchEnable;

            // Refers to https://github.com/XIAN-SHENG-576692/ad5940_applications/blob/8e6cea71d2572c06becd124dddce352dc61521c2/application/electrochemical/utils/dac_tia_adc/ad5940_electrochemical_utils_dac_tia_adc.c
            setting.hsloop_cfg = (HSLoopCfg_Type) {
                .HsDacCfg = {0},
                .HsTiaCfg = {
                    .DiodeClose = DEMO_DiodeClose,
                    .ExtRtia = DEMO_ExtRtia,
                    .HstiaCtia = DEMO_HstiaCtia,
                    .HstiaDeRload = DEMO_HstiaDeRload,
                    .HstiaDeRtia = DEMO_HstiaDeRtia,
                    .HstiaRtiaSel = DEMO_HstiaRtiaSel,

                    #if defined(CHIPSEL_M355)
                    .HstiaDe1Rtia = DEMO_HstiaDe1Rtia
                    .HstiaDe1Rload = DEMO_HstiaDe1Rload,
                    #endif
                },
                .SWMatCfg = {
                    .Tswitch = DEMO_Tswitch,
                },
                .WgCfg = {0},
            };

            setting.lploop_cfg = (LPLoopCfg_Type) {
                .LpAmpCfg = {
                    #if defined(CHIPSEL_M355)
                    .LpAmpSel,
                    #endif
                    .LpAmpPwrMod = DEMO_LpAmpPwrMod,
                    .LpTiaRf = DEMO_LpTiaRf,
                    .LpTiaRload = DEMO_LpTiaRload,
                    .LpTiaRtia = DEMO_LpTiaRtia,
                },
                .LpDacCfg = {
                    .LpDacRef = DEMO_LpDacRef,
                    #if defined(CHIPSEL_M355)
                    .LpdacSel = DEMO_LpdacSel,
                    #endif
                    .LpDacSrc = DEMO_LpDacSrc,
                },
            };

            BoolFlag use_lpdac = bTRUE;
            BoolFlag use_lptia = bFALSE;
            BoolFlag use_hsdac = bFALSE;
            BoolFlag use_hstia = bTRUE;
            setting_electrochemical(&setting, use_lpdac, use_lptia, use_hsdac, use_hstia);

            ad5940_controller_event = (AD5940_CONTROLLER_EVENT) {
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
        }
        {
            VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_AfeCtrlSet;
            main_buffer[len] = main_command;
            len++;
            main_buffer[len] = vice_command;
            len++;

            memcpy(
                main_buffer + len,
                &setting.AfeCtrlSet,
                sizeof(setting.AfeCtrlSet)
            );
            len += sizeof(setting.AfeCtrlSet);
        }
        {
            VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_aferef_cfg;
            main_buffer[len] = main_command;
            len++;
            main_buffer[len] = vice_command;
            len++;

            memcpy(
                main_buffer + len,
                &setting.aferef_cfg,
                sizeof(setting.aferef_cfg)
            );
            len += sizeof(setting.aferef_cfg);
        }
        {
            VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_dsp_cfg;
            main_buffer[len] = main_command;
            len++;
            main_buffer[len] = vice_command;
            len++;

            memcpy(
                main_buffer + len,
                &setting.dsp_cfg,
                sizeof(setting.dsp_cfg)
            );
            len += sizeof(setting.dsp_cfg);
        }
        {
            VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_event;
            main_buffer[len] = main_command;
            len++;
            main_buffer[len] = vice_command;
            len++;

            memcpy(
                main_buffer + len,
                &ad5940_controller_event,
                sizeof(ad5940_controller_event)
            );
            len += sizeof(ad5940_controller_event);
        }
        {
            VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_hsloop_cfg;
            main_buffer[len] = main_command;
            len++;
            main_buffer[len] = vice_command;
            len++;

            memcpy(
                main_buffer + len,
                &setting.hsloop_cfg,
                sizeof(setting.hsloop_cfg)
            );
            len += sizeof(setting.hsloop_cfg);
        }
        {
            VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_WRITE_lploop_cfg;
            main_buffer[len] = main_command;
            len++;
            main_buffer[len] = vice_command;
            len++;
            
            memcpy(
                main_buffer + len,
                &setting.lploop_cfg,
                sizeof(setting.lploop_cfg)
            );
            len += sizeof(setting.lploop_cfg);
        }
        {
            VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_calibrate_ad5940;
            main_buffer[len] = main_command;
            len++;
            main_buffer[len] = vice_command;
            len++;
        }
        {
            VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_trigger_ad5940_controller;
            main_buffer[len] = main_command;
            len++;
            main_buffer[len] = vice_command;
            len++;
        }
        {
            VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_delay;
            main_buffer[len] = main_command;
            len++;
            main_buffer[len] = vice_command;
            len++;

            COMMANDS_DELAY_TYPE delay = DEMO_delay;

            memcpy(
                main_buffer + len,
                &delay,
                sizeof(delay)
            );
            len += sizeof(delay);
        }
        {
            VICE_COMMANDS_ENUM vice_command = (VICE_COMMANDS_ENUM) VICE_COMMANDS_shift;
            main_buffer[len] = main_command;
            len++;
            main_buffer[len] = vice_command;
            len++;

            VICE_COMMANDS_SHIFT_TYPE shift = 2;

            memcpy(
                main_buffer + len,
                &shift,
                sizeof(shift)
            );
            len += sizeof(shift);
        }
    }

    main_commands_ctx.main_commands_buffer_ctx->len = len;

    MAIN_COMMANDS_handler(&main_commands_ctx);
    printf("main_commands_buffer_ctx[%d]: {", len);
    for (size_t i = 0; i < len; i++)
    {
        printf("0x%02x, ", main_commands_buffer_ctx.buffer[i]);
    }
    printf("};\n");

    {
        uint8_t trigger = MAIN_COMMANDS_START_VICE_BUFF;
        main_commands_buffer_ctx.buffer[0] = trigger;
        main_commands_buffer_ctx.len = 1;
        MAIN_COMMANDS_handler(&main_commands_ctx);
    }

    VICE_COMMANDS_handler(&vice_commands_ctx);
    printf("vice_commands_buffer_ctx:[%ld]: {", vice_commands_buff_curr_len);
    uint16_t vice_commands_buff_curr_len = atomic_load(vice_commands_buffer_ctx.curr_len);
    for (size_t i = 0; i < vice_commands_buff_curr_len; i++)
    {
        printf("0x%02x, ", vice_commands_buffer_ctx.buffer[i]);
    }
    printf("};\n");

    return 0;
}