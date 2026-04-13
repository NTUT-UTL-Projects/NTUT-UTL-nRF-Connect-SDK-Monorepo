/**
 * TODO
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

// TODO
inline void AD5940_LPDACData12Bit_cal(uint16_t DacData12Bit, LPDACPara_Type LPDACPara)
{
    return;
}
inline void AD5940_LPDACData6Bit_cal(uint16_t DacData6Bit, LPDACPara_Type LPDACPara)
{
    return;
}

inline float AD5940_volts_LPDACData12Bit_cal(float volts, LPDACPara_Type LPDACPara)
{
    return (volts * LPDACPara.kC2V_DAC12B) + LPDACPara.bC2V_DAC12B;
}
inline float AD5940_volts_LPDACData6Bit_cal(float volts, LPDACPara_Type LPDACPara)
{
    return ((volts * LPDACPara.kC2V_DAC6B) + LPDACPara.bC2V_DAC6B);
}

#ifdef __cplusplus
}
#endif
