#include "ad5940_struct.h"

// Miss
// - clks_cal.DataType
// - clks_cal.DataCount
// - clks_cal.ADCAvgNum
// - clks_cal.BpNotch
void AD5940_write_ClksCalInfo_Type(
    AD5940_write_ClksCalInfo_Type_Para para,
    ClksCalInfo_Type *const clks_cal
)
{
    clks_cal->ADCAvgNum = para.dsp_cfg.ADCFilterCfg.ADCAvgNum;
    clks_cal->ADCRate = para.dsp_cfg.ADCFilterCfg.ADCRate;
    clks_cal->ADCSinc2Osr = para.dsp_cfg.ADCFilterCfg.ADCSinc2Osr;
    clks_cal->ADCSinc3Osr = para.dsp_cfg.ADCFilterCfg.ADCSinc3Osr;
    clks_cal->BpNotch = para.dsp_cfg.ADCFilterCfg.BpNotch;
    clks_cal->DataCount = para.DataCount;
    clks_cal->DataType = para.DataType;
    clks_cal->DftSrc = para.dsp_cfg.DftCfg.DftSrc;
    clks_cal->RatioSys2AdcClk = para.RatioSys2AdcClk;
}
