#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

/**
 * Clears all configuration settings related to GPIO and interrupt (INT) flags.
 * 
 * @note
 * Refer to pages 112 and 122 of the datasheet for detailed information. 
 * Also, check the `ad5940.c` file for implementation details.
 */
void AD5940_GPIOClr(void);

/**
 * Retrieves the interrupt (INT) enable status based on the specified AGPIO configuration.
 * 
 * @note
 * Refer to pages 112 and 123 of the datasheet for more details. 
 * This function maps the relationship between GPIO and interrupt controllers (INT).
 * 
 * @param FuncSet       @see AGPIOCfg_Type.FuncSet.
 * @param AFEINTC_0_En  Pointer to a `BoolFlag` to store the enable status of `AFEINTC_0`.
 * @param AFEINTC_1_En  Pointer to a `BoolFlag` to store the enable status of `AFEINTC_1`.
 */
void AD5940_FuncSet2AFEINTCx(
    uint32_t FuncSet,
    BoolFlag *const AFEINTC_0_En,
    BoolFlag *const AFEINTC_1_En
);

/**
 * Configures the interrupt controller (INTC) settings based on the specified AGPIO configuration.
 * 
 * @note
 * This function is implemented based on @ref AD5940_INTCCfg,
 * which is defined in ad5940.h.
 * 
 * @param AFEINTC_0_En  enable to config `AFEINTC_0`.
 * @param AFEINTC_1_En  enable to config `AFEINTC_1`.
 * @param AFEIntSrc     Interrupt source to configure. Refer to @ref AFEINTC_SRC_Const 
 *                      for available interrupt source flags.
 */
void AD5940_AFEINTCxCfg(
    BoolFlag AFEINTC_0_En,
    BoolFlag AFEINTC_1_En,
    int32_t AFEIntSrc
);

#ifdef __cplusplus
}
#endif
