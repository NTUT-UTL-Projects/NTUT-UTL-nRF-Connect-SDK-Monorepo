#include "peripheral_nus.h"
#include "peripheral_nus_received_data.h"

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/services/nus.h>

#define DATA_BUFFER_LEN 1024
static uint8_t data_buffer[DATA_BUFFER_LEN];

static struct bt_nus_cb nus_listener = {
	.received = peripheral_nus_received_data_listener,
};

int main(void)
{
	int err;

	peripheral_nus_received_data_ring_buffer_init(data_buffer, DATA_BUFFER_LEN);

    err = peripheral_nus_init(&nus_listener);
    
	if (err) {
		printk("Failed to peripheral_nus_init callback: %d\n", err);
		return err;
	}

	while (true) {
		k_sleep(K_MSEC(500));

		uint16_t len;
		peripheral_nus_received_data_get_len(&len);
		if (len == 0) continue;

		#define CHUNK_LEN 5
		for (size_t i = 0; i < len; i+=CHUNK_LEN)
		{
			k_sleep(K_MSEC(500));
			
			uint16_t chunk_len = CHUNK_LEN;
			if (i + CHUNK_LEN > len)
			{
				chunk_len = len - i;
			}
			uint8_t data_buffer[chunk_len];
			peripheral_nus_received_data_get_data(data_buffer, chunk_len);

			for (size_t j = 0; j < chunk_len; j++)
			{
				printk("0x%02x, ", data_buffer[i]);
			}

			err = bt_nus_send(NULL, data_buffer, chunk_len);
			printk("Data send - Result: %d\n", err);

			if (err < 0 && (err != -EAGAIN) && (err != -ENOTCONN)) {
				return err;
			}
		}
		printk("\n");
	}

    return 0;
}
