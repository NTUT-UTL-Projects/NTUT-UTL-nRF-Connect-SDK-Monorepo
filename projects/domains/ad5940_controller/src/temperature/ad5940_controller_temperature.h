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
    float t_interval;   /**< Sampling interval in microseconds. */
    uint16_t n_step;
    uint32_t TEMPSENS;  /**< Temperature sensor configuration. Refer to page 57 of the datasheet:
                             ```
                             For improved accuracy, configure the temperature sensor in chop
                             mode via TEMPCON0, Bits[3:1]. If chopping is selected, the user
                             must ensure an even number of ADC conversions take place on the
                             temperature sensor channel and that these results are averaged.
                             ```
                             See TEMPCON0 configuration details on page 61 of the datasheet. */
}
AD5940_CONTROLLER_TEMPERATURE_PARAM;

#ifdef __cplusplus
}
#endif
