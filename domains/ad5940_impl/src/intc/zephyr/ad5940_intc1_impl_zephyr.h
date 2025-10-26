#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "gpio_debounce_impl_zephyr.h"

int AD5940_intc1_impl_zephyr_init(
    gpio_debounce_ctx_t *const ctx,
    const struct gpio_dt_spec *const dt,
    void (*callback)(void)
);

#ifdef __cplusplus
}
#endif
