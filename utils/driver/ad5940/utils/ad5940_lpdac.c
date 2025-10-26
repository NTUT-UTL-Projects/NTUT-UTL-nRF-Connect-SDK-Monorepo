#include "ad5940_lpdac.h"

#include <stdlib.h>

/** 
 * Default LPDAC resolution(2.5V internal reference).
 * @see page 30 of the datasheet.
*/
#define DAC12BITVOLT_1LSB   (2.2 / 4095.0)              // V
#define DAC6BITVOLT_1LSB    (64.0 * DAC12BITVOLT_1LSB)  // V

AD5940Err AD5940_LPDACData12BitIsGreater(
    uint16_t DacData12Bit,
    uint16_t DacData6Bit,
    BoolFlag *const result
)
{
    if(DacData12Bit > 0xFFF) return AD5940ERR_PARA;
    if(DacData6Bit > 0x3F) return AD5940ERR_PARA;
    *result = (DacData12Bit) > (DacData6Bit * 64.0);
    return AD5940ERR_OK;
}

AD5940Err AD5940_Volt2LPDACData6Bit(
    const float voltage, 
    AD5940_VOLT2DAC_OPTION option,
    uint16_t *const DacData6Bit
)
{
    uint16_t bits;
    switch (option)
    {
    case AD5940_VOLT2DAC_OPTION_CEIL:
        bits = (uint16_t) ceilf(((double) voltage) / DAC6BITVOLT_1LSB);
        break;
    case AD5940_VOLT2DAC_OPTION_FLOOR:
        bits = (uint16_t) floorf(((double) voltage) / DAC6BITVOLT_1LSB);
        break;
    case AD5940_VOLT2DAC_OPTION_NEAREST:
        bits = (uint16_t) roundf(((double) voltage) / DAC6BITVOLT_1LSB);
        break;
    default:
        return AD5940ERR_PARA;
    }
    if(bits > 0x3F) return AD5940ERR_PARA;
    *DacData6Bit = bits;
    return AD5940ERR_OK;
}

AD5940Err AD5940_Volt2LPDACData12Bit(
    const float voltage, 
    AD5940_VOLT2DAC_OPTION option,
    BoolFlag is12BitGreater,
    uint16_t *const DacData12Bit
)
{
    uint16_t bits;
    switch (option)
    {
    case AD5940_VOLT2DAC_OPTION_CEIL:
        bits = (uint16_t) ceilf(((double) voltage) / DAC12BITVOLT_1LSB);
        break;
    case AD5940_VOLT2DAC_OPTION_FLOOR:
        bits = (uint16_t) floorf(((double) voltage) / DAC12BITVOLT_1LSB);
        break;
    case AD5940_VOLT2DAC_OPTION_NEAREST:
        bits = (uint16_t) roundf(((double) voltage) / DAC12BITVOLT_1LSB);
        break;
    default:
        return AD5940ERR_PARA;
    }
    if(is12BitGreater) bits++;
    if(bits > 0xFFF) return AD5940ERR_PARA;
    *DacData12Bit = bits;
    return AD5940ERR_OK;
}

AD5940Err AD5940_LPDACDataBits2LPDACDAT(
    uint16_t DacData12Bit,
    uint16_t DacData6Bit,
    uint32_t *const LPDACDAT
)
{
    *LPDACDAT = (((uint32_t) (DacData6Bit)) << 12) | ((uint32_t) (DacData12Bit));
    return AD5940ERR_OK;
}

AD5940Err AD5940_LPDACDAT2DacDataBits(
    uint32_t LPDACDAT,
    uint16_t *const DacData12Bit,
    uint16_t *const DacData6Bit
)
{
    *DacData12Bit = LPDACDAT & 0xFFF;
    *DacData6Bit = LPDACDAT >> 12;
    return AD5940ERR_OK;
}

AD5940Err AD5940_DacData6Bit2Volt(
    uint16_t DacData6Bit,
    float *const Volt
)
{
    if(DacData6Bit > 0x3F) return AD5940ERR_PARA;
    *Volt = DacData6Bit * DAC6BITVOLT_1LSB;
    return AD5940ERR_OK;
}

AD5940Err AD5940_DacData12Bit2Volt(
    uint16_t DacData12Bit,
    LPDACPara_Type LPDACPara,
    BoolFlag is12BitGreater,
    float *const Volt
)
{
    if(DacData12Bit > 0xFFF) return AD5940ERR_PARA;
    if(is12BitGreater) DacData12Bit--;
    *Volt = DacData12Bit * DAC12BITVOLT_1LSB;
    return AD5940ERR_OK;
}
