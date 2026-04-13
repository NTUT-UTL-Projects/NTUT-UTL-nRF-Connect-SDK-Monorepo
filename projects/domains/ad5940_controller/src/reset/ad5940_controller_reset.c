#include "ad5940_controller_reset.h"

AD5940Err AD5940_controller_reset(
    uint32_t *const SEQGenBuff, 
    const uint16_t SEQGenBuffLen,
    const AD5940_CONTROLLER_RESET_OPTION reset_option
)
{
    AD5940Err err = AD5940ERR_OK;

    if(SEQGenBuff == 0) return AD5940ERR_PARA;
    if(SEQGenBuffLen == 0) return AD5940ERR_PARA;

    AD5940_SEQGenInit(
        SEQGenBuff, 
        SEQGenBuffLen
    );

    // Reset
    switch (reset_option)
    {
    case AD5940_CONTROLLER_RESET_OPTION_HAREWARE:
        AD5940_HWReset();
        break;
    case AD5940_CONTROLLER_RESET_OPTION_SOFEWARE:
        err = AD5940_SoftRst();
        if(err != AD5940ERR_OK) return err;
        break;
    }

    /* Platform configuration */
    AD5940_Initialize();

    /* Enable AFE to enter sleep mode. */
    AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK);

    /* Goto hibernate */
    AD5940_EnterSleepS();

    return AD5940ERR_OK;
}
