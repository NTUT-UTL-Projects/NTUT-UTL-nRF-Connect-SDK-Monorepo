#include "ad5940_wg.h"

AD5940Err AD5940_WGFreq2Word(float SinFreqHz, float WGClock, uint32_t *const Word)
{
    uint32_t __BITWIDTH_WGFCW = 26;
    // if(bIsS2silicon == bTRUE)
    __BITWIDTH_WGFCW = 30;

    if(WGClock == 0) return AD5940ERR_PARA;

    *Word = (uint32_t)(SinFreqHz * (1LL<<__BITWIDTH_WGFCW) / WGClock + 0.5f);
    if(*Word > ((__BITWIDTH_WGFCW == 26)?0xfffff:0xffffff)) return AD5940ERR_PARA;

    return AD5940ERR_OK;
}
