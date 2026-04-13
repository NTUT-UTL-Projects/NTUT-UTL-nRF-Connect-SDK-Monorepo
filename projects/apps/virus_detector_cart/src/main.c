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

// ========================================
// PWM

static const struct pwm_dt_spec custompwm[] = 
{
	PWM_DT_SPEC_GET_BY_IDX(DT_ALIAS(mycustompwm), 0),
	PWM_DT_SPEC_GET_BY_IDX(DT_ALIAS(mycustompwm), 1),
	PWM_DT_SPEC_GET_BY_IDX(DT_ALIAS(mycustompwm), 2),
	// PWM_DT_SPEC_GET_BY_IDX(DT_ALIAS(mycustompwm), 3),
};

#define PWM_DEVICES_LENGTH sizeof(custompwm) / sizeof(custompwm[0])

// ========================================
// GPIO

#define USER_PATH DT_PATH(zephyr_user)
static const struct gpio_dt_spec gpio_devices[] = {
	GPIO_DT_SPEC_GET(USER_PATH, track0_gpios),
	GPIO_DT_SPEC_GET(USER_PATH, track1_gpios),
	GPIO_DT_SPEC_GET(USER_PATH, track2_gpios),
	GPIO_DT_SPEC_GET(USER_PATH, track3_gpios),
};

#define GPIO_DEVICES_LENGTH sizeof(gpio_devices) / sizeof(gpio_devices[0])

// ========================================
// Valve

#include "valve.h"

// ========================================
// thread

static k_tid_t wheel_thread_tid;
static struct k_thread wheel_thread_thread;
K_THREAD_STACK_DEFINE(wheel_thread_stack, 16384);
#define wheel_thread_prio 5

static k_tid_t valve_thread_tid;
static struct k_thread valve_thread_thread;
K_THREAD_STACK_DEFINE(valve_thread_stack, 16384);
#define valve_thread_prio 5

// ========================================
// queue

typedef struct
{
	uint8_t gpio[GPIO_DEVICES_LENGTH];
	uint8_t pwm[PWM_DEVICES_LENGTH];
} WHEEL_PARA;

#define WHEEL_MAX_QUEUE 1000
K_MSGQ_DEFINE(_wheel_quene, sizeof(WHEEL_PARA), WHEEL_MAX_QUEUE, 4);

typedef struct
{
	uint8_t direction;
} VALVE_PARA;

#define VALVE_MAX_QUEUE 1000
K_MSGQ_DEFINE(_valve_quene, sizeof(VALVE_PARA), VALVE_MAX_QUEUE, 4);

// ========================================
// thread runner

void wheel_thread_runner(void)
{
	int err;
	while(true)
	{
		WHEEL_PARA wheel;
		k_msgq_get(&_wheel_quene, &wheel, K_FOREVER);
		for(size_t i=0; i<GPIO_DEVICES_LENGTH; i++)
		{
			err = gpio_pin_set_dt(
				gpio_devices + i, 
				(int) wheel.gpio[i]
			);
			if(err) continue;
		}
		for(size_t i=0; i<PWM_DEVICES_LENGTH; i++)
		{
			err = pwm_set_pulse_dt(
				custompwm + i, 
				custompwm->period * (((float) (wheel.pwm[i])) / 255.0f)
			);
			if(err) continue;
		}
	}
}

void valve_thread_runner(void)
{
	int err;
	while(true)
	{
		VALVE_PARA valve;
		k_msgq_get(&_valve_quene, &valve, K_FOREVER);
		valve_turn(valve.direction);
	}
}

// ========================================
// Bluetooth

typedef enum
{
	COMMAND_TYPE_WHEEL,
	COMMAND_TYPE_VALVE,
} COMMAND_TYPE;

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

	if(len < 1) return;
	uint16_t new_len = len - 1;
	switch (data[0])
	{
	case COMMAND_TYPE_WHEEL:
		if(new_len != sizeof(WHEEL_PARA)) return;
		k_msgq_put(&_wheel_quene, (data + 1), K_NO_WAIT);
		break;
	case COMMAND_TYPE_VALVE:
		if(new_len != sizeof(VALVE_PARA)) return;
		k_msgq_put(&_valve_quene, (data + 1), K_NO_WAIT);
		break;
	default:
		break;
	}
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

// ========================================
// Main

int main(void)
{
	int err;

	// ========================================
	// GPIO

	// Check if the output voltage of REGOUT0 is not set to 3.0V
	if ((NRF_UICR->REGOUT0 & UICR_REGOUT0_VOUT_Msk)
		!= (UICR_REGOUT0_VOUT_3V0 << UICR_REGOUT0_VOUT_Pos)) {

		// Enable write mode for Non-Volatile Memory Controller (NVMC)
		NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos;
		
		// Wait until NVMC is ready
		while (NRF_NVMC->READY == NVMC_READY_READY_Busy);
		
		// Set REGOUT0 output voltage to 3.0V
		NRF_UICR->REGOUT0 = (NRF_UICR->REGOUT0 & ~((uint32_t)UICR_REGOUT0_VOUT_Msk))
							| (UICR_REGOUT0_VOUT_3V0 << UICR_REGOUT0_VOUT_Pos);
		
		// Set NVMC back to read mode
		NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos;
		
		// Wait until NVMC is ready
		while (NRF_NVMC->READY == NVMC_READY_READY_Busy);
		
		// Reset the system to apply the new setting
		NVIC_SystemReset();
	}

	for(size_t i=0; i<GPIO_DEVICES_LENGTH; i++)
	{
		if (gpio_pin_configure_dt(&(gpio_devices[i]), GPIO_OUTPUT_ACTIVE)) {
			// printk("Error: GPIO device %s is not ready\n", gpio_devices[i]);
			return 0;
		}
	}
	
	// ========================================
	// PWM

	for(size_t i=0; i<PWM_DEVICES_LENGTH; i++)
	{
		if (!pwm_is_ready_dt(custompwm + i)) {
			// printk("Error: PWM device %s is not ready\n", gpio_devices[i]);
			return 0;
		}
	}

	for(size_t i=0; i<PWM_DEVICES_LENGTH; i++)
	{
		err = pwm_set_pulse_dt(
			custompwm + i, 
			0
		);
	}
	
	// ========================================
	// Valve

	valve_init();

	// ========================================
	// Bluetooth

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

	// ========================================
	// Threads

	wheel_thread_tid = k_thread_create(
		&wheel_thread_thread,
		wheel_thread_stack,
		K_THREAD_STACK_SIZEOF(wheel_thread_stack),
		wheel_thread_runner,
		NULL, NULL, NULL,
		wheel_thread_prio, 0,
		K_NO_WAIT
	);

	valve_thread_tid = k_thread_create(
		&valve_thread_thread,
		valve_thread_stack,
		K_THREAD_STACK_SIZEOF(valve_thread_stack),
		valve_thread_runner,
		NULL, NULL, NULL,
		valve_thread_prio, 0,
		K_NO_WAIT
	);

	return 0;
}
