#include "setting.h"

void setting_electrochemical(
    Setting *const setting,
    BoolFlag       use_lpdac,
    BoolFlag       use_lptia,
    BoolFlag       use_hsdac,
    BoolFlag       use_hstia
)
{
    // ==================================================
    // AfeCtrlSet
    setting->AfeCtrlSet = 0;
    if (use_hstia)
    {
        setting->AfeCtrlSet |= AFECTRL_HSTIAPWR;
    }

    // ==================================================
    // AFERefCfg_Type
    // Refers to
    // https://github.com/XIAN-SHENG-576692/ad5940_applications/blob/8e6cea71d2572c06becd124dddce352dc61521c2/application/electrochemical/utils/afe_dac_tia/ad5940_electrochemical_utils_afe_dac_tia.c

    /**
     * Enable the high-precision voltage references.
     * Refer to page 25 and Figure 37 (page 87) of the datasheet.
     */
    setting->aferef_cfg.HpBandgapEn = bTRUE;

    /**
     * Recommended high-precision buffer settings.
     * Refer to page 87 of the datasheet for details.
     */
    setting->aferef_cfg.Hp1V1BuffEn   = bTRUE;
    setting->aferef_cfg.Hp1V8BuffEn   = bTRUE;
    setting->aferef_cfg.Disc1V1Cap    = bFALSE;
    setting->aferef_cfg.Disc1V8Cap    = bFALSE;
    setting->aferef_cfg.Hp1V8ThemBuff = bFALSE;
    setting->aferef_cfg.Hp1V8Ilimit   = bFALSE;
    setting->aferef_cfg.Lp1V1BuffEn   = bFALSE;
    setting->aferef_cfg.Lp1V8BuffEn   = bFALSE;

    // LP reference control - turn off them to save power
    setting->aferef_cfg.LpRefBoostEn = bFALSE;

    // @see page 34 of the datasheet, With bias voltage
    if (use_lpdac)
    {
        setting->aferef_cfg.LpBandgapEn = bTRUE;
        setting->aferef_cfg.LpRefBufEn  = bTRUE;
    }
    else
    {
        setting->aferef_cfg.LpBandgapEn = bFALSE;
        setting->aferef_cfg.LpRefBufEn  = bFALSE;
    }

    // ==================================================
    // DSPCfg_Type
    // Refers to
    // https://github.com/XIAN-SHENG-576692/ad5940_applications/blob/8e6cea71d2572c06becd124dddce352dc61521c2/application/electrochemical/utils/dac_tia_adc/ad5940_electrochemical_utils_dac_tia_adc.c
    // Refers to
    // https://github.com/analogdevicesinc/ad5940-examples/blob/master/examples/AD5940_SqrWaveVoltammetry/SqrWaveVoltammetry.c

    if (use_lptia)
    {
        setting->dsp_cfg.ADCBaseCfg.ADCMuxN = ADCMUXN_LPTIA0_N;
        setting->dsp_cfg.ADCBaseCfg.ADCMuxP = ADCMUXP_LPTIA0_P;
    }
    else if (use_hstia)
    {
        setting->dsp_cfg.ADCBaseCfg.ADCMuxN = ADCMUXN_HSTIA_N;
        setting->dsp_cfg.ADCBaseCfg.ADCMuxP = ADCMUXP_HSTIA_P;
    }
    // setting->dsp_cfg.ADCBaseCfg.ADCPga = DEMO_ADCPga;

    // setting->dsp_cfg.ADCFilterCfg.ADCSinc3Osr = DEMO_ADCSinc3Osr;
    // setting->dsp_cfg.ADCFilterCfg.ADCRate = DEMO_ADCRate;
    // setting->dsp_cfg.ADCFilterCfg.BpSinc3 = DEMO_BpSinc3;
    // setting->dsp_cfg.ADCFilterCfg.Sinc2NotchEnable = DEMO_Sinc2NotchEnable;
    // setting->dsp_cfg.ADCFilterCfg.BpNotch = DEMO_BpNotch;
    // setting->dsp_cfg.ADCFilterCfg.ADCSinc2Osr = DEMO_ADCSinc2Osr;
    // setting->dsp_cfg.ADCFilterCfg.ADCAvgNum = DEMO_ADCAvgNum;

    // ==================================================
    // HSLoopCfg_Type
    // Refers to
    // https://github.com/XIAN-SHENG-576692/ad5940_applications/blob/8e6cea71d2572c06becd124dddce352dc61521c2/application/electrochemical/utils/dac_tia_adc/ad5940_electrochemical_utils_dac_tia_adc.c

    if (use_hsdac)
    {
        // setting->hsloop_cfg.HsDacCfg = (HSDACCfg_Type) {
        //     DEMO_
        // };
    }
    else
    {
        setting->hsloop_cfg.HsDacCfg = (HSDACCfg_Type){0};
    }

    if (use_hstia)
    {
        // setting->hsloop_cfg.HsTiaCfg.DiodeClose = DEMO_DiodeClose;
        // setting->hsloop_cfg.HsTiaCfg.ExtRtia = DEMO_ExtRtia;
        // setting->hsloop_cfg.HsTiaCfg.HstiaCtia = DEMO_HstiaCtia;
        // setting->hsloop_cfg.HsTiaCfg.HstiaDeRload = DEMO_HstiaDeRload;
        // setting->hsloop_cfg.HsTiaCfg.HstiaDeRtia = DEMO_HstiaDeRtia;
        // setting->hsloop_cfg.HsTiaCfg.HstiaRtiaSel = DEMO_HstiaRtiaSel;
        // #if defined(CHIPSEL_M355)
        // setting->hsloop_cfg.HsTiaCfg.HstiaDe1Rtia = DEMO_HstiaDe1Rtia;
        // setting->hsloop_cfg.HsTiaCfg.HstiaDe1Rload = DEMO_HstiaDe1Rload;
        // #endif

        // @see page 45 of the datasheet, With bias voltage */
        if (use_lpdac)
        {
            setting->hsloop_cfg.HsTiaCfg.HstiaBias = HSTIABIAS_VZERO0;
        }
        else
        {
            setting->hsloop_cfg.HsTiaCfg.HstiaBias = HSTIABIAS_1P1;
        }

        /**
         * Refers to
         * https://github.com/XIAN-SHENG-576692/ad5940_applications/blob/8e6cea71d2572c06becd124dddce352dc61521c2/application/electrochemical/utils/dac_tia_adc/ad5940_electrochemical_utils_electrode_routing.h
         * @note
         * - For detailed configuration guidance, refer to the datasheet:
         *   - Pages 30–52: Overview of LPLoop and HSLoop configurations
         *   - Pages 74–76: Electrochemical sensor setup and switch matrix
         * configuration
         * - See Figure 36 (Page 76) for the switch matrix routing diagram.
         */
        if (!use_hsdac)
        {
            setting->hsloop_cfg.SWMatCfg.Dswitch = 0;
            setting->hsloop_cfg.SWMatCfg.Nswitch = 0;
            setting->hsloop_cfg.SWMatCfg.Pswitch = 0;
            // setting->hsloop_cfg.SWMatCfg.Tswitch = DEMO_Tswitch;
        }
        else
        {
            // setting->hsloop_cfg.SWMatCfg.Dswitch = DEMO_Dswitch;
            // setting->hsloop_cfg.SWMatCfg.Nswitch = DEMO_Nswitch;
            // setting->hsloop_cfg.SWMatCfg.Pswitch = DEMO_Pswitch;
            // setting->hsloop_cfg.SWMatCfg.Tswitch = DEMO_Tswitch;
        }
    }
    else
    {
        setting->hsloop_cfg.HsTiaCfg = (HSTIACfg_Type){0};
        setting->hsloop_cfg.SWMatCfg = (SWMatrixCfg_Type){0};
    }

    // setting->hsloop_cfg.WgCfg = (WGCfg_Type) {
    //     DEMO_
    // };

    // ==================================================
    // LPLoopCfg_Type
    // Refers to
    // https://github.com/XIAN-SHENG-576692/ad5940_applications/blob/8e6cea71d2572c06becd124dddce352dc61521c2/application/electrochemical/utils/dac_tia_adc/ad5940_electrochemical_utils_dac_tia_adc.c

    if (use_lpdac)
    {
#if defined(CHIPSEL_M355)
// setting->lploop_cfg.LpAmpCfg.LpAmpSel = DEMO_LpAmpSel;
#else
        setting->lploop_cfg.LpAmpCfg.LpAmpSel = LPAMP0;
#endif
        // setting->lploop_cfg.LpAmpCfg.LpAmpPwrMod = DEMO_LpAmpPwrMod;
        setting->lploop_cfg.LpAmpCfg.LpPaPwrEn = bTRUE;
    }
    else
    {
        setting->lploop_cfg.LpAmpCfg = (LPAmpCfg_Type){0};
    }

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
    if (use_lptia && !use_hsdac)
    {
        setting->lploop_cfg.LpAmpCfg.LpTiaPwrEn = bTRUE;
        // setting->lploop_cfg.LpAmpCfg.LpTiaRf = DEMO_LpTiaRf;
        // setting->lploop_cfg.LpAmpCfg.LpTiaRload = DEMO_LpTiaRload;
        // setting->lploop_cfg.LpAmpCfg.LpTiaRtia = DEMO_LpTiaRtia;

        if (setting->lploop_cfg.LpAmpCfg.LpTiaRtia == LPTIARTIA_OPEN)
        {
            setting->lploop_cfg.LpAmpCfg.LpTiaSW =
                0 | LPTIASW(2) | LPTIASW(4) | LPTIASW(5) | LPTIASW(9)
                // | LPTIASW(12)
                // | LPTIASW(13)
                ;
        }
        else
        {
            setting->lploop_cfg.LpAmpCfg.LpTiaSW =
                0 | LPTIASW(2) | LPTIASW(4) | LPTIASW(5)
                // | LPTIASW(12)
                // | LPTIASW(13)
                ;
        }
    }
    else
    {
        setting->lploop_cfg.LpAmpCfg.LpTiaPwrEn = bFALSE;
        setting->lploop_cfg.LpAmpCfg.LpTiaRf    = 0;
        setting->lploop_cfg.LpAmpCfg.LpTiaRload = 0;
        setting->lploop_cfg.LpAmpCfg.LpTiaRtia  = 0;
        setting->lploop_cfg.LpAmpCfg.LpTiaSW =
            0 | LPTIASW(2) |
            LPTIASW(4)

            // When using LPDAC with HATIA to start the electrochemical
            // reaction, close LPTIASW(6) to prevent the signal from being
            // generated on SE0.
            | LPTIASW(6)

            | LPTIASW(7);
    }

    if (use_lpdac)
    {
        setting->lploop_cfg.LpDacCfg.DacData12Bit = 0x00;
        setting->lploop_cfg.LpDacCfg.DacData6Bit  = 0x00;
        setting->lploop_cfg.LpDacCfg.DataRst =
            bFALSE; // Do not reset data register
// setting->lploop_cfg.LpDacCfg.LpDacRef = DEMO_LpDacRef;
#if defined(CHIPSEL_M355)
// setting->lploop_cfg.LpDacCfg.LpdacSel = DEMO_LpdacSel;
#else
        setting->lploop_cfg.LpDacCfg.LpdacSel = LPDAC0;
#endif

        // setting->lploop_cfg.LpDacCfg.LpDacSrc = DEMO_LpDacSrc;
        // Refer to page 39 of the datasheet, LPPA receives feedback to support
        // LPDAC in producing a precise output signal.
        if (use_lptia)
        {
            setting->lploop_cfg.LpDacCfg.LpDacSW =
                0 | LPDACSW_VBIAS2LPPA | LPDACSW_VZERO2LPTIA;
        }
        else if (use_hstia)
        {
            setting->lploop_cfg.LpDacCfg.LpDacSW =
                0 | LPDACSW_VBIAS2LPPA | LPDACSW_VZERO2HSTIA;
        }
        else
        {
            setting->lploop_cfg.LpDacCfg.LpDacSW = 0 | LPDACSW_VBIAS2LPPA;
        }

        setting->lploop_cfg.LpDacCfg.LpDacVbiasMux = LPDACVBIAS_12BIT;
        setting->lploop_cfg.LpDacCfg.LpDacVzeroMux = LPDACVZERO_6BIT;
        setting->lploop_cfg.LpDacCfg.PowerEn       = bTRUE; // Power up
    }
    else
    {
        setting->lploop_cfg.LpDacCfg = (LPDACCfg_Type){0};
    }
}
