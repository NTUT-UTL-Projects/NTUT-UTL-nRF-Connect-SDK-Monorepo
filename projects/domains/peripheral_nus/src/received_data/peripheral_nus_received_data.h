#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <zephyr/bluetooth/conn.h>

    int peripheral_nus_received_data_ring_buffer_init(
        uint8_t *const data,
        uint16_t       max_len
    );

    /**
     * @brief Get data length from ring buffer.
     *
     * @param len
     */
    void peripheral_nus_received_data_get_len(uint16_t *len);

    /**
     * @brief Get data by memcpy.
     *
     * @param data
     * @param len
     */
    void peripheral_nus_received_data_get_data(uint8_t *data, uint16_t len);

    void peripheral_nus_received_data_wait_for_data(void);

    void peripheral_nus_received_data_listener(
        struct bt_conn *conn,
        const void     *data,
        uint16_t        len,
        void           *ctx
    );

#ifdef __cplusplus
}
#endif
