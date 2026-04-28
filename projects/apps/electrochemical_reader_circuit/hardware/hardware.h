#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940_controller.h"

    typedef struct
    {
        AD5940_CONTROLLER_RESET_OPTION ad5940_controller_reset;
        AD5940_CONTROLLER_CAL_PARA     ad5940_controller_cal_para;
        AGPIOCfg_Type                  ad5940_gpio_cfg;
        int (*const ad5940_gpio_init)(void);
        int (*const ad5940_intc_init)(void);
        int (*const ad5940_intc_wait)(void);
        int (*const ad5940_spi_init)(void);
    } hardware_t;

#ifdef __cplusplus
}
#endif
