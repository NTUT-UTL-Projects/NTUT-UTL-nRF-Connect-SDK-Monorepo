#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

/**
 * If the FIFO count exceeds FIFOSIZE, the SEQ will stop running.
 * To resume, REG_AFE_FIFOCON must be reset.
 */
void AD5940_FIFOCONRst(void);

#ifdef __cplusplus
}
#endif
