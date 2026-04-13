#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "main_commands.h"
#include "vice_commands.h"

#include "hardware.h"

#include <stdatomic.h>
#include <stdint.h>

extern uint32_t SEQGenBuff[];
extern const uint16_t SEQGenBuffLen;

extern AD5940_CONTROLLER_TRIGGER_PARA ad5940_controller_trigger_para;

extern volatile atomic_bool main_commands_flag[];

extern void AD5940_STOP(void);

void main_commands_handler(uint8_t *buffer, uint16_t len);

#ifdef __cplusplus
}
#endif
