#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <zephyr/bluetooth/services/nus.h>

    int peripheral_nus_init(struct bt_nus_cb *const nus_listener);

#ifdef __cplusplus
}
#endif
