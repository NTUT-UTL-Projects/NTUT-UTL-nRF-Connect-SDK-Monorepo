#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

AD5940Err AD5940_SEQ_WR_SEQID(
    const uint8_t SEQID,
    const uint16_t RegAddr, 
    const uint16_t RegLen,
    uint32_t *const sequence_command
);

AD5940Err AD5940_WriteSEQID(
    const uint8_t SEQID,
    const uint16_t RegAddr, 
    const uint16_t RegLen
);

#ifdef __cplusplus
}
#endif
