#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "ad5940.h"

typedef struct Setting
{
    uint32_t AfeCtrlSet;
    AFERefCfg_Type aferef_cfg;
    DSPCfg_Type dsp_cfg;
    HSLoopCfg_Type hsloop_cfg;
    LPLoopCfg_Type lploop_cfg;
} Setting;

void setting_electrochemical(Setting *const setting, BoolFlag use_lpdac, BoolFlag use_lptia, BoolFlag use_hsdac, BoolFlag use_hstia);

void setting_temperature(Setting *const setting);

#ifdef __cplusplus
}
#endif
