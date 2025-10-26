#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_clk.h"

/**
 * @param results       @see AD5940_ADCIMPCheckFreq_Results.
 * @param SamplePeriod  Sampling period.
 */
typedef struct
{
    AD5940_ADCIMPCheckFreq_Results results;
    uint32_t SamplePeriod;
} AD5940_fFreqCal_Para;

/**
 * Calculates the calibration frequency used in AD5940_LPRtiaCal and AD5940_HSRtiaCal.
 * 
 * @param para  @see AD5940_fFreqCal_Para.
 * @param fFreq Pointer to store the calculated calibration frequency (in Hz).
 * 
 * @return AD5940Err Error code indicating the success or failure of the operation.
 */
AD5940Err AD5940_fFreqCal(
    AD5940_fFreqCal_Para para,
    float *const fFreq
);

/**
 * Retrieves the DftNum value based on the DftNum constant.
 * 
 * @param DftNum        The DftNum configuration constant. @see DFTNUM_Const.
 * @param DftNumVal     Pointer to store the resulting DftNum value.
 * 
 * @return AD5940Err Error code indicating the success or failure of the operation.
 */
AD5940Err AD5940_map_DftNum(
    const uint32_t DftNum, 
    uint16_t *const DftNumVal
);

/**
 * Retrieves the ADC SINC2 oversampling rate based on the given constant.
 * 
 * @param ADCSinc2Osr       The ADC SINC2 oversampling rate constant. @see ADCSINC2OSR_Const.
 * @param ADCSinc2OsrVal    Pointer to store the resulting oversampling rate.
 * 
 * @return AD5940Err Error code indicating the success or failure of the operation.
 */
AD5940Err AD5940_map_ADCSinc2Osr(
    const uint32_t ADCSinc2Osr, 
    uint16_t *const ADCSinc2OsrVal
);

/**
 * Retrieves the ADC SINC3 oversampling rate based on the given constant.
 * 
 * @param ADCSinc3Osr       The ADC SINC3 oversampling rate constant. @see ADCSINC3OSR_Const.
 * @param ADCSinc3OsrVal    Pointer to store the resulting oversampling rate.
 * 
 * @return AD5940Err Error code indicating the success or failure of the operation.
 */
AD5940Err AD5940_map_ADCSinc3Osr(
    const uint32_t ADCSinc3Osr, 
    uint16_t *const ADCSinc3OsrVal
);

/**
 * Retrieves the ADC Programmable Gain Amplifier (PGA) value.
 * 
 * @note Refer to pages 54 to 57 of the datasheet for more details.
 * 
 * @param ADCPga        ADC PGA configuration constant. @see ADCPGA_Const.
 * @param ADCPgaVal     Pointer to store the calculated PGA value (in volts).
 * 
 * @return AD5940Err Error code
 */
AD5940Err AD5940_map_ADCPga(
    const uint32_t ADCPga, 
    float *const ADCPgaVal
);

/**
 * @param ADCcode               The ADC data retrieved from the FIFO.
 * @param ADCPga                ADC PGA gain value. See @ref ADCPGA_Const.
 * @param RtiaCalValue          Pointer to the RTIA calibration result. Available after calibration:
 *                              - @ref AD5940_HSRtiaCal
 *                              - @ref AD5940_LPRtiaCal
 * @param VRef1p82              Reference voltage for the ADC (in volts). Refer to datasheet page 87.
 */
typedef struct
{
    uint32_t ADCcode;
    uint32_t ADCPga;
    fImpPol_Type RtiaCalValue;
    float VRef1p82;
} AD5940_ADCCode2Current_Para;

/**
 * Converts ADC data to current values.
 * ADCcode and currents allow be the same pointer.
 * 
 * @param para      @see AD5940_ADCCode2Current_Para.
 * @param currents  Pointer to store the resulting current values (in amperes).
 * 
 * @return AD5940Err Error code indicating the success or failure of the operation.
 */
AD5940Err AD5940_ADCCode2Current(
    AD5940_ADCCode2Current_Para para,
    float *const current
);

/**
 * @param ADCcode               The ADC data retrieved from the FIFO.
 * @param ADCPga                ADC PGA gain configuration constant. See @ref ADCPGA_Const.
 */
typedef struct
{
    uint32_t ADCcode;
    uint32_t ADCPga;
} AD5940_ADCCode2Temperature_Para;

/**
 * Converts ADC data to temperature values.
 * ADCcode and temperatures allow be the same pointer.
 * 
 * @param para          @see AD5940_ADCCode2Temperature_Para.
 * @param temperatures  Pointer to store the resulting temperature values (in degrees Celsius).
 * 
 * @return AD5940Err Error code indicating the success or failure of the operation.
 */
AD5940Err AD5940_ADCCode2Temperature(
    AD5940_ADCCode2Temperature_Para para,
    float *const temperature
);

#ifdef __cplusplus
}
#endif
