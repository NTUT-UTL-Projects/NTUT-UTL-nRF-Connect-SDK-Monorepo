#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_clk.h"

typedef struct
{
    AD5940_ADCIMPCheckFreq_Results freq_results;
    ADCBaseCfg_Type ADCBaseCfg;
    uint32_t PGACalType;
    int32_t TimeOut10us;
    float VRef1p11;
    float VRef1p82;
} AD5940_write_ADCPGACal_Type_Para;

void AD5940_write_ADCPGACal_Type(
    AD5940_write_ADCPGACal_Type_Para para,
    ADCPGACal_Type *const cal
);

typedef struct
{
    AD5940_ADCIMPCheckFreq_Results freq_results;
    ADCBaseCfg_Type ADCBaseCfg;
    uint16_t DacData12Bit;
    uint16_t DacData6Bit;
    LPLoopCfg_Type lploop_cfg;
    int32_t SettleTime10us;
    int32_t TimeOut10us;
} AD5940_write_LPTIAOffsetCal_Type_Para;

void AD5940_write_LPTIAOffsetCal_Type(
    AD5940_write_LPTIAOffsetCal_Type_Para para,
    LPTIAOffsetCal_Type *const cal
);

typedef struct
{
    AD5940_ADCIMPCheckFreq_Results freq_results;
    uint32_t bPolarResult;
    BoolFlag bWithCtia;
    DFTCfg_Type DftCfg;
    float fFreq;
    float fRcal;
    LPAmpCfg_Type LpAmpCfg;
} AD5940_write_LPRTIACal_Type_Para;

void AD5940_write_LPRTIACal_Type(
    AD5940_write_LPRTIACal_Type_Para para,
    LPRTIACal_Type *const cal
);

typedef struct
{
    AD5940_ADCIMPCheckFreq_Results freq_results;
    uint32_t bPolarResult;
    BoolFlag bWithCtia;
    DFTCfg_Type DftCfg;
    float fFreq;
    float fRcal;
    HSTIACfg_Type HsTiaCfg;
} AD5940_write_HSRTIACal_Type_Para;

void AD5940_write_HSRTIACal_Type(
    AD5940_write_HSRTIACal_Type_Para para,
    HSRTIACal_Type *const cal
);

typedef struct
{
    AD5940_ADCIMPCheckFreq_Results freq_results;
    float ADCRefVolt;
    LPDACCfg_Type LpDacCfg;
    int32_t SettleTime10us;
    int32_t TimeOut10us;
} AD5940_write_LPDACCal_Type_Para;

void AD5940_write_LPDACCal_Type(
    AD5940_write_LPDACCal_Type_Para para,
    LPDACCal_Type *const cal
);

#ifdef __cplusplus
}
#endif
