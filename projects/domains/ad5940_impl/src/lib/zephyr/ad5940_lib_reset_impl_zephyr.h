#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <zephyr/drivers/gpio.h>

int AD5940_Rst_impl_zephyr_init(const struct gpio_dt_spec *const dt);

#ifdef __cplusplus
}
#endif
