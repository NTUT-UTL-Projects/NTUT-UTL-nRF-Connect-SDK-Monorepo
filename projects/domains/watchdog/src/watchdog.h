#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

int watchdog0_init(uint32_t window_min, uint32_t window_max);

int watchdog0_feed(void);

#ifdef __cplusplus
}
#endif
