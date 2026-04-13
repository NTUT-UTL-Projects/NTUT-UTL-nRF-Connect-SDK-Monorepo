#include "ad5940_seq.h"

AD5940Err AD5940_SEQ_WR_SEQID(
    const uint8_t SEQID,
    const uint16_t RegAddr, 
    const uint16_t RegLen,
    uint32_t *const pSeqCmd
)
{
    switch (SEQID)
    {
    case SEQID_0:
        *pSeqCmd = SEQ_WR(
            REG_AFE_SEQ0INFO,
            (RegAddr << BITP_AFE_SEQ0INFO_ADDR) | (RegLen << BITP_AFE_SEQ0INFO_LEN)
        );
        break;
    case SEQID_1:
        *pSeqCmd = SEQ_WR(
            REG_AFE_SEQ1INFO,
            (RegAddr << BITP_AFE_SEQ1INFO_ADDR) | (RegLen << BITP_AFE_SEQ1INFO_LEN)
        );
        break;
    case SEQID_2:
        *pSeqCmd = SEQ_WR(
            REG_AFE_SEQ2INFO,
            (RegAddr << BITP_AFE_SEQ2INFO_ADDR) | (RegLen << BITP_AFE_SEQ2INFO_LEN)
        );
        break;
    case SEQID_3:
        *pSeqCmd = SEQ_WR(
            REG_AFE_SEQ3INFO,
            (RegAddr << BITP_AFE_SEQ3INFO_ADDR) | (RegLen << BITP_AFE_SEQ3INFO_LEN)
        );
        break;
    default:
        return AD5940ERR_PARA;
    }
    return AD5940ERR_OK;
}

AD5940Err AD5940_WriteSEQID(
    const uint8_t SEQID,
    const uint16_t RegAddr, 
    const uint16_t RegLen
)
{
    switch (SEQID)
    {
    case SEQID_0:
        AD5940_WriteReg(
            REG_AFE_SEQ0INFO,
            (RegAddr << BITP_AFE_SEQ0INFO_ADDR) | (RegLen << BITP_AFE_SEQ0INFO_LEN)
        );
        break;
    case SEQID_1:
        AD5940_WriteReg(
            REG_AFE_SEQ1INFO,
            (RegAddr << BITP_AFE_SEQ1INFO_ADDR) | (RegLen << BITP_AFE_SEQ1INFO_LEN)
        );
        break;
    case SEQID_2:
        AD5940_WriteReg(
            REG_AFE_SEQ2INFO,
            (RegAddr << BITP_AFE_SEQ2INFO_ADDR) | (RegLen << BITP_AFE_SEQ2INFO_LEN)
        );
        break;
    case SEQID_3:
        AD5940_WriteReg(
            REG_AFE_SEQ3INFO,
            (RegAddr << BITP_AFE_SEQ3INFO_ADDR) | (RegLen << BITP_AFE_SEQ3INFO_LEN)
        );
        break;
    default:
        return AD5940ERR_PARA;
    }
    return AD5940ERR_OK;
}
