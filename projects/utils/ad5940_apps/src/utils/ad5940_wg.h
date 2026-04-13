#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

/**
   @brief Calculate sine wave generator frequency word. The maxim frequency is 250kHz-1LSB
   @note See Register—WGFCW, p. 102 of the datasheet. 
   @param SinFreqHz : Target frequency in Hz unit.
   @param WGClock: Waveform generator clock frequency in Hz unit. The clock is sourced from system clock, default value is 16MHz HFOSC.
   @return return none.
*/
AD5940Err AD5940_WGFreq2Word(float SinFreqHz, float WGClock, uint32_t *const Word);

#ifdef __cplusplus
}
#endif
