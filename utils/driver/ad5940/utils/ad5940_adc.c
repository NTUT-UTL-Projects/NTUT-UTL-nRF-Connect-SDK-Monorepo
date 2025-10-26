#include "ad5940_adc.h"

static const uint32_t dft_table[] = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};
static const uint32_t sinc2osr_table[] = {22,44,89,178,267,533,640,667,800,889,1067,1333,0};
static const uint32_t sinc3osr_table[] = {5,4,2,0};

AD5940Err AD5940_fFreqCal(
    AD5940_fFreqCal_Para para,
    float *const fFreq
)
{
    AD5940Err error = AD5940ERR_OK;

    uint16_t DftNumVal;
    uint16_t ADCSinc2OsrVal;
    uint16_t ADCSinc3OsrVal;
    error = AD5940_map_DftNum(
        para.results.freq_params.DftNum, 
        &DftNumVal
    );
    if(error != AD5940ERR_OK) return error;
    error = AD5940_map_ADCSinc2Osr(
        para.results.freq_params.ADCSinc2Osr, 
        &ADCSinc2OsrVal
    );
    if(error != AD5940ERR_OK) return error;
    error = AD5940_map_ADCSinc3Osr(
        para.results.freq_params.ADCSinc3Osr, 
        &ADCSinc3OsrVal
    );
    if(error != AD5940ERR_OK) return error;

    *fFreq = para.results.AdcClkFreq / DftNumVal / ADCSinc2OsrVal / ADCSinc3OsrVal * para.SamplePeriod;
    return AD5940ERR_OK;
}

AD5940Err AD5940_map_DftNum(
    const uint32_t DftNum, 
    uint16_t *const DftNumVal
)
{
    if(DftNum > 12) return AD5940ERR_PARA;
    *DftNumVal = dft_table[DftNum];
    return AD5940ERR_OK;
}

AD5940Err AD5940_map_ADCSinc2Osr(
    const uint32_t ADCSinc2Osr, 
    uint16_t *const ADCSinc2OsrVal
)
{
    if(!IS_ADCSINC2OSR(ADCSinc2Osr)) return AD5940ERR_PARA;
    *ADCSinc2OsrVal = sinc2osr_table[ADCSinc2Osr];
    return AD5940ERR_OK;
}

AD5940Err AD5940_map_ADCSinc3Osr(
    const uint32_t ADCSinc3Osr, 
    uint16_t *const ADCSinc3OsrVal
)
{
    if(!IS_ADCSINC3OSR(ADCSinc3Osr)) return AD5940ERR_PARA;
    *ADCSinc3OsrVal = sinc3osr_table[ADCSinc3Osr];
    return AD5940ERR_OK;
}

AD5940Err AD5940_map_ADCPga(
    const uint32_t ADCPga,
    float *const ADCPgaVal
)
{
    switch (ADCPga)
    {
    case ADCPGA_1:
        *ADCPgaVal = 1.0f;
        return AD5940ERR_OK;
    
    case ADCPGA_1P5:
        *ADCPgaVal = 1.5f;
        return AD5940ERR_OK;
    
    case ADCPGA_2:
        *ADCPgaVal = 2.0f;
        return AD5940ERR_OK;
    
    case ADCPGA_4:
        *ADCPgaVal = 4.0f;
        return AD5940ERR_OK;
    
    case ADCPGA_9:
        *ADCPgaVal = 9.0f;
        return AD5940ERR_OK;

    default:
        break;
    }
    return AD5940ERR_PARA;
}

AD5940Err AD5940_ADCCode2Current(
    AD5940_ADCCode2Current_Para para,
    float *const current
)
{
    // TODO (Refer to DATAFIFOSRCSEL at page 94 of the datasheet.)
    if(!IS_ADCPGA(para.ADCPga)) return AD5940ERR_PARA;
    *current = (
        (
            AD5940_ADCCode2Volt(
                para.ADCcode & 0xFFFF, 
                para.ADCPga,
                para.VRef1p82
            ) 
            / para.RtiaCalValue.Magnitude
        )
    );
    return AD5940ERR_OK;
}

AD5940Err AD5940_ADCCode2Temperature(
    AD5940_ADCCode2Temperature_Para para,
    float *const temperature
)
{
    #define K 8.13f
    float adcpga_float;
    AD5940Err error = AD5940_map_ADCPga(
        para.ADCPga,
        &adcpga_float
    );
    if(error != AD5940ERR_OK) return AD5940ERR_PARA;
    *temperature = para.ADCcode & 0xffff;
    *temperature -= 0x8000;	// data from SINC2 is added 0x8000, while data from register TEMPSENSDAT has no 0x8000 offset.
    *temperature = (*temperature / K / adcpga_float - 273.15f);
    return AD5940ERR_OK;
}
