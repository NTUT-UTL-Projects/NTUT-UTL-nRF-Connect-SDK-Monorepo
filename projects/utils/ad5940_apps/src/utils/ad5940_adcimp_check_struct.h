#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_clk.h"

void AD5940_write_DSPCfg_Type(
    AD5940_ADCIMPCheckFreq_Results freq_results,
    DSPCfg_Type *const dsp_cfg
);

void AD5940_write_HSDACCfg_Type(
    AD5940_ADCIMPCheckFreq_Results freq_results,
    HSDACCfg_Type *const hsdac_cfg
);

#ifdef __cplusplus
}
#endif
