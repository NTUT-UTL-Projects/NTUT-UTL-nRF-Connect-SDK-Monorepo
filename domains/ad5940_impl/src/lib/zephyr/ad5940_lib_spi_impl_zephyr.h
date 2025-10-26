#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "spi_impl_zephyr.h"

int AD5940_spi_impl_zephyr_init(
    const struct spi_config *const spi_cfg,
    struct k_poll_signal *const spi_done_sig,
    const struct device *const spi_device,
    const struct gpio_dt_spec *const spi_cs
);

#ifdef __cplusplus
}
#endif
