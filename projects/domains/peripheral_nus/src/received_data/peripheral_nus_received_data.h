#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <zephyr/bluetooth/conn.h>

#include <stdint.h>

void peripheral_nus_received_data_ring_buffer_init(uint8_t *const data, uint16_t max_len);

void peripheral_nus_received_data_get_len(uint16_t *len);

/**
 * @brief Get data by memcpy
 * 
 * @param data 
 */
void peripheral_nus_received_data_get_data(uint8_t *data, uint16_t len);

void peripheral_nus_received_data_listener(struct bt_conn *conn, const void *data, uint16_t len, void *ctx);

#ifdef __cplusplus
}
#endif
