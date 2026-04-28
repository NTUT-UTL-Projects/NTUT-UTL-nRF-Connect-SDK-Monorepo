#include "ad5940.h"
#include "ad5940_lib_reset_impl_zephyr.h"

#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec *_dt;

int AD5940_Rst_impl_zephyr_init(const struct gpio_dt_spec *const dt)
{
    int err;

	err = gpio_pin_configure_dt(dt, GPIO_OUTPUT_ACTIVE);
	if (err < 0) {
		return 0;
	}
	_dt = dt;

    return err;
}

void AD5940_RstSet(void)
{
	if(_dt == NULL) return;
    gpio_pin_set_dt(_dt, 1);
	return;
}

void AD5940_RstClr(void)
{
	if(_dt == NULL) return;
    gpio_pin_set_dt(_dt, 0);
	return;
}
