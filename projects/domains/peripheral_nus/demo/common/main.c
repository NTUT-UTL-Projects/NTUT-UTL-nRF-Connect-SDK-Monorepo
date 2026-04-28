#include "peripheral_nus.h"

#include <zephyr/bluetooth/services/nus.h>
#include <zephyr/kernel.h>

static const uint8_t *data_buffer;
static uint16_t       data_buffer_len;

static void notif_enabled(bool enabled, void *ctx)
{
    ARG_UNUSED(ctx);

    printk("%s() - %s\n", __func__, (enabled ? "Enabled" : "Disabled"));
}

static void
received(struct bt_conn *conn, const void *data, uint16_t len, void *ctx)
{
    ARG_UNUSED(conn);
    ARG_UNUSED(ctx);

    printk("%s() - Len: %d, Message: %.*s\n", __func__, len, len, (char *)data);

    data_buffer     = data;
    data_buffer_len = len;
}

static struct bt_nus_cb nus_listener = {
    .notif_enabled = notif_enabled,
    .received      = received,
};

int main(void)
{
    int err;

    err = peripheral_nus_init(&nus_listener);

    if (err)
    {
        printk("Failed to peripheral_nus_init callback: %d\n", err);
        return err;
    }

    while (true)
    {
        k_sleep(K_MSEC(500));

        if (data_buffer == NULL)
            continue;

        // err = bt_nus_send(NULL, data_buffer, data_buffer_len);
        err = bt_nus_send(NULL, &data_buffer_len, sizeof(data_buffer_len));
        printk("Data send - Result: %d\n", err);

        if (err < 0 && (err != -EAGAIN) && (err != -ENOTCONN))
        {
            return err;
        }
    }

    return 0;
}