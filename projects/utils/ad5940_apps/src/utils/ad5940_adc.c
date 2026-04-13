#include "ad5940_adc.h"

/**
 * @brief Retrieves the DftNum value based on the DftNum constant.
 * 
 * @param DFTNUM The DftNum configuration constant. @see DFTNUM_Const.
 */
const uint32_t AD5940_DFTNUM_to_val_table[] = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};

/**
 * @brief Retrieves the ADC SINC2 oversampling rate based on the given constant.
 * 
 * @param ADCSinc2Osr The ADC SINC2 oversampling rate constant. @see ADCSINC2OSR_Const.
 */
const uint32_t AD5940_ADCSINC2OSR_to_val_table[] = {1, 22, 44, 89, 178, 267, 533, 640, 667, 800, 889, 1067, 1333};

/**
 * @brief Retrieves the ADC SINC3 oversampling rate based on the given constant.
 * 
 * @param ADCSinc3Osr The ADC SINC3 oversampling rate constant. @see ADCSINC3OSR_Const.
 */
const uint32_t AD5940_ADCSINC3OSR_to_val_table[] = {2, 4, 5};

/**
 * @brief Retrieves the ADC Programmable Gain Amplifier (PGA) value.
 * 
 * @note Refer to pages 54 to 57 of the datasheet for more details.
 * 
 * @param ADCPga        ADC PGA configuration constant. @see ADCPGA_Const.
 */
const float AD5940_ADCPGA_to_val_table[] = {1, 1.5, 2, 4, 9};

/**
 * Calculates the calibration frequency used in AD5940_LPRtiaCal and AD5940_HSRtiaCal.
 * 
 * @param results       @see AD5940_ADCIMPCheckFreq_Results.
 * @param SamplePeriod  Sampling period.
 * 
 * @return The calculated calibration frequency (in Hz).
 */
extern inline float AD5940_fFreqCal(const AD5940_ADCIMPCheckFreq_Results *const results, uint32_t SamplePeriod);

/**
 * @brief Convert ADC code for data postprocessing.
 * 
 * @param ADCcode The ADC data retrieved from the FIFO.
 * @param is_raw  Refer to pages 57, 58, 60, 91 and 94 of the datasheet
 *                - Figure 34. Postprocessing Filter Options: Shows the option should be raw ADC code (DATAFIFO_SINC3 or DATAFIFO_SINC2).
 *                - `DATAFIFOSRCSEL`: shows how to selects the source for the data FIFO.
 * 
 * @note The `ad5940.h` file provides an `AD5940_ReadAfeResult` function for reading various DATAFIFOSRC.
 */
extern inline uint32_t AD5940_ADCcode_base(uint32_t ADCcode, BoolFlag is_raw);

/**
 * @brief Converts ADC data to current values.
 * 
 * @param ADCcode See @ref AD5940_ADCcode_base
 * @param ADCPga @see ADCPGA_Const.
 * @param VRef1p82 Reference voltage for the ADC (in volts). Refer to datasheet page 87.
 * 
 * @note
 * Refer to pages 56 of the datasheet.
 * 
 * @return The resulting voltage values (in volts).
 */
extern inline float AD5940_ADCCode_to_volts(uint32_t ADCcode, uint32_t ADCPga, float VRef1p82);

/**
 * @brief Converts ADC data to current values.
 * 
 * @param RtiaCalValue Pointer to the RTIA calibration result. Available after calibration:
 *                     - @ref AD5940_HSRtiaCal
 *                     - @ref AD5940_LPRtiaCal
 * 
 * @return The resulting current values (in amperes).
 */
extern inline float AD5940_volts_to_amperes(float volts, fImpPol_Type RtiaCalValue);

/**
 * @brief Converts ADC data to temperature values.
 * 
 * @param ADCcode See @ref AD5940_ADCcode_base
 * @param ADCPga  ADC PGA gain configuration constant. See @ref ADCPGA_Const.
 * 
 * @note
 * Refer to pages 57 of the datasheet.
 * 
 * @return The resulting temperature values (in Kelvin).
 */
extern inline float AD5940_ADCCode_to_Kelvin(uint32_t ADCcode, uint32_t ADCPga);
