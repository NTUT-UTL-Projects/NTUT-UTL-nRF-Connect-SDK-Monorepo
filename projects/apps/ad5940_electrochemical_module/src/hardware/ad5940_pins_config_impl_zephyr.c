#include "ad5940_pins_config_impl_zephyr.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

const struct spi_config spi_1_cfg = {
	.operation = 
		SPI_OP_MODE_MASTER
		| SPI_WORD_SET(8)
		| SPI_TRANSFER_MSB,
	.frequency = 6400000,
	.slave = SPI_OP_MODE_MASTER,
};
struct k_poll_signal spi_1_done_sig = K_POLL_SIGNAL_INITIALIZER(spi_1_done_sig);

const struct device *const spi_1_device = DEVICE_DT_GET(
	DT_NODELABEL(spi1)
);

const struct gpio_dt_spec spi_1_cs = GPIO_DT_SPEC_GET(
	DT_PATH(zephyr_user), 
	ad5940_cs_gpios
);

#if defined(CONFIG_VIRUS_DETECTOR)

gpio_debounce_ctx_t ad5940_gpio0_ctx;
const struct gpio_dt_spec ad5940_gpio0_dt = GPIO_DT_SPEC_GET(
    DT_PATH(zephyr_user),
    ad5940_gpio0_gpios
);

const struct gpio_dt_spec ad5940_reset_dt = GPIO_DT_SPEC_GET(
	DT_PATH(zephyr_user),
	ad5940_reset_gpios
);

#endif

#if defined(CONFIG_JOHNNY_OVAL)

gpio_debounce_ctx_t ad5940_gpio7_ctx;
const struct gpio_dt_spec ad5940_gpio7_dt = GPIO_DT_SPEC_GET(
    DT_PATH(zephyr_user),
    ad5940_gpio7_gpios
);

#endif
