#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdatomic.h>
#include <stdint.h>

#define VICE_COMMANDS_LIST(X) \
	X(NULL) \
	X(WRITE_AfeCtrlSet) \
	X(WRITE_aferef_cfg) \
	X(WRITE_dsp_cfg) \
	X(WRITE_event) \
    X(WRITE_hsloop_cfg) \
    X(WRITE_lploop_cfg) \
    X(delay) \
    X(shift) \
	X(calibrate_ad5940) \
    X(trigger_ad5940_controller)

typedef enum {
	#define X(name) VICE_COMMANDS_##name,
		VICE_COMMANDS_LIST(X)
	#undef X
} VICE_COMMANDS_ENUM;

#define VICE_COMMANDS_DELAY_UNIT(t) K_MSEC(t)
#define VICE_COMMANDS_DELAY_TYPE uint32_t

#define VICE_COMMANDS_SHIFT_TYPE uint8_t

#ifdef __cplusplus
}
#endif
