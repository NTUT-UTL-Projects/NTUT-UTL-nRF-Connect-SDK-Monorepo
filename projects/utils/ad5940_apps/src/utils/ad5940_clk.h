#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

typedef struct {
    float freq;                     // 
    BoolFlag HFXTALEn;              // @see CLKCfg_Type.
    BoolFlag LFOSCEn;               // @see CLKCfg_Type.
} AD5940_ADCIMPCheckFreq_Para;

typedef struct {
    FreqParams_Type freq_params;    // @see FreqParams_Type.
    CLKCfg_Type clk_cfg;            // @see CLKCfg_Type.
    BoolFlag HanWinEn;              // 
    uint32_t AfePwr;                // @see AD5940_AFEPwrBW.
    uint32_t AfeBw;                 // @see AD5940_AFEPwrBW.
    uint32_t HsDacUpdateRate;       // @see HSDACCfg_Type.
    uint32_t ADCRate;               // @see ADCRATE_Const.
    float AdcClkFreq;               // @see ADC clock frequency value (in Hz). This term is referenced throughout the datasheet.
    float SysClkFreq;               // @see system clock frequency value (in Hz). This term is referenced throughout the datasheet.
    float RatioSys2AdcClk;          // @see SysClkFreq / AdcClkFreq.
} AD5940_ADCIMPCheckFreq_Results;

/**
 * Refer to https://github.com/analogdevicesinc/ad5940-examples/blob/master/examples/AD5940_Impedance_Adjustable_with_frequency/Impedance.c, access date: 2025-08-11 11:07
 * 
 * Refer to HIGH SPEED DAC CIRCUITS, p. 43 of the datasheet.
 * 
 * Refer to AVOIDING INCOHERENCY ERRORS BETWEEN EXCITATION AND MEASUREMENT FREQUENCIES DURING IMPEDANCE MEASUREMENTS, p. 45 of the datasheet.
 * 
 * Refer to HIGH PERFORMANCE ADC CIRCUIT, p. 54 of the datasheet.
 * 
 * Refer to Table 41. Digital Filter Settings to Support Simultaneous 50 Hz/60 Hz Mains Rejection, p. 57 of the datasheet.
 * 
 * Refer to POWER MODES, p. 129 of the datasheet.
 * 
 * Refer to CLOCKING ARCHITECTURE, p. 132 of the datasheet.
 * 
 * Parameters:
 * @param para      @see AD5940_ADCIMPCheckFreq_Para.
 * @param results   @see AD5940_ADCIMPCheckFreq_Results.
 * 
 * @return AD5940Err    Error code indicating the success or failure of the operation.
 */
AD5940Err AD5940_ADCIMPCheckFreq(
    AD5940_ADCIMPCheckFreq_Para para,
    AD5940_ADCIMPCheckFreq_Results *const results
);

#ifdef __cplusplus
}
#endif
