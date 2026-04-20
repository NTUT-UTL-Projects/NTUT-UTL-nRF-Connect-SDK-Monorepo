#include "peripheral_nus_received_data.h"

#include <stdatomic.h>
#include <zephyr/kernel.h>

static K_MUTEX_DEFINE(_mutex);
static K_CONDVAR_DEFINE(_condvar);
typedef struct
{
	uint8_t *data;
	uint16_t max_len;
	uint8_t * volatile _Atomic head;
	uint8_t * volatile _Atomic tail;
} _ring_buffer;

static _ring_buffer __ring_buffer;

int peripheral_nus_received_data_ring_buffer_init(uint8_t *const data, uint16_t max_len)
{
	// ring buffer
	__ring_buffer.data = data;
	__ring_buffer.max_len = max_len;
	atomic_store(&__ring_buffer.head, data);
	atomic_store(&__ring_buffer.tail, data);

	// lock
	int err = 0;
    err = k_mutex_init(&_mutex);
    if(err) return err;
    err = k_condvar_init(&_condvar);
    if(err) return err;
    return 0;
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

void peripheral_nus_received_data_get_data(uint8_t *data, uint16_t len)
{
	uint8_t *final = __ring_buffer.data + __ring_buffer.max_len;

	uint8_t *tail = atomic_load(&__ring_buffer.tail);
	uint8_t *target = tail + len;
	if (target >= final)
	{
		uint16_t first_part_len = final - tail;
		memcpy(data, tail, first_part_len);
		memcpy(data + first_part_len, __ring_buffer.data, len - first_part_len);
		
		target = __ring_buffer.data + len - first_part_len;
	}
	else
	{
		memcpy(data, tail, len);
	}
	atomic_store(&__ring_buffer.tail, target);
	return;
}

void peripheral_nus_received_data_wait_for_data(void)
{
	if (atomic_load(&__ring_buffer.head) != atomic_load(&__ring_buffer.tail)) return;
	
    k_mutex_lock(&_mutex, K_FOREVER);
    k_condvar_wait(&_condvar, &_mutex, K_FOREVER);
    k_mutex_unlock(&_mutex);
}

void peripheral_nus_received_data_listener(struct bt_conn *conn, const void *data, uint16_t len, void *ctx)
{
	ARG_UNUSED(conn);
	ARG_UNUSED(ctx);

	// ring buffer
	uint8_t *final = __ring_buffer.data + __ring_buffer.max_len;
	uint8_t *head = atomic_load(&__ring_buffer.head);
	if (head + len > final)
	{
		uint16_t first_part_len = final - head;
		memcpy(head, data, first_part_len);
		memcpy(__ring_buffer.data, (uint8_t *) data + first_part_len, len - first_part_len);
		atomic_store(&__ring_buffer.head, __ring_buffer.data + len - first_part_len);
	}
	else
	{
		memcpy(head, data, len);
		if (head == __ring_buffer.data + __ring_buffer.max_len)
		{
			atomic_store(&__ring_buffer.head, __ring_buffer.data);
		}
		else
		{
			atomic_store(&__ring_buffer.head, head + len);
		}
	}

	// lock
	k_mutex_lock(&_mutex, K_FOREVER);
    k_condvar_broadcast(&_condvar);
    k_mutex_unlock(&_mutex);
	return;
}
