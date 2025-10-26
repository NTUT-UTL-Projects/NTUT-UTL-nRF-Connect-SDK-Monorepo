#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

/**
 * @brief Parameters for the AD5940 Electrochemical Impedance Spectroscopy (EIS) operation.
 * 
 * TODO
 */
typedef struct 
{
    float freq;
}
AD5940_EIS_PARAM;

#ifdef __cplusplus
}
#endif
