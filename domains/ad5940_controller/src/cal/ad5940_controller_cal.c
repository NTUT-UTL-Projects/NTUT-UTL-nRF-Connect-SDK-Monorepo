#include "ad5940_controller_cal.h"
#include "ad5940_utils.h"

#define _SamplePeriod 3

#define _SettleTime10us 1E3
#define _TimeOut10us 1E3

#define _bPolarResult bTRUE

AD5940Err AD5940_controller_cal(
    const AD5940_CONTROLLER_CAL_PARA para,
    const AD5940_CONTROLLER_CAL_WRITE write,
    AD5940_CONTROLLER_CAL_RESULTS *const results
)
{
    AD5940Err err = AD5940ERR_OK;

	// ========================================
    /* Wakeup AFE by read register, read 10 times at most */
    if(AD5940_WakeUp(10) > 10) return AD5940ERR_WAKEUP;  /* Wakeup Failed */

    AD5940_ADCIMPCheckFreq_Results adc_imp_check_freq_results = {};

	// ========================================
    // Clock
    {
        // ========================================
        // ========================================
        // TODO
        // NOTE:
        // This block only for the testing.
        // This is the new why to config the clock, but it cause gpio do not work well.
        // This code is referenced from https://github.com/analogdevicesinc/ad5940-examples/blob/master/examples/AD5940_Impedance_Adjustable_with_frequency/Impedance.c

        // AD5940_ADCIMPCheckFreq_Para p = {
        //     .HFXTALEn = bFALSE, // NTUT-UTL didn't need external 
        //     .LFOSCEn = bTRUE,   // For use with WUPT (and optionally with DAC, TIA, or ADC).
        // };
        
        // /**
        //  * Refer to page 55 of the datasheet.
        //  * High power mode is only necessary for impedance measurements when the frequency exceeds 80 kHz.
        //  */
        // switch (para.event)
        // {
        // case AD5940_CONTROLLER_EVENT_CA:
        // case AD5940_CONTROLLER_EVENT_CV:
        // case AD5940_CONTROLLER_EVENT_DPV:
        //     p.freq = 0.0f;
        //     break;
        // case AD5940_CONTROLLER_EVENT_EIS:
        //     p.freq = para.param.eis.freq;
        //     break;
        // default:
        //     return AD5940ERR_PARA;
        // }

        // err = AD5940_ADCIMPCheckFreq(
        //     p,
        //     &adc_imp_check_freq_results
        // );
        // if(err != AD5940ERR_OK) return err;

        // AD5940_WriteReg(REG_AFECON_CLKSEL, 0x0000);
        // AD5940_AFEPwrBW(adc_imp_check_freq_results.AfePwr, adc_imp_check_freq_results.AfeBw);
        // AD5940_CLKCfg(&adc_imp_check_freq_results.clk_cfg);







        // ========================================
        // ========================================
        // TODO
        // NOTE:
        // This block only for the testing.
        // This is the old why to config the clock, but it work well.
        // This code is referenced from https://github.com/analogdevicesinc/ad5940-examples/blob/master/examples/AD5940_ChronoAmperometric/ChronoAmperometric.c

        AD5940_WriteReg(REG_AFECON_CLKSEL, 0x00);

        uint32_t AFEPWR_Const = AFEPWR_LP;
        switch (AFEPWR_Const)
        {
        case AFEPWR_HP:
            AD5940_HPModeEn(bTRUE);
            adc_imp_check_freq_results.ADCRate = ADCRATE_1P6MHZ;
            adc_imp_check_freq_results.AdcClkFreq = 3.2E7F;
            adc_imp_check_freq_results.SysClkFreq = 1.6E7F;
            break;

        case AFEPWR_LP:
            AD5940_HPModeEn(bFALSE);
            adc_imp_check_freq_results.ADCRate = ADCRATE_800KHZ;
            adc_imp_check_freq_results.AdcClkFreq = 1.6E7F;
            adc_imp_check_freq_results.SysClkFreq = 1.6E7F;
            break;

        default:
            return AD5940ERR_PARA;
        }

        adc_imp_check_freq_results.RatioSys2AdcClk = adc_imp_check_freq_results.SysClkFreq / adc_imp_check_freq_results.AdcClkFreq;

        adc_imp_check_freq_results.freq_params.DftNum = DFTNUM_16;
        adc_imp_check_freq_results.freq_params.DftSrc = DFTSRC_SINC2NOTCH;
        adc_imp_check_freq_results.freq_params.ADCSinc3Osr = ADCSINC3OSR_4;
        adc_imp_check_freq_results.freq_params.ADCSinc2Osr = ADCSINC2OSR_22;
        adc_imp_check_freq_results.freq_params.HighPwrMode = bTRUE;
        adc_imp_check_freq_results.freq_params.NumClks = 0;
        adc_imp_check_freq_results.HanWinEn = bTRUE;
        adc_imp_check_freq_results.HsDacUpdateRate = 255;
        // ========================================
        // ========================================







        // Calibrate LFOSC
        LFOSCMeasure_Type LFOSC_measure = {
            .CalDuration = 1000,
            .CalSeqAddr = 0x00000000,
            .SystemClkFreq = adc_imp_check_freq_results.SysClkFreq,
        };
        err = AD5940_LFOSCMeasure(
            &LFOSC_measure,
            &results->LFOSCClkFreq
        );
        if(err != AD5940ERR_OK) return err;
    }

	// ========================================
    // Set Config
    AD5940_write_DSPCfg_Type(
        adc_imp_check_freq_results,
        write.dsp_cfg
    );

    AD5940_write_HSDACCfg_Type(
        adc_imp_check_freq_results,
        &(write.hsloop_cfg->HsDacCfg)
    );

    results->ADCRate = adc_imp_check_freq_results.ADCRate;
    results->RatioSys2AdcClk = adc_imp_check_freq_results.RatioSys2AdcClk;

	// ========================================
    // Calibration
    // TODO
    {
        float fFreq;
        err = AD5940_fFreqCal(
            (AD5940_fFreqCal_Para) {
                .results = adc_imp_check_freq_results,
                .SamplePeriod = _SamplePeriod,
            },
            &fFreq
        );
        if(err != AD5940ERR_OK) return err;

        // // cal ADC PGA
        // {
        //     ADCPGACal_Type ADCPGACal;
        //     AD5940_write_ADCPGACal_Type(
        //         (AD5940_write_ADCPGACal_Type_Para) {
        //             .ADCBaseCfg = write.dsp_cfg->ADCBaseCfg,
        //             .freq_results = adc_imp_check_freq_results,
        //             .PGACalType = PGACALTYPE_OFFSETGAIN,
        //             .TimeOut10us = _TimeOut10us,
        //             .VRef1p11 = para.VRef1p11,
        //             .VRef1p82 = para.VRef1p82,
        //         },
        //         &ADCPGACal
        //     );
        //     err = AD5940_ADCPGACal(&ADCPGACal);
        //     if(err != AD5940ERR_OK) return err;
        // }

        // // cal LPTIA
        // {
        //     LPTIAOffsetCal_Type LPTIAOffsetCal;
        //     AD5940_write_LPTIAOffsetCal_Type(
        //         (AD5940_write_LPTIAOffsetCal_Type_Para) {
        //             .ADCBaseCfg = write.dsp_cfg->ADCBaseCfg,
        //             .DacData12Bit = 0x00,
        //             .DacData6Bit = 0x00,
        //             .freq_results = adc_imp_check_freq_results,
        //             .lploop_cfg = *(write.lploop_cfg),
        //             .SettleTime10us = _SettleTime10us,
        //             .TimeOut10us = _TimeOut10us,
        //         },
        //         &LPTIAOffsetCal
        //     );
        //     err = AD5940_LPTIAOffsetCal(&LPTIAOffsetCal);
        //     if(err != AD5940ERR_OK) return err;
        // }

        {
            LPRTIACal_Type LPRTIACal;
            AD5940_write_LPRTIACal_Type(
                (AD5940_write_LPRTIACal_Type_Para) {
                    .bPolarResult = _bPolarResult,
                    .bWithCtia = para.bWithCtia,
                    .DftCfg = write.dsp_cfg->DftCfg,
                    .fFreq = fFreq,
                    .fRcal = para.fRcal,
                    .freq_results = adc_imp_check_freq_results,
                    .LpAmpCfg = write.lploop_cfg->LpAmpCfg,
                },
                &LPRTIACal
            );
            err = AD5940_LPRtiaCal(
                &LPRTIACal,
                &results->LpRtiaCal
            );
            if(err != AD5940ERR_OK) return err;
        }

        // {
        //     HSRTIACal_Type HSRTIACal;
        //     AD5940_write_HSRTIACal_Type(
        //         (AD5940_write_HSRTIACal_Type_Para) {
        //             .bPolarResult = _bPolarResult,
        //             .bWithCtia = para.bWithCtia,
        //             .DftCfg = write.dsp_cfg->DftCfg,
        //             .fFreq = fFreq,
        //             .fRcal = para.fRcal,
        //             .freq_results = adc_imp_check_freq_results,
        //             .HsTiaCfg = write.hsloop_cfg->HsTiaCfg,
        //         },
        //         &HSRTIACal
        //     );
        //     err = AD5940_HSRtiaCal(
        //         &HSRTIACal, 
        //         &results->HsRtiaCal
        //     );
        //     if(err != AD5940ERR_OK) return err;
        // }

        // {
        //     LPDACCal_Type LPDACCal;
        //     AD5940_write_LPDACCal_Type(
        //         (AD5940_write_LPDACCal_Type_Para) {
        //             .ADCRefVolt = para.ADCRefVolt,
        //             .freq_results = adc_imp_check_freq_results,
        //             .LpDacCfg = write.lploop_cfg->LpDacCfg,
        //             .SettleTime10us = _SettleTime10us,
        //             .TimeOut10us = _TimeOut10us,
        //         },
        //         &LPDACCal
        //     );
        //     err = AD5940_LPDACCal(
        //         &LPDACCal,
        //         &results->LpDacPara
        //     );
        //     if(err != AD5940ERR_OK) return err;
        // }
    }

    return AD5940ERR_OK;
}
