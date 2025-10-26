/**
 * Configures the voltage between V_bias and V_zero.
 * 
 * @note
 * Refer to page 30 of the datasheet for detailed information on setting the voltage
 * between V_bias and V_zero. The default MUX settings used in this file are:
 * - `LPDACVBIAS_12BIT` for V_bias (higher precision: 12 bits)
 * - `LPDACVZERO_6BIT` for V_zero (lower precision: 6 bits)
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
#include "ad5940_volt.h"

AD5940Err AD5940_LPDACData12BitIsGreater(
    uint16_t DacData12Bit,
    uint16_t DacData6Bit,
    BoolFlag *const result
);

AD5940Err AD5940_Volt2LPDACData6Bit(
    const float voltage, 
    AD5940_VOLT2DAC_OPTION option,
    uint16_t *const DacData6Bit
);

AD5940Err AD5940_Volt2LPDACData12Bit(
    const float voltage, 
    AD5940_VOLT2DAC_OPTION option,
    BoolFlag is12BitGreater,
    uint16_t *const DacData12Bit
);

AD5940Err AD5940_LPDACDataBits2LPDACDAT(
    uint16_t DacData12Bit,
    uint16_t DacData6Bit,
    uint32_t *const LPDACDAT
);

AD5940Err AD5940_LPDACDAT2DacDataBits(
    uint32_t LPDACDAT,
    uint16_t *const DacData12Bit,
    uint16_t *const DacData6Bit
);

AD5940Err AD5940_DacData6Bit2Volt(
    uint16_t DacData6Bit,
    float *const Volt
);

AD5940Err AD5940_DacData12Bit2Volt(
    uint16_t DacData12Bit,
    LPDACPara_Type LPDACPara,
    BoolFlag is12BitGreater,
    float *const Volt
);

#ifdef __cplusplus
}
#endif
