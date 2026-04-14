#include "config.h"

#include <stdio.h>
#include <string.h>

// --------------------------------------------------

#define DEMO_USE_LPDAC
#define DEMO_USE_HSTIA

#define DEMO_ADCPga ADCPGA_1P5

#define DEMO_ADCSinc3Osr ADCSINC3OSR_4  /* Refer to page 57 of the datasheet: `It is recommended to use a decimation rate of 4.`. */
#define DEMO_ADCRate ADCRATE_800KHZ         /* ADC runs at 16MHz clock in this example, sample rate is 800kHz */
#define DEMO_BpSinc3 bFALSE                 /* We use data from SINC3 filter */
#define DEMO_Sinc2NotchEnable bTRUE
#define DEMO_BpNotch bTRUE
#define DEMO_ADCSinc2Osr ADCSINC2OSR_22   /* Refer to page 57 of the datasheet for more details. */
#define DEMO_ADCAvgNum ADCAVGNUM_2          /* Don't care because it's disabled */

static AD5940_CONTROLLER_EVENT ad5940_controller_event = {
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

// --------------------------------------------------

#define DEMO_LpAmpPwrMod LPAMPPWR_NORM
#define DEMO_LpAmpSel LPAMP0
#define DEMO_LpTiaRf LPTIARF_SHORT
#define DEMO_LpTiaRload LPTIARLOAD_10R

#define DEMO_LpDacRef LPDACREF_2P5
#define DEMO_LpDacSrc LPDACSRC_MMR              // Use MMR data, we use LPDAC to generate bias voltage for LPTIA - the V_zero

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

#define DEMO_ExtRtia 0
#define DEMO_HstiaRtiaSel HSTIARTIA_10K

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

// This parameter is chosen arbitrarily.
#define DEMO_DiodeClose bFALSE

#define DEMO_DataType DATATYPE_SINC2

#define DEMO_FifoSrc FIFOSRC_SINC2NOTCH

// --------------------------------------------------

#define DEMO_Dswitch 0
#define DEMO_Nswitch 0
#define DEMO_Pswitch 0
#define DEMO_Tswitch (SWT_SE0LOAD | SWT_TRTIA)

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
            .ADCBaseCfg = {
                #if defined(DEMO_USE_LPTIA)
                .ADCMuxN = ADCMUXN_LPTIA0_N,
                .ADCMuxP = ADCMUXP_LPTIA0_P,
                #elif defined(DEMO_USE_HSTIA)
                .ADCMuxN = ADCMUXN_HSTIA_N,
                .ADCMuxP = ADCMUXP_HSTIA_P,
                #endif
                .ADCPga = DEMO_ADCPga,
            },
            .ADCFilterCfg = {
                .ADCSinc3Osr = DEMO_ADCSinc3Osr,
                .ADCRate = DEMO_ADCRate,
                .BpSinc3 = DEMO_BpSinc3,
                .Sinc2NotchEnable = DEMO_Sinc2NotchEnable,
                .BpNotch = DEMO_BpNotch,
                .ADCSinc2Osr = DEMO_ADCSinc2Osr,
                .ADCAvgNum = DEMO_ADCAvgNum,
            },
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

        AD5940_CONTROLLER_EVENT event = ad5940_controller_event;

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
            #endif
            #if defined(DEMO_USE_HSTIA)
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

                // @see page 45 of the datasheet, With bias voltage */
                #if defined(DEMO_USE_LPDAC)
                .HstiaBias = HSTIABIAS_VZERO0,
                #else
                .HstiaBias = HSTIABIAS_1P1,
                #endif
            },
            /** 
             * Refers to https://github.com/XIAN-SHENG-576692/ad5940_applications/blob/8e6cea71d2572c06becd124dddce352dc61521c2/application/electrochemical/utils/dac_tia_adc/ad5940_electrochemical_utils_electrode_routing.h
             * @note
             * - For detailed configuration guidance, refer to the datasheet:
             *   - Pages 30–52: Overview of LPLoop and HSLoop configurations
             *   - Pages 74–76: Electrochemical sensor setup and switch matrix configuration
             * - See Figure 36 (Page 76) for the switch matrix routing diagram.
             */
            .SWMatCfg = {
                #if defined(DEMO_USE_HSDAC)
                .Dswitch = DEMO_Dswitch,
                .Nswitch = DEMO_Nswitch,
                .Pswitch = DEMO_Pswitch,
                #endif
                #if defined(DEMO_USE_HSTIA)
                .Tswitch = DEMO_Tswitch,
                #endif
            },
            #if defined(DEMO_USE_HSDAC)
            .WgCfg = {0},
            #endif
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

        /**
         * @note 
         * Refers to page 45 of the datasheet.
         * 
         * ```
         * Note that the high speed DAC signal chain must never be used
         * in conjunction with the low power TIA. The high speed DAC can
         * become unstable, leading to incorrect measurements.
         * ```
         */
        LPLoopCfg_Type lploop_cfg = {
            .LpAmpCfg = {
                #if defined(DEMO_USE_LPDAC)
                #if defined(CHIPSEL_M355)
                .LpAmpSel,
                #else
                .LpAmpSel = LPAMP0,
                #endif
                .LpAmpPwrMod = DEMO_LpAmpPwrMod,
                .LpPaPwrEn = bTRUE,
                #endif
                #if defined(DEMO_USE_LPTIA) && !defined(DEMO_USE_HSDAC)
                .LpTiaPwrEn = bTRUE,
                .LpTiaRf = 0,
                .LpTiaRload = 0,
                .LpTiaRtia = 0,
                #if defined(DEMO_LpTiaRtia) && DEMO_LpTiaRtia == LPTIARTIA_OPEN
                .LpTiaSW = 0
                    | LPTIASW(2)
                    | LPTIASW(4)
                    | LPTIASW(5)
                    | LPTIASW(9)
                    // | LPTIASW(12)
                    // | LPTIASW(13)
                #else
                .LpTiaSW = 0
                    | LPTIASW(2)
                    | LPTIASW(4)
                    | LPTIASW(5)
                    // | LPTIASW(12)
                    // | LPTIASW(13)
                #endif
                #else
                .LpTiaPwrEn = bFALSE,
                .LpTiaRf = 0,
                .LpTiaRload = 0,
                .LpTiaRtia = 0,
                .LpTiaSW = 0
                    | LPTIASW(2)
                    | LPTIASW(4)

                    // When using LPDAC with HATIA to start the electrochemical reaction, 
                    // close LPTIASW(6) to prevent the signal from being generated on SE0.
                    | LPTIASW(6)

                    | LPTIASW(7)
                ,
                #endif
            },
            #if defined(DEMO_USE_LPDAC)
            .LpDacCfg = {
                .DacData12Bit = 0x00,
                .DacData6Bit = 0x00,
                .DataRst = bFALSE,                     // Do not reset data register
                .LpDacRef = DEMO_LpDacRef,
                #if defined(CHIPSEL_M355)
                .LpdacSel = DEMO_LpdacSel,
                #else
                .LpdacSel = LPDAC0,
                #endif
                .LpDacSrc = DEMO_LpDacSrc,
                // Refer to page 39 of the datasheet, LPPA receives feedback to support LPDAC in producing a precise output signal.
                #if defined(DEMO_USE_LPTIA)
                .LpDacSW = 0 
                        | LPDACSW_VBIAS2LPPA 
                        | LPDACSW_VZERO2LPTIA
                    ,
                #elif defined(DEMO_USE_HSTIA)
                .LpDacSW = 0
                    | LPDACSW_VBIAS2LPPA 
                    | LPDACSW_VZERO2HSTIA
                ,
                #else
                .LpDacSW = 0 
                    | LPDACSW_VBIAS2LPPA
                ,
                #endif
                .LpDacVbiasMux = LPDACVBIAS_12BIT,
                .LpDacVzeroMux = LPDACVZERO_6BIT,
                .PowerEn = bTRUE,                      // Power up
            },
            #endif
        };
        
        memcpy(
            main_buffer + len,
            &lploop_cfg,
            sizeof(lploop_cfg)
        );
        len += sizeof(lploop_cfg);
    }

    main_commands_handler(&main_ctx);
    vice_commands_handler(&vice_ctx);

    printf("main_commands_buff_ctx: ");
    for (size_t i = 0; i < main_commands_buff_ctx.len; i++)
    {
        printf("0x%02x,", main_commands_buff_ctx.buffer[i]);
    }

    printf("\n");

    printf("vice_commands_buff_ctx: ");
    uint16_t vice_commands_buff_curr_len = atomic_load(vice_commands_buff_ctx.curr_len);
    for (size_t i = 0; i < vice_commands_buff_curr_len; i++)
    {
        printf("0x%02x,", vice_commands_buff_ctx.buffer[i]);
    }

    printf("\n");

    return 0;
}