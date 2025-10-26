#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

/**
 * @brief Options for signal inversion in Differential Pulse Voltammetry (DPV) measurements.
 * 
 * This enumeration defines the possible inversion options that can be applied during DPV measurements.
 */
typedef enum {
    AD5940_DPV_INVERSION_NONE,      /**< No inversion is applied. */
    AD5940_DPV_INVERSION_BOTH,      /**< Invert both anodic and cathodic signals. */
    AD5940_DPV_INVERSION_CATHODIC,  /**< Invert only the cathodic signal. */
    AD5940_DPV_INVERSION_ANODIC,    /**< Invert only the anodic signal. */
} AD5940_DPV_INVERSION;

/**
 * @brief Macro to validate if a given inversion option is valid.
 * 
 * This macro checks if the provided option is one of the defined inversion options.
 * 
 * @param option The inversion option to validate.
 * 
 * @return Non-zero if the option is valid, zero otherwise.
 */
#define AD5940_DPV_is_inversion(option) (\
    (option == AD5940_DPV_INVERSION_NONE) || \
    (option == AD5940_DPV_INVERSION_BOTH) || \
    (option == AD5940_DPV_INVERSION_CATHODIC) || \
    (option == AD5940_DPV_INVERSION_ANODIC) \
)

/**
 * @brief Parameters for the AD5940 Electrochemical Differential Pulse Voltammetry (DPV) operation.
 */
typedef struct 
{
    float e_begin;    /**< Starting potential of the scan, in millivolts (mV). */
    float e_end;      /**< Ending potential of the scan, in millivolts (mV). */
    float e_step;    /**< Step potential between measurements, in millivolts (mV). */
    float e_pulse;   /**< Pulse amplitude, in millivolts (mV). */
    float t_pulse;   /**< Pulse duration, in milliseconds (ms). */
    float scan_rate; /**< Rate of potential change during the scan, in millivolts per second (mV/s). */
    AD5940_DPV_INVERSION inversion; /**< Option to invert the signal during measurement. */
}
AD5940_DPV_PARAM;

AD5940Err AD5940_DPV_PARAM_check(
    AD5940_DPV_PARAM param
);

AD5940Err AD5940_DPV_get_t_interval(
    AD5940_DPV_PARAM param,
    float *const t_interval
);

/**
 * @brief Calculates the number of remaining FIFO data points required to complete the 
 *        Differential Pulse Voltammetry (DPV) operation.
 * 
 * @param param         DPV parameter settings.
 * @param FIFOCnt       Pointer to a variable where the calculated remaining FIFO count 
 *                      will be stored.
 * 
 * @return AD5940Err                 Error code indicating success (0) or failure.
 */
AD5940Err AD5940_DPV_get_FIFOCnt(
    const AD5940_DPV_PARAM param,
    uint16_t *const FIFOCnt
);

typedef struct
{
    AD5940_DPV_PARAM param;
    float base_volt;
	uint32_t SeqRamAddr;
    uint32_t SeqIdStep;
    uint32_t SeqIdPulse;
    BoolFlag EnterSleepEn;
} AD5940_DPV_writeSEQ_Para;

AD5940Err AD5940_DPV_writeSEQ(
    AD5940_DPV_writeSEQ_Para para,
    uint32_t *const SeqLen
);

typedef struct
{
    AD5940_DPV_PARAM param;
    uint32_t SeqIdAdc;
    uint32_t SeqIdStep;
    uint32_t SeqIdPulse;
    float LFOSCClkFreq;
} AD5940_DPV2WUPT_Para;

AD5940Err AD5940_DPV2WUPT(
    AD5940_DPV2WUPT_Para para,
	WUPTCfg_Type *const wupt_cfg
);

#ifdef __cplusplus
}
#endif
