/*
 * Copyright (c) 2024 Croxel, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/services/nus.h>

#include <zephyr/logging/log.h>
#define LOG_MODULE_NAME main
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#define DEVICE_NAME		CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN		(sizeof(DEVICE_NAME) - 1)

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_NUS_SRV_VAL),
};

static void notif_enabled(bool enabled, void *ctx)
{
	ARG_UNUSED(ctx);

	LOG_INF("%s() - %s\n", __func__, (enabled ? "Enabled" : "Disabled"));
}

static void received(struct bt_conn *conn, const void *data, uint16_t len, void *ctx)
{
	ARG_UNUSED(conn);
	ARG_UNUSED(ctx);

	LOG_INF("%s() - Len: %d, Message: %.*s\n", __func__, len, len, (char *)data);
}

struct bt_nus_cb nus_listener = {
	.notif_enabled = notif_enabled,
	.received = received,
};

static void mtu_exchange_cb(
	struct bt_conn *conn, 
	uint8_t err,
	struct bt_gatt_exchange_params *params
)
{
    LOG_INF("MTU exchange %u %s (%u)", 
		bt_conn_index(conn),
		err == 0U ? "successful" : "failed", 
		bt_gatt_get_mtu(conn)
	);
	return;
}

static void connected(struct bt_conn *conn, uint8_t err)
{
	static struct bt_gatt_exchange_params exchange_params;
	exchange_params.func = mtu_exchange_cb;

	err = bt_gatt_exchange_mtu(conn, &exchange_params);
	if (err) { LOG_ERR("MTU exchange failed: %d", err); }

	err = bt_conn_le_data_len_update(conn, BT_LE_DATA_LEN_PARAM_MAX);
	if (err) { LOG_ERR("Data length update failed: %d", err); }

	err = bt_conn_le_phy_update(conn, BT_CONN_LE_PHY_PARAM_2M);
	if (err) { LOG_ERR("Phy update request failed: %d",  err); }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	LOG_INF("Disconnected: %s (reason %u)", addr, reason);

	return;
}

static void recycled_cb(void)
{
    LOG_INF("Connection object recycled — can start advertising\n");
	int err;
	err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) { LOG_ERR("Failed to start advertising: %d\n", err); }
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
	.recycled = recycled_cb,
};

int main(void)
{
	int err;

	LOG_INF("Sample - Bluetooth Peripheral NUS\n");

	err = bt_nus_cb_register(&nus_listener, NULL);
	if (err) {
		LOG_ERR("Failed to register NUS callback: %d\n", err);
		return err;
	}

	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Failed to enable bluetooth: %d\n", err);
		return err;
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_ERR("Failed to start advertising: %d\n", err);
		return err;
	}

	LOG_INF("Initialization complete\n");

	while (true) {
		const char *hello_world = "Hello World!\n";

		k_sleep(K_SECONDS(3));

		err = bt_nus_send(NULL, hello_world, strlen(hello_world));
		LOG_INF("Data send - Result: %d\n", err);

		if (err < 0 && (err != -EAGAIN) && (err != -ENOTCONN)) {
			return err;
		}
	}

	return 0;
}
