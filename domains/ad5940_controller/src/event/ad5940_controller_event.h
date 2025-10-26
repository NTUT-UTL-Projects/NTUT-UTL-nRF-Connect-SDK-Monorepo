#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_apps.h"

typedef enum {
    AD5940_CONTROLLER_EVENT_CA,
    AD5940_CONTROLLER_EVENT_CV,
    AD5940_CONTROLLER_EVENT_DPV,
    AD5940_CONTROLLER_EVENT_EIS,
} AD5940_CONTROLLER_EVENT;

typedef union
{
    AD5940_DPV_PARAM dpv;
    AD5940_EIS_PARAM eis;
} AD5940_ELEC_PARAM;

#ifdef __cplusplus
}
#endif
