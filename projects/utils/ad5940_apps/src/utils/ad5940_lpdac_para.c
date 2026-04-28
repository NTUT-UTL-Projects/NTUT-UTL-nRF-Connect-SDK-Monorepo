#include "ad5940_lpdac_para.h"

// TODO
extern inline void
AD5940_LPDACData12Bit_cal(uint16_t DacData12Bit, LPDACPara_Type LPDACPara);
extern inline void
AD5940_LPDACData6Bit_cal(uint16_t DacData6Bit, LPDACPara_Type LPDACPara);

extern inline float
AD5940_volts_LPDACData12Bit_cal(float volts, LPDACPara_Type LPDACPara);
extern inline float
AD5940_volts_LPDACData6Bit_cal(float volts, LPDACPara_Type LPDACPara);
