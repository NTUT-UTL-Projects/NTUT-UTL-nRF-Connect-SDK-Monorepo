#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

int valve_init(void);

#define VALVE_DIRECTION_120 1
#define VALVE_DIRECTION_240 2
#define VALVE_DIRECTION_360 3
void valve_turn(const uint8_t direction);

#ifdef __cplusplus
}
#endif
