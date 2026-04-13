#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940_controller.h"

typedef struct
{
	AD5940_CONTROLLER_RESET_OPTION ad5940_controller_reset;
	AD5940_CONTROLLER_CAL_PARA ad5940_controller_cal_para;
	int (*const ad5940_gpio_init) (void);
	int (*const ad5940_intc_init) (void);
	int (*const ad5940_intc_wait) (void);
	int (*const ad5940_spi_init) (void);
	AGPIOCfg_Type ad5940_gpio_cfg;
} hardware_t;

extern hardware_t hardware;

#ifdef __cplusplus
}
#endif
