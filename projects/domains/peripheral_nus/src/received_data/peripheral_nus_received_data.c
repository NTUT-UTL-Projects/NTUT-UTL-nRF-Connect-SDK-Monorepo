#include "peripheral_nus_received_data.h"

#include <stdatomic.h>

typedef struct
{
	uint8_t *data;
	uint16_t max_len;
	uint8_t * volatile _Atomic head;
	uint8_t * volatile _Atomic tail;
} _ring_buffer;

static _ring_buffer __ring_buffer;

void peripheral_nus_received_data_ring_buffer_init(uint8_t *const data, uint16_t max_len)
{
	__ring_buffer.data = data;
	__ring_buffer.max_len = max_len;
	atomic_store(&__ring_buffer.head, data);
	atomic_store(&__ring_buffer.tail, data);
	return;
}

void peripheral_nus_received_data_get_len(uint16_t *len)
{
	uint8_t *head = atomic_load(&__ring_buffer.head);
	uint8_t *tail = atomic_load(&__ring_buffer.tail);
	if (head >= tail)
	{
		*len = head - tail;
	}
	else
	{
		*len = __ring_buffer.max_len - (tail - head);
	}
	return;
}

void peripheral_nus_received_data_get_data(uint8_t *data)
{
	uint8_t *head = atomic_load(&__ring_buffer.head);
	uint8_t *tail = atomic_load(&__ring_buffer.tail);
	if (head >= tail)
	{
		memcpy(data, tail, head - tail);
	}
	else
	{
		uint16_t first_part_len = __ring_buffer.data + __ring_buffer.max_len - tail;
		memcpy(data, tail, first_part_len);
		memcpy(data + first_part_len, __ring_buffer.data, head - __ring_buffer.data);
	}
	atomic_store(&__ring_buffer.tail, head);
	return;
}

void peripheral_nus_received_data_listener(struct bt_conn *conn, const void *data, uint16_t len, void *ctx)
{
	ARG_UNUSED(conn);
	ARG_UNUSED(ctx);

	uint8_t *head = atomic_load(&__ring_buffer.head);
	if (head + len > __ring_buffer.data + __ring_buffer.max_len)
	{
		uint16_t first_part_len = __ring_buffer.data + __ring_buffer.max_len - head;
		memcpy(head, data, first_part_len);
		memcpy(__ring_buffer.data, (uint8_t *) data + first_part_len, len - first_part_len);
		atomic_store(&__ring_buffer.head, __ring_buffer.data + len - first_part_len);
	}
	else
	{
		memcpy(head, data, len);
		head += len;
		if (head == __ring_buffer.data + __ring_buffer.max_len)
		{
			atomic_store(&__ring_buffer.head, __ring_buffer.data);
		}
	}
	return;
}
