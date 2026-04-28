#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_clk.h"

    extern const uint32_t AD5940_DFTNUM_to_val_table[];

    extern const uint32_t AD5940_ADCSINC2OSR_to_val_table[];

    extern const uint32_t AD5940_ADCSINC3OSR_to_val_table[];

    extern const float AD5940_ADCPGA_to_val_table[];

    inline float AD5940_fFreqCal(
        const AD5940_ADCIMPCheckFreq_Results *const results,
        uint32_t                                    SamplePeriod
    )
    {
        return (
            results->AdcClkFreq /
            AD5940_DFTNUM_to_val_table[results->freq_params.DftNum] /
            AD5940_ADCSINC2OSR_to_val_table[results->freq_params.ADCSinc2Osr] /
            AD5940_ADCSINC3OSR_to_val_table[results->freq_params.ADCSinc3Osr] *
            SamplePeriod
        );
    }

    inline uint32_t AD5940_ADCcode_base(uint32_t ADCcode, BoolFlag is_raw)
    {
        return (ADCcode & 0xFFFF) - ((is_raw) ? 0x8000 : 0x0000);
    }

    inline float
    AD5940_ADCCode_to_volts(uint32_t ADCcode, uint32_t ADCPga, float VRef1p82)
    {
        return (ADCPga == ADCPGA_1P5 ? 1.835 : VRef1p82) /
               AD5940_ADCPGA_to_val_table[ADCPga] * ADCcode / 0x8000;
    }

    inline float AD5940_volts_to_amperes(float volts, fImpPol_Type RtiaCalValue)
    {
        return (volts / RtiaCalValue.Magnitude);
    }

    inline float AD5940_ADCCode_to_Kelvin(uint32_t ADCcode, uint32_t ADCPga)
    {
#define K 8.13
        return ADCcode / (AD5940_ADCPGA_to_val_table[ADCPga] * K);
#undef K
    }

#ifdef __cplusplus
}
#endif
