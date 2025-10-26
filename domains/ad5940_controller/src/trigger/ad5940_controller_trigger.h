#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_controller_event.h"

typedef struct {
    AD5940_CONTROLLER_EVENT event;
    AD5940_ELEC_PARAM param;
    AFERefCfg_Type aferef_cfg;
    LPLoopCfg_Type lploop_cfg;
    HSLoopCfg_Type hsloop_cfg;
    DSPCfg_Type dsp_cfg;
    uint32_t AfeCtrlSet;
    float LFOSCClkFreq;
    uint32_t DataType;
    float RatioSys2AdcClk;
    AGPIOCfg_Type gpio_cfg;
    uint32_t FifoSrc;
} AD5940_CONTROLLER_TRIGGER_PARA;

AD5940Err AD5940_controller_trigger(
    AD5940_CONTROLLER_TRIGGER_PARA para
);

#ifdef __cplusplus
}
#endif
