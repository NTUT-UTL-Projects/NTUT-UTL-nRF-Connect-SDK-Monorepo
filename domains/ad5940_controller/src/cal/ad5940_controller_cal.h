#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_controller_event.h"

typedef struct {
    float ADCRefVolt;
    BoolFlag bWithCtia;
    AD5940_CONTROLLER_EVENT event;
    float fRcal;
    AD5940_ELEC_PARAM param;
    float VRef1p11;
    float VRef1p82;
} AD5940_CONTROLLER_CAL_PARA;

typedef struct {
    AFERefCfg_Type *const aferef_cfg;
    DSPCfg_Type *const dsp_cfg;
    HSLoopCfg_Type *const hsloop_cfg;
    LPLoopCfg_Type *const lploop_cfg;
} AD5940_CONTROLLER_CAL_WRITE;

typedef struct {
    uint32_t ADCRate;
    fImpPol_Type HsRtiaCal;
    float LFOSCClkFreq;
    LPDACPara_Type LpDacPara;
    fImpPol_Type LpRtiaCal;
    float RatioSys2AdcClk;
} AD5940_CONTROLLER_CAL_RESULTS;

AD5940Err AD5940_controller_cal(
    const AD5940_CONTROLLER_CAL_PARA para,
    const AD5940_CONTROLLER_CAL_WRITE write,
    AD5940_CONTROLLER_CAL_RESULTS *const results
);

#ifdef __cplusplus
}
#endif
