#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

/**
 * Retrieves the value of the RCAL resistor.
 * 
 * @note 
 * Refer to page 46 of the datasheet for more details. 
 * This function must be implemented to match your specific board design.
 * 
 * @return The resistance value (in ohms) of the resistor connected between 
 *         the RCAL0 pin and the RCAL1 pin.
 */
#if defined(CONFIG_VIRUS_DETECTOR)
#define AD5940_fRcal 1000.0
#endif
#if defined(CONFIG_JOHNNY_OVAL)
#define AD5940_fRcal 10000.0
#endif

/**
 * Retrieves the resistance value of the resister between RC0_0 and RC0_1.
 * 
 * @note 
 * This function must be implemented to match your specific board design. 
 * It's often use to The HSRTIA resistance is used only when the HSTIA is powered on and 
 * `HstiaRtiaSel` is set to `HSTIARTIA_OPEN`.
 * 
 * @return The resistance value (in ohms) of the HSRTIA.
 */
#define AD5940_fRc0 0.0     // Our project didn't use it.

// Our circuit use GPIO7 as interrupt pin.
extern AGPIOCfg_Type AD5940_gpio_cfg;

#define AD5940_VRef1p11 1.11f

/**
 * Refer to pgae 87 of the datasheet.
 * This application use internal reference voltage.
 */
#define AD5940_VRef1p82 1.82f
#define AD5940_ADCRefVolt AD5940_VRef1p82

/**
 * Refer to page 39 of the datasheet.
 * @see AD5940_LPRtiaCal.
 */
#define AD5940_bWithCtia bTRUE

#define AD5940_DataType DATATYPE_SINC2

#define AD5940_FifoSrc FIFOSRC_SINC2NOTCH

#ifdef __cplusplus
}
#endif
