#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

#define AD5940_DATAFIFOTHRESH_UNCHANGE 0x00010000

/**
 * @param Enable                If set to bFALSE, the AD5940 will halt the ongoing measurements.
 * @param FIFOThresh            New FIFO threshold value to set.
 *                              - If set to AD5940_DATAFIFOTHRESH_UNCHANGE, the threshold will remain unchanged.
 * @param BufferSize            Maximum allowable length of the MCU buffer.
 */
typedef struct
{
    BoolFlag Enable;
    uint32_t FIFOThresh;
    uint16_t BufferSize;
} AD5940_DATAFIFOISR_PARA;

/**
 * @param pBuff                 Pointer to the MCU buffer to store FIFO data from the AD5940.
 * @param pBuffCount            Pointer to a variable where the current FIFO count will be stored.
 *                              This count can be retrieved even if an error is raised.
 */
typedef struct
{
    uint32_t *const pBuff;
    uint16_t *const pBuffCount;
} AD5940_DATAFIFOISR_RET;

/**
 * @brief Handles interrupts during measurement on the AD5940.
 *
 * This function processes interrupts by reading data from the AD5940 FIFO buffer, 
 * transferring it to the MCU buffer, and optionally updating the FIFO threshold. 
 * If the new FIFO threshold is set to 0, the AD5940 will shut down measurement.
 * 
 * @return AD5940Err Returns an error code of type `AD5940Err`. A value of 0 indicates success, 
 *                   while any other value represents an error encountered during interrupt handling.
 */
AD5940Err AD5940_DATAFIFOISR(
    AD5940_DATAFIFOISR_PARA para, 
    AD5940_DATAFIFOISR_RET ret
);

#ifdef __cplusplus
}
#endif
