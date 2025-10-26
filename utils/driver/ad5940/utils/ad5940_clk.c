#include "ad5940_clk.h"

AD5940Err AD5940_ADCIMPCheckFreq(
    AD5940_ADCIMPCheckFreq_Para para,
    AD5940_ADCIMPCheckFreq_Results *const results
)
{
    results->clk_cfg.ADCClkDiv = ADCCLKDIV_1;

    results->clk_cfg.ADCCLkSrc = ADCCLKSRC_HFOSC;
    results->clk_cfg.SysClkSrc = SYSCLKSRC_HFOSC;

    results->clk_cfg.HFXTALEn = para.HFXTALEn;
    results->clk_cfg.HFOSCEn = bTRUE;
    results->clk_cfg.LFOSCEn = para.LFOSCEn;
    
    results->HanWinEn = bTRUE;

    results->SysClkFreq = 1.6E7;

    /* Step 1: Check Frequency */
    results->freq_params = AD5940_GetFreqParameters(para.freq);

    // AVOIDING INCOHERENCY ERRORS BETWEEN EXCITATION AND MEASUREMENT FREQUENCIES DURING IMPEDANCE MEASUREMENTS
    if(para.freq<80000)
    {
        /* Update HSDAC update rate */
        results->HsDacUpdateRate = 0x1B;
        /*Update ADC rate */
        results->ADCRate = ADCRATE_800KHZ;
        results->AdcClkFreq = 1.6E7;
        /* Change clock to 16MHz oscillator */
        results->freq_params.HighPwrMode = bFALSE;
        results->clk_cfg.HfOSC32MHzMode = bFALSE;
        results->clk_cfg.SysClkDiv = SYSCLKDIV_1;
        results->AfePwr = AFEPWR_LP;
        results->AfeBw = AFEBW_100KHZ;
    }
    /* High power mode */
    if(para.freq >= 80000)
    {
        /* Update HSDAC update rate */
        results->HsDacUpdateRate = 0x07;
        /*Update ADC rate */
        results->ADCRate = ADCRATE_1P6MHZ;
        results->AdcClkFreq = 3.2E7;
        /* Change clock to 32MHz oscillator */
        results->freq_params.HighPwrMode = bTRUE;
        results->clk_cfg.HfOSC32MHzMode = bTRUE;
        results->clk_cfg.SysClkDiv = SYSCLKDIV_2;
        results->AfePwr = AFEPWR_HP;
        results->AfeBw = AFEBW_250KHZ;
    }

    results->RatioSys2AdcClk = results->SysClkFreq / results->AdcClkFreq;

    return AD5940ERR_OK;
}
