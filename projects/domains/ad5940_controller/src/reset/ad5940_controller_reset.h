#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

typedef enum {
    AD5940_CONTROLLER_RESET_OPTION_HAREWARE,
    AD5940_CONTROLLER_RESET_OPTION_SOFEWARE,
} AD5940_CONTROLLER_RESET_OPTION;

AD5940Err AD5940_controller_reset(
    uint32_t *const SEQGenBuff, 
    const uint16_t SEQGenBuffLen,
    const AD5940_CONTROLLER_RESET_OPTION reset_option
);

#ifdef __cplusplus
}
#endif
