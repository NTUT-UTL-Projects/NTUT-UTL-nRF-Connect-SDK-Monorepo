#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "gpio_debounce_impl_zephyr.h"

#include "spi_impl_zephyr.h"

extern const struct spi_config spi_1_cfg;
extern struct k_poll_signal spi_1_done_sig;
extern const struct device *const spi_1_device;
extern const struct gpio_dt_spec spi_1_cs;

#if defined(CONFIG_VIRUS_DETECTOR)

extern gpio_debounce_ctx_t ad5940_gpio0_ctx;
extern const struct gpio_dt_spec ad5940_gpio0_dt;

extern const struct gpio_dt_spec ad5940_reset_dt;

#endif

#if defined(CONFIG_JOHNNY_OVAL)

extern gpio_debounce_ctx_t ad5940_gpio7_ctx;
extern const struct gpio_dt_spec ad5940_gpio7_dt;

#endif

#ifdef __cplusplus
}
#endif
