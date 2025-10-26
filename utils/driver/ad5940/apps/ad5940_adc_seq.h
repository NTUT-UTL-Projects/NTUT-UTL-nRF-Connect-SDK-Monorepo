#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

#define AD5940_DATAFIFOTHRESH_UNCHANGE 0x00010000

typedef struct
{
    ClksCalInfo_Type clks_cal;
    BoolFlag EnterSleepEn;
    uint32_t SeqId;
	uint32_t SeqRamAddr;
} AD5940_ADCSEQCmdWrite_Para;

/**
* @brief Generate ADC control sequence and write the commands to SRAM.
* @return return error code.
*/
AD5940Err AD5940_ADCSEQCmdWrite(
	AD5940_ADCSEQCmdWrite_Para para,
    uint32_t *const SeqLen
);

#ifdef __cplusplus
}
#endif
