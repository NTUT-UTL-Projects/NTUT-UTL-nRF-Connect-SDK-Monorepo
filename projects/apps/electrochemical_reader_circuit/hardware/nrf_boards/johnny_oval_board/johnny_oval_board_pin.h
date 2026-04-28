#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "gpio_debounce_impl_zephyr.h"
#include "spi_impl_zephyr.h"

    extern const struct spi_config    spi_1_cfg;
    extern struct k_poll_signal       spi_1_done_sig;
    extern const struct device *const spi_1_device;
    extern const struct gpio_dt_spec  spi_1_cs;

    extern gpio_debounce_ctx_t       ad5940_gpio7_ctx;
    extern const struct gpio_dt_spec ad5940_gpio7_dt;

#ifdef __cplusplus
}
#endif
