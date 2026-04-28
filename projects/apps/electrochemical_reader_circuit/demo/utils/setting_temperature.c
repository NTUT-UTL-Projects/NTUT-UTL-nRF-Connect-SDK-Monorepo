#include "setting.h"

// Refers to
// https://github.com/analogdevicesinc/ad5940-examples/blob/master/examples/AD5940_Temperature/AD5940_Temperature.c
void setting_temperature(Setting *const setting)
{
    setting->AfeCtrlSet =
        0 | AFECTRL_TEMPSPWR /* Turn on temperature sensor power */
        ;

    setting->aferef_cfg.HpBandgapEn = bTRUE;
    setting->aferef_cfg.Hp1V1BuffEn = bTRUE;
    setting->aferef_cfg.Hp1V8BuffEn =
        bTRUE; /* The High speed buffers are automatically turned off during
                  hibernate */
    setting->aferef_cfg.Disc1V1Cap    = bFALSE;
    setting->aferef_cfg.Disc1V8Cap    = bFALSE;
    setting->aferef_cfg.Hp1V8ThemBuff = bFALSE;
    setting->aferef_cfg.Hp1V8Ilimit   = bFALSE;
    setting->aferef_cfg.Lp1V1BuffEn   = bFALSE;
    setting->aferef_cfg.Lp1V8BuffEn   = bFALSE;
    /* LP reference control - turn off them to save power*/
    setting->aferef_cfg.LpBandgapEn  = bFALSE;
    setting->aferef_cfg.LpRefBufEn   = bFALSE;
    setting->aferef_cfg.LpRefBoostEn = bFALSE;

    /* Initialize ADC basic function */
    setting->dsp_cfg.ADCBaseCfg.ADCMuxP = ADCMUXP_TEMPP;
    setting->dsp_cfg.ADCBaseCfg.ADCMuxN = ADCMUXN_TEMPN;
    // setting->dsp_cfg.ADCBaseCfg.ADCPga = DEMO_ADCPga;

    /* Initialize ADC filters */
    // setting->dsp_cfg.ADCFilterCfg.ADCAvgNum = DEMO_ADCAvgNum;
    // setting->dsp_cfg.ADCFilterCfg.ADCRate = DEMO_ADCRate;
    // setting->dsp_cfg.ADCFilterCfg.ADCSinc2Osr = DEMO_ADCSinc2Osr;
    // setting->dsp_cfg.ADCFilterCfg.ADCSinc3Osr = DEMO_ADCSinc3Osr;
    // setting->dsp_cfg.ADCFilterCfg.BpNotch = DEMO_BpNotch;
    // setting->dsp_cfg.ADCFilterCfg.BpSinc3 = DEMO_BpSinc3;
    // setting->dsp_cfg.ADCFilterCfg.Sinc2NotchEnable = DEMO_Sinc2NotchEnable;

    // Turn off DAC, TIA
    setting->hsloop_cfg = (HSLoopCfg_Type){0};
    setting->lploop_cfg = (LPLoopCfg_Type){0};

    return;
}
