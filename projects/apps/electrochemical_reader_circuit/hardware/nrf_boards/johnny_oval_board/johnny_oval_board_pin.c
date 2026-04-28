#include "johnny_oval_board_pin.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

const struct spi_config spi_1_cfg = {
    .operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
    .frequency = 6400000,
    .slave     = SPI_OP_MODE_MASTER,
};
struct k_poll_signal spi_1_done_sig = K_POLL_SIGNAL_INITIALIZER(spi_1_done_sig);

const struct device *const spi_1_device = DEVICE_DT_GET(DT_NODELABEL(spi1));

const struct gpio_dt_spec spi_1_cs =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), ad5940_cs_gpios);

gpio_debounce_ctx_t       ad5940_gpio7_ctx;
const struct gpio_dt_spec ad5940_gpio7_dt =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), ad5940_gpio7_gpios);
