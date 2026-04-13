#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

extern const float AD5940_EXCITBUFGAIN_to_inampgnmde[];

extern const float AD5940_HSDACGAIN_to_attenen[];

inline BoolFlag AD5940_HSDACDAT_is_vaild(uint32_t HSDACDAT) 
{
    return (HSDACDAT >= 0x200) && (HSDACDAT <= 0xE00);
}

inline uint32_t AD5940_volts_to_HSDACDAT(float volts, float inampgnmde, float attenen)
{
    return volts * 2048.0 / inampgnmde / attenen / (404.4 / 1e3) + 2048.0;
}

inline float AD5940_HSDACDAT_to_volts(uint32_t HSDACDAT, float inampgnmde, float attenen)
{
    return (HSDACDAT - 2048.0) * inampgnmde * attenen * (404.4 / 1e3) / 2048.0;
}

#ifdef __cplusplus
}
#endif
