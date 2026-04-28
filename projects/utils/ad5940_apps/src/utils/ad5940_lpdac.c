#include "ad5940_lpdac.h"

extern inline BoolFlag AD5940_DacDAta12Bit_is_vaild(uint16_t DacData12Bit);
extern inline BoolFlag AD5940_DacDAta6Bit_is_vaild(uint16_t DacData6Bit);

extern inline BoolFlag
AD5940_LPDACData12Bit_is_greater(uint16_t DacData12Bit, uint16_t DacData6Bit);

extern inline uint16_t AD5940_volts_to_LPDACData6Bit(float volts);
extern inline uint16_t
AD5940_volts_to_LPDACData12Bit(float volts, uint16_t DacData6Bit);

extern inline uint32_t
AD5940_LPDACDataBits_to_LPDACDAT(uint16_t DacData12Bit, uint16_t DacData6Bit);

extern inline uint16_t AD5940_LPDACDAT_to_DacData12Bit(uint32_t LPDACDAT);
extern inline uint16_t AD5940_LPDACDAT_to_DacData6Bit(uint32_t LPDACDAT);

extern inline float AD5940_DacData12Bit_to_volts(
    uint16_t DacData12Bit,
    BoolFlag LPDACData12Bit_is_greater
);
extern inline float AD5940_DacData6Bit_to_volts(uint16_t DacData6Bit);
