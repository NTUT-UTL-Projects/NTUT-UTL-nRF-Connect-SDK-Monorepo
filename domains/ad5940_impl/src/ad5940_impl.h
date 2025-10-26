#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

#include "ad5940_intc0_lock.h"
#include "ad5940_intc1_lock.h"

#if defined(CONFIG_ZEPHYR_BASE)
#include "ad5940_intc0_impl_zephyr.h"
#include "ad5940_intc1_impl_zephyr.h"
#include "ad5940_intc0_lock_impl_zephyr.h"
#include "ad5940_intc1_lock_impl_zephyr.h"
#include "ad5940_lib_reset_impl_zephyr.h"
#include "ad5940_lib_spi_impl_zephyr.h"
#endif

#ifdef __cplusplus
}
#endif
