#include "ad5940.h"

/**
 * @brief @see page 43 of the datasheet.
 * 
 * @param EXCTBUFGAIN   select from EXCTBUFGAIN_2, EXCTBUFGAIN_0P25 @ref EXCITBUFGAIN_Const
*/
const float AD5940_EXCITBUFGAIN_to_inampgnmde[] = {2.0, 0.25};

/**
 * @brief @see page 43 of the datasheet.
 * 
 * @param HSDACGAIN     select from HSDACGAIN_1, HSDACGAIN_0P2 @ref HSDACGAIN_Const
*/
const float AD5940_HSDACGAIN_to_attenen[] = {1.0, 0.2};

/**
 * @brief @see page 43 of the datasheet.
 * 
 * @param HSDACDAT
*/
extern inline BoolFlag AD5940_HSDACDAT_is_vaild(uint32_t HSDACDAT);

/**
 * @brief @see page 43 of the datasheet.
 * 
 * @param volts
*/
extern inline uint32_t AD5940_volts_to_HSDACDAT(float volts, float inampgnmde, float attenen);

/**
 * @brief @see page 43 of the datasheet.
 * 
 * @param HSDACDAT
*/
extern inline float AD5940_HSDACDAT_to_volts(uint32_t HSDACDAT, float inampgnmde, float attenen);
