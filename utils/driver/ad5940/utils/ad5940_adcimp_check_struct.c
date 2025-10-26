#include "ad5940_adcimp_check_struct.h"

void AD5940_write_DSPCfg_Type(
    AD5940_ADCIMPCheckFreq_Results freq_results,
    DSPCfg_Type *const dsp_cfg
)
{
    // freq_results.freq_params.HighPwrMode;
    dsp_cfg->DftCfg.DftNum = freq_results.freq_params.DftNum;
    dsp_cfg->DftCfg.DftSrc = freq_results.freq_params.DftSrc;
    dsp_cfg->ADCFilterCfg.ADCSinc3Osr = freq_results.freq_params.ADCSinc3Osr;
    dsp_cfg->ADCFilterCfg.ADCSinc2Osr = freq_results.freq_params.ADCSinc2Osr;
    // freq_results.freq_params.NumClks;

    // freq_results.clk_cfg;

    dsp_cfg->DftCfg.HanWinEn = freq_results.HanWinEn;
    // freq_results.AfePwr;
    // freq_results.AfeBw;
    // freq_results.HsDacUpdateRate;
    dsp_cfg->ADCFilterCfg.ADCRate = freq_results.ADCRate;
    // freq_results.AdcClkFreq;
    // freq_results.SysClkFreq;
    // freq_results.RatioSys2AdcClk;
}

// Only
// - hsdac_cfg->HsDacUpdateRate
void AD5940_write_HSDACCfg_Type(
    AD5940_ADCIMPCheckFreq_Results freq_results,
    HSDACCfg_Type *const hsdac_cfg
)
{
    // freq_results.freq_params.HighPwrMode;
    // freq_results.freq_params.DftNum;
    // freq_results.freq_params.DftSrc;
    // freq_results.freq_params.ADCSinc3Osr;
    // freq_results.freq_params.ADCSinc2Osr;
    // freq_results.freq_params.NumClks;

    // freq_results.clk_cfg;

    // freq_results.HanWinEn;
    // freq_results.AfePwr;
    // freq_results.AfeBw;
    hsdac_cfg->HsDacUpdateRate = freq_results.HsDacUpdateRate;
    // freq_results.ADCRate;
    // freq_results.AdcClkFreq;
    // freq_results.SysClkFreq;
    // freq_results.RatioSys2AdcClk;
}
