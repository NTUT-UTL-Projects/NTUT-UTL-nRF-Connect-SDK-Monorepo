/**
 * TODO
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

AD5940Err AD5940_LPDACData12BitCal(
    uint16_t *const DacData12Bit,
    LPDACPara_Type LPDACPara
);

AD5940Err AD5940_LPDACData6BitCal(
    uint16_t *const DacData6Bit,
    LPDACPara_Type LPDACPara
);

AD5940Err AD5940_VoltLPDACData12BitCal(
    float *const Volt,
    LPDACPara_Type LPDACPara
);

AD5940Err AD5940_VoltLPDACData6BitCal(
    float *const Volt,
    LPDACPara_Type LPDACPara
);

#ifdef __cplusplus
}
#endif
