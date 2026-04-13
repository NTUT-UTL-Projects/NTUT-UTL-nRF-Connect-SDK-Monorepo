#include "ad5940_cal_struct.h"

void AD5940_write_ADCPGACal_Type(
    AD5940_write_ADCPGACal_Type_Para para,
    ADCPGACal_Type *const cal
)
{
    cal->AdcClkFreq = para.freq_results.AdcClkFreq;
    cal->ADCPga = para.ADCBaseCfg.ADCPga;
    cal->ADCSinc2Osr = para.freq_results.freq_params.ADCSinc2Osr;
    cal->ADCSinc3Osr = para.freq_results.freq_params.ADCSinc3Osr;
    cal->PGACalType = para.PGACalType;
    cal->SysClkFreq = para.freq_results.SysClkFreq;
    cal->TimeOut10us = para.TimeOut10us;
    cal->VRef1p11 = para.VRef1p11;
    cal->VRef1p82 = para.VRef1p82;
}

void AD5940_write_LPTIAOffsetCal_Type(
    AD5940_write_LPTIAOffsetCal_Type_Para para,
    LPTIAOffsetCal_Type *const cal
)
{
    cal->AdcClkFreq = para.freq_results.AdcClkFreq;
    cal->ADCPga = para.ADCBaseCfg.ADCPga;
    cal->ADCSinc2Osr = para.freq_results.freq_params.ADCSinc2Osr;
    cal->ADCSinc3Osr = para.freq_results.freq_params.ADCSinc3Osr;
    cal->DacData12Bit = para.lploop_cfg.LpDacCfg.DacData12Bit;
    cal->DacData6Bit = para.lploop_cfg.LpDacCfg.DacData6Bit;
    cal->LpAmpPwrMod = para.lploop_cfg.LpAmpCfg.LpAmpPwrMod;
    cal->LpAmpSel = para.lploop_cfg.LpAmpCfg.LpAmpSel;
    cal->LpDacVzeroMux = para.lploop_cfg.LpDacCfg.LpDacVzeroMux;
    cal->LpTiaRtia = para.lploop_cfg.LpAmpCfg.LpTiaRtia;
    cal->LpTiaSW = para.lploop_cfg.LpAmpCfg.LpTiaSW;
    cal->SettleTime10us = para.SettleTime10us;
    cal->SysClkFreq = para.freq_results.SysClkFreq;
    cal->TimeOut10us = para.TimeOut10us;
}

void AD5940_write_LPRTIACal_Type(
    AD5940_write_LPRTIACal_Type_Para para,
    LPRTIACal_Type *const cal
)
{
    cal->AdcClkFreq = para.freq_results.AdcClkFreq;
    cal->ADCSinc2Osr = para.freq_results.freq_params.ADCSinc2Osr;
    cal->ADCSinc3Osr = para.freq_results.freq_params.ADCSinc3Osr;
    cal->bPolarResult = para.bPolarResult;
    cal->bWithCtia = para.bWithCtia;
    cal->DftCfg = para.DftCfg;
    cal->fFreq = para.fFreq;
    cal->fRcal = para.fRcal;
    cal->LpAmpPwrMod = para.LpAmpCfg.LpAmpPwrMod;
    cal->LpAmpSel = para.LpAmpCfg.LpAmpSel;
    cal->LpTiaRtia = para.LpAmpCfg.LpTiaRtia;
    cal->SysClkFreq = para.freq_results.SysClkFreq;
}

void AD5940_write_HSRTIACal_Type(
    AD5940_write_HSRTIACal_Type_Para para,
    HSRTIACal_Type *const cal
)
{
    cal->AdcClkFreq = para.freq_results.AdcClkFreq;
    cal->ADCSinc2Osr = para.freq_results.freq_params.ADCSinc2Osr;
    cal->ADCSinc3Osr = para.freq_results.freq_params.ADCSinc3Osr;
    cal->bPolarResult = para.bPolarResult;
    cal->DftCfg = para.DftCfg;
    cal->fFreq = para.fFreq;
    cal->fRcal = para.fRcal;
    cal->HsTiaCfg = para.HsTiaCfg;
    cal->SysClkFreq = para.freq_results.SysClkFreq;
}

void AD5940_write_LPDACCal_Type(
    AD5940_write_LPDACCal_Type_Para para,
    LPDACCal_Type *const cal
)
{
    cal->AdcClkFreq = para.freq_results.AdcClkFreq;
    cal->ADCRefVolt = para.ADCRefVolt;
    cal->ADCSinc2Osr = para.freq_results.freq_params.ADCSinc2Osr;
    cal->ADCSinc3Osr = para.freq_results.freq_params.ADCSinc3Osr;
    cal->LpdacSel = para.LpDacCfg.LpdacSel;
    cal->SettleTime10us = para.SettleTime10us;
    cal->SysClkFreq = para.freq_results.SysClkFreq;
    cal->TimeOut10us = para.TimeOut10us;
}
