#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "electrochemical_dev_kit.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct 
{
    ELECTROCHEMICAL_DEV_KIT_ENERGY_TYPE e_dc;
    ELECTROCHEMICAL_DEV_KIT_TIME_TYPE t_interval;
    ELECTROCHEMICAL_DEV_KIT_NUMBER_TYPE n_step;
} AD5940_CONTROLLER_ELECTROCHEMICAL_CA_PARAM;

typedef struct 
{
    ELECTROCHEMICAL_DEV_KIT_cv_params cv;
    ELECTROCHEMICAL_DEV_KIT_NUMBER_TYPE number_of_scans;
} AD5940_CONTROLLER_ELECTROCHEMICAL_CV_PARAM;

typedef ELECTROCHEMICAL_DEV_KIT_dpv_params AD5940_CONTROLLER_ELECTROCHEMICAL_DPV_PARAM;

#ifdef __cplusplus
}
#endif
