#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_dpv.h"

#define DataType_methylene_blue DATATYPE_NOTCH
#define FifoSrc_methylene_blue FIFOSRC_SINC2NOTCH

extern const AD5940_DPV_PARAM methylene_blue_electrochemical_para;

void ad5940_controller_const_LPLoopCfg_Type_methylene_blue(
    LPLoopCfg_Type *const cfg
);

void ad5940_controller_const_HSLoopCfg_Type_methylene_blue(
    HSLoopCfg_Type *const cfg
);

void ad5940_controller_const_DSPCfg_Type_methylene_blue(
    DSPCfg_Type *const cfg
);

#ifdef __cplusplus
}
#endif
