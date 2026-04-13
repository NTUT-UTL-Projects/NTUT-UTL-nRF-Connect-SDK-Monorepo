#include "johnny_oval_board.h"

#include "ad5940_controller.h"
#include "ad5940_impl.h"

int ad5940_intc_init(void)
{
    int err;
	err = ad5940_intc0_lock_init_impl_zephyr();
	if (err) return err;
	err = ad5940_intc1_lock_init_impl_zephyr();
	if (err) return err;
    return 0;
}

int ad5940_intc_wait(void)
{
    int err;

    err = ad5940_intc1_lock_wait();
    if (err) return err;

    return 0;
}

int ad5940_gpio_init(void)
{
    int err;

	err = ad5940_intc1_lock_init_impl_zephyr();
	if (err) return err;

	err = AD5940_intc1_impl_zephyr_init(
		&ad5940_gpio7_ctx,
		&ad5940_gpio7_dt,
		ad5940_intc1_lock_boardcast
	);
	if (err) return err;

    return 0;
}

int ad5940_spi_init(void)
{
    int err;

	err = AD5940_spi_impl_zephyr_init(
		&spi_1_cfg,
		&spi_1_done_sig,
		spi_1_device,
		&spi_1_cs
	);
	if (err) return err;

    return 0;
}

hardware_t hardware = {
    .ad5940_controller_cal_para = {
        .ADCRefVolt = AD5940_ADCRefVolt,
        .bWithCtia = AD5940_bWithCtia,
        .fRcal = AD5940_fRcal,
        .VRef1p11 = AD5940_VRef1p11,
        .VRef1p82 = AD5940_VRef1p82,
    },
    .ad5940_controller_reset = AD5940_CONTROLLER_RESET_OPTION_SOFEWARE,
    // This circuit use GPIO7 as interrupt pin.
    .ad5940_gpio_cfg = {
        .FuncSet = GP0_GPIO | GP7_INT,
        .InputEnSet = 0,
        .OutputEnSet = AGPIO_Pin7,
        .OutVal = 0,
        .PullEnSet = AGPIO_Pin7,
    },
    .ad5940_intc_init = ad5940_intc_init,
    .ad5940_intc_wait = ad5940_intc_wait,
    .ad5940_gpio_init = ad5940_gpio_init,
    .ad5940_spi_init = ad5940_spi_init,
};
