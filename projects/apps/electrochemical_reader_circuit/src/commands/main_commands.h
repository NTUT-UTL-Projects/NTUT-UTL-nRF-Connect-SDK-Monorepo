#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

#define MAIN_COMMANDS_LIST(X) \
	X(NULL) \
    X(REBOOT)      \
	X(RESET_AD5940) \
	X(STOP_AD5940)  \
	X(CLEAR_VICE_BUFF) \
	X(WRITE_VICE_BUFF) \
	X(START_VICE_BUFF)

typedef enum {
#define X(name) MAIN_COMMANDS_##name,
        MAIN_COMMANDS_LIST(X)
#undef X
} MAIN_COMMANDS_ENUM;

#ifdef __cplusplus
}
#endif
