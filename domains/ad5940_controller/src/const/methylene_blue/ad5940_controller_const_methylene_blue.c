#include "ad5940_controller_const_methylene_blue.h"

const AD5940_DPV_PARAM methylene_blue_electrochemical_para = {
    .e_begin = -0.6f,
    .e_end = 0.1f,
    .e_pulse = 0.05f,
    .e_step = 0.01f,
    .inversion = AD5940_DPV_INVERSION_NONE,
    .scan_rate = 0.05f,
    .t_pulse = 0.05f,
};

// Set
// - cfg->HsTiaCfg.LpAmpPwrMod
// - cfg->LpAmpCfg.LpTiaRf
// - cfg->LpAmpCfg.LpTiaRload
// - cfg->LpAmpCfg.LpTiaRtia
void ad5940_controller_const_LPLoopCfg_Type_methylene_blue(
    LPLoopCfg_Type *const cfg
)
{
    cfg->LpAmpCfg.LpAmpPwrMod = LPAMPPWR_NORM;
    // cfg->LpAmpCfg.LpAmpSel;
    // cfg->LpAmpCfg.LpPaPwrEn;
    // cfg->LpAmpCfg.LpTiaPwrEn;
    cfg->LpAmpCfg.LpTiaRf = LPTIARF_SHORT;
    cfg->LpAmpCfg.LpTiaRload = LPTIARLOAD_SHORT;
    cfg->LpAmpCfg.LpTiaRtia = LPTIARTIA_10K;
    // cfg->LpAmpCfg.LpTiaSW;

    // cfg->LpDacCfg;
}

// Set
// - cfg->HsTiaCfg.HstiaCtia
void ad5940_controller_const_HSLoopCfg_Type_methylene_blue(
    HSLoopCfg_Type *const cfg
)
{
    /**
     * Refer to page 52 of the datasheet.
     * 31pF + 2pF
     */
    cfg->HsTiaCfg.HstiaCtia = 31;
}

// Unset
// - cfg->ADCBaseCfg.ADCMuxN
// - cfg->ADCBaseCfg.ADCMuxP
// - cfg->ADCFilterCfg.ADCRate
void ad5940_controller_const_DSPCfg_Type_methylene_blue(
    DSPCfg_Type *const cfg
)
{
    // cfg->ADCBaseCfg.ADCMuxN;
    // cfg->ADCBaseCfg.ADCMuxP;
    cfg->ADCBaseCfg.ADCPga = ADCPGA_1;
    
    cfg->ADCDigCompCfg = (ADCDigComp_Type) {0};

    cfg->ADCFilterCfg.ADCAvgNum = ADCAVGNUM_16;
    // cfg->ADCFilterCfg.ADCRate;
    /**
     * Refer to page 57 of the datasheet for more details.
     */
    cfg->ADCFilterCfg.ADCSinc2Osr = ADCSINC2OSR_22;
    /**
     * Refer to page 57 of the datasheet.
     * ```
     * It is recommended to use a decimation rate of 4.
     * ```
     */
    cfg->ADCFilterCfg.ADCSinc3Osr = ADCSINC3OSR_4;
    cfg->ADCFilterCfg.BpNotch = bTRUE;
    cfg->ADCFilterCfg.BpSinc3 = bTRUE;
    cfg->ADCFilterCfg.DFTClkEnable = bFALSE;
    cfg->ADCFilterCfg.Sinc2NotchClkEnable = bFALSE;
    cfg->ADCFilterCfg.Sinc2NotchEnable = bTRUE;
    cfg->ADCFilterCfg.Sinc3ClkEnable = bFALSE;
    cfg->ADCFilterCfg.WGClkEnable = bFALSE;

    cfg->DftCfg.DftNum = DFTNUM_16;
    cfg->DftCfg.DftSrc = DFTSRC_SINC2NOTCH;
    cfg->DftCfg.HanWinEn = bTRUE;

    cfg->StatCfg = (StatCfg_Type) {0};
}
