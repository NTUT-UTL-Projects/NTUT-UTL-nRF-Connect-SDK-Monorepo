#include "ad5940_intc0_impl_zephyr.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(ad5940_intc0, LOG_LEVEL_INF);

int AD5940_intc0_impl_zephyr_init(
    gpio_debounce_ctx_t *const ctx,
    const struct gpio_dt_spec *const dt,
    void (*callback)(void)
)
{
    int ret;

    ret = z_impl_gpio_debounce_init(
        ctx,
        dt,
        GPIO_INPUT,
        K_USEC(100),
        NULL,
        callback
    );
    if (ret) {
        LOG_ERR("GPIO device not ready");
        return -ENODEV;
    }

    return ret;
}
