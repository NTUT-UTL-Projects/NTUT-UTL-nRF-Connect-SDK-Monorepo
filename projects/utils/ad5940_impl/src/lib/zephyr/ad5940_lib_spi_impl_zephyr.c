#include "ad5940.h"
#include "ad5940_lib_spi_impl_zephyr.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

#include "spi_impl_zephyr.h"

const struct spi_config *_spi_cfg;
struct k_poll_signal *_spi_done_sig;
const struct device *_spi_device;
const struct gpio_dt_spec *_spi_cs;

int AD5940_spi_impl_zephyr_init(
    const struct spi_config *const spi_cfg,
    struct k_poll_signal *const spi_done_sig,
    const struct device *const spi_device,
    const struct gpio_dt_spec *const spi_cs
)
{
    int err = 0;

	err = z_impl_spi_device_init(spi_device);
	if(err)
	{
		return err;
	}
	_spi_device = spi_device;

	err = z_impl_spi_cs_init(spi_cs);
	if(err)
	{
		return err;
	}
	_spi_cs = spi_cs;

	_spi_cfg = spi_cfg;
	_spi_done_sig = spi_done_sig;

    return err;
}

void AD5940_CsSet(void)
{
    z_impl_spi_cs_select(_spi_cs, true);
	return;
}

void AD5940_CsClr(void)
{
    z_impl_spi_cs_select(_spi_cs, false);
	return;
}

void AD5940_ReadWriteNBytes(unsigned char *pSendBuffer, unsigned char *pRecvBuff, unsigned long length)
{
	z_impl_spi_transfer(
		_spi_device,
		_spi_cfg, 
		_spi_done_sig, 
		pSendBuffer,
		pRecvBuff,
		length
	);
	return;
}
