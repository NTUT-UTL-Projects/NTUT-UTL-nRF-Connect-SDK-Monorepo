#include "ad5940_lpdac_para.h"

#include <stdlib.h>

AD5940Err AD5940_LPDACData12BitCal(
    uint16_t *const DacData12Bit,
    LPDACPara_Type LPDACPara
)
{
    return AD5940ERR_OK;
}

AD5940Err AD5940_LPDACData6BitCal(
    uint16_t *const DacData6Bit,
    LPDACPara_Type LPDACPara
)
{
    return AD5940ERR_OK;
}

AD5940Err AD5940_VoltLPDACData12BitCal(
    float *const Volt,
    LPDACPara_Type LPDACPara
)
{
    *Volt = ((*Volt) * LPDACPara.kC2V_DAC12B) + LPDACPara.bC2V_DAC12B;
    return AD5940ERR_OK;
}

AD5940Err AD5940_VoltLPDACData6BitCal(
    float *const Volt,
    LPDACPara_Type LPDACPara
)
{
    *Volt = ((*Volt) * LPDACPara.kC2V_DAC6B) + LPDACPara.bC2V_DAC6B;
    return AD5940ERR_OK;
}
