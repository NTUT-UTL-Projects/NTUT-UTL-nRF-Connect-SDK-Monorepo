#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_clk.h"

typedef struct
{
    uint32_t DataType;
    uint32_t DataCount;
    DSPCfg_Type dsp_cfg;
    float RatioSys2AdcClk;
} AD5940_write_ClksCalInfo_Type_Para;

void AD5940_write_ClksCalInfo_Type(
    AD5940_write_ClksCalInfo_Type_Para para,
    ClksCalInfo_Type *const clks_cal
);

#ifdef __cplusplus
}
#endif
