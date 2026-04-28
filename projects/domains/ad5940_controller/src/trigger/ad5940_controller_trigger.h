#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_controller_event.h"

    typedef struct
    {
        // event
        AD5940_CONTROLLER_EVENT event;
        // loop
        uint32_t       AfeCtrlSet;
        AFERefCfg_Type aferef_cfg;
        DSPCfg_Type    dsp_cfg;
        LPLoopCfg_Type lploop_cfg;
        HSLoopCfg_Type hsloop_cfg;
        // others
        float    LFOSCClkFreq;
        uint32_t DataType;
        uint32_t FifoSrc;
        float    RatioSys2AdcClk;
        BoolFlag EnterSleepEn;
        // gpio
        AGPIOCfg_Type gpio_cfg;
    } AD5940_CONTROLLER_TRIGGER_PARA;

    AD5940Err
    AD5940_controller_trigger(AD5940_CONTROLLER_TRIGGER_PARA *const para);

#ifdef __cplusplus
}
#endif
