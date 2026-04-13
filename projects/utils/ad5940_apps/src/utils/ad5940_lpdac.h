/**
 * Configures the voltage between V_bias and V_zero.
 * 
 * @note
 * Refer to page 30 of the datasheet for detailed information on setting the voltage
 * between V_bias and V_zero. The default MUX settings used in this file are:
 * - `AD5940_LPDACVBIAS_12BIT` for V_bias (higher precision: 12 bits)
 * - `AD5940_LPDACVZERO_6BIT` for V_zero (lower precision: 6 bits)
 * 
 * Since V_bias has higher precision than V_zero, the desired output voltage (V_out)
 * should be set as follows:
 * 1. Configure V_zero using the maximum voltage (V_max).
 * 2. Adjust V_bias to fine-tune the precision of V_out.
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

/** 
 * Default LPDAC resolution(2.5V internal reference).
 * @see page 30 of the datasheet.
*/
#define DAC12BITVOLT_1LSB   (2.2 / 4095.0)              // V
#define DAC6BITVOLT_1LSB    (64.0 * DAC12BITVOLT_1LSB)  // V

inline BoolFlag AD5940_DacDAta12Bit_is_vaild(uint16_t DacData12Bit)
{
    return DacData12Bit <= 0xFFF;
}
inline BoolFlag AD5940_DacDAta6Bit_is_vaild(uint16_t DacData6Bit)
{
    return DacData6Bit <= 0x3F;
}

inline BoolFlag AD5940_LPDACData12Bit_is_greater(uint16_t DacData12Bit, uint16_t DacData6Bit)
{
    return DacData12Bit > (DacData6Bit * 64.0);
}

inline uint16_t AD5940_volts_to_LPDACData6Bit(float volts)
{
    return volts / DAC6BITVOLT_1LSB;
}
inline uint16_t AD5940_volts_to_LPDACData12Bit(float volts, uint16_t DacData6Bit)
{
    float LPDACData12Bit = volts / DAC12BITVOLT_1LSB;
    return LPDACData12Bit + (AD5940_LPDACData12Bit_is_greater(LPDACData12Bit, DacData6Bit) ? 1 : 0);
}

inline uint32_t AD5940_LPDACDataBits_to_LPDACDAT(uint16_t DacData12Bit, uint16_t DacData6Bit)
{
    return ((uint32_t) (DacData6Bit)) << 12 | ((uint32_t) (DacData12Bit));
}

inline uint16_t AD5940_LPDACDAT_to_DacData12Bit(uint32_t LPDACDAT)
{
    return LPDACDAT & 0xFFF;
}
inline uint16_t AD5940_LPDACDAT_to_DacData6Bit(uint32_t LPDACDAT)
{
    return LPDACDAT >> 12;
}

inline float AD5940_DacData12Bit_to_volts(uint16_t DacData12Bit, BoolFlag LPDACData12Bit_is_greater)
{
    return DacData12Bit * DAC12BITVOLT_1LSB + (LPDACData12Bit_is_greater ? -1 : 0);
}
inline float AD5940_DacData6Bit_to_volts(uint16_t DacData6Bit)
{
    return DacData6Bit * DAC6BITVOLT_1LSB;
}

#undef DAC12BITVOLT_1LSB
#undef DAC6BITVOLT_1LSB

#ifdef __cplusplus
}
#endif
