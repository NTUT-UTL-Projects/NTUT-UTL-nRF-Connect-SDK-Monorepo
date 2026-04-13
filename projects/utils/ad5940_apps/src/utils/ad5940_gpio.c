#include "ad5940_gpio.h"

void AD5940_GPIOClr(void)
{
    AD5940_INTCCfg(AFEINTC_1, AFEINTSRC_ALLINT, bFALSE);
    AD5940_INTCCfg(AFEINTC_0, AFEINTSRC_ALLINT, bFALSE);
    AD5940_INTCClrFlag(AFEINTSRC_ALLINT);

    AGPIOCfg_Type gpio_cfg = {};
    AD5940_AGPIOCfg(&gpio_cfg);

    return;
}

void AD5940_FuncSet2AFEINTCx(
    uint32_t FuncSet,
    BoolFlag *const AFEINTC_0_En,
    BoolFlag *const AFEINTC_1_En
)
{
    if(
        ((FuncSet & BITM_AGPIO_GP0CON_PIN0CFG) == GP0_INT)
        || ((FuncSet & BITM_AGPIO_GP0CON_PIN3CFG) == GP3_INT0)
        || ((FuncSet & BITM_AGPIO_GP0CON_PIN6CFG) == GP6_INT0)
    ) *AFEINTC_0_En = bTRUE;
    else *AFEINTC_0_En = bFALSE;
    if(
        ((FuncSet & BITM_AGPIO_GP0CON_PIN4CFG) == GP4_INT1)
        || ((FuncSet & BITM_AGPIO_GP0CON_PIN7CFG) == GP7_INT)
    ) *AFEINTC_1_En = bTRUE;
    else *AFEINTC_1_En = bFALSE;
    return;
}

void AD5940_AFEINTCxCfg(
    BoolFlag AFEINTC_0_En,
    BoolFlag AFEINTC_1_En,
    int32_t AFEIntSrc
)
{
    if(AFEINTC_0_En) AD5940_INTCCfg(AFEINTC_0, AFEIntSrc, bTRUE);
    if(AFEINTC_1_En) AD5940_INTCCfg(AFEINTC_1, AFEIntSrc, bTRUE);
    return;
}
