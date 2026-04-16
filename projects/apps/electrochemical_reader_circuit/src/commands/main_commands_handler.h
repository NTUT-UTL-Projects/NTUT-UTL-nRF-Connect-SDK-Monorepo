#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "main_commands.h"
#include "vice_commands_handler.h"

#include "ad5940_controller.h"

#include <stdatomic.h>
#include <stdint.h>

typedef struct
{
    uint8_t *buffer;
    uint16_t len;
} main_commands_buffer_ctx;
typedef struct
{
    uint32_t *ad5940_SEQGenBuff;
    uint16_t ad5940_SEQGenBuffLen;
    AD5940_CONTROLLER_RESET_OPTION ad5940_reset_option;

    AD5940_CONTROLLER_TRIGGER_PARA *ad5940_controller_trigger_para;

    void (*ad5940_stop)(void);

    void (*circuit_reboot)(void);

    void (*log)(const char *format, ...);

    main_commands_buffer_ctx *main_commands_buffer_ctx;

    vice_commands_buffer_ctx *vice_commands_buffer_ctx;
} main_commands_ctx;

void main_commands_handler(const main_commands_ctx *const ctx);

#ifdef __cplusplus
}
#endif
