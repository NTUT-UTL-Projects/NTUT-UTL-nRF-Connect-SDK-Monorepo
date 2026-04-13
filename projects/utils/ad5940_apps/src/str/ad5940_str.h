#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

void AD5940_AfeCtrlSet_to_str(uint32_t AfeCtrlSet, char *const str, uint16_t *const str_len);

void AD5940_AFERefCfg_to_str(AFERefCfg_Type *const aferef_cfg, char *const str, uint16_t *const str_len);

void AD5940_DSPCfg_to_str(DSPCfg_Type *const dsp_cfg, char *const str, uint16_t *const str_len);

void AD5940_LPLoopCfg_to_str(LPLoopCfg_Type *const lploop_cfg, char *const str, uint16_t *const str_len);

void AD5940_HSLoopCfg_to_str(HSLoopCfg_Type *const hsloop_cfg, char *const str, uint16_t *const str_len);

#ifdef __cplusplus
}
#endif
