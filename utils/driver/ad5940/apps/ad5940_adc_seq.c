#include "ad5940_adc_seq.h"

AD5940Err AD5940_ADCSEQCmdWrite(
	AD5940_ADCSEQCmdWrite_Para para,
    uint32_t *const SeqLen
)
{
	AD5940Err error = AD5940ERR_OK;

	const uint32_t *pSeqCmd;

	uint32_t WaitClks;
	AD5940_ClksCalculate(&para.clks_cal, &WaitClks);

	AD5940_SEQGenCtrl(bTRUE);
    
	AD5940_AFECtrlS(AFECTRL_ADCPWR | AFECTRL_SINC2NOTCH, bTRUE);
	AD5940_SEQGenInsert(SEQ_WAIT(16*250));  /* wait 250us for reference power up */
	AD5940_AFECtrlS(AFECTRL_ADCCNV, bTRUE);  /* Start ADC convert and DFT */
	AD5940_SEQGenInsert(SEQ_WAIT(WaitClks));  /* wait for first data ready */
	AD5940_AFECtrlS(AFECTRL_ADCPWR | AFECTRL_ADCCNV | AFECTRL_SINC2NOTCH, bFALSE);  /* Stop ADC */

    if(para.EnterSleepEn) AD5940_EnterSleepS();/* Goto hibernate */
    
	/* Sequence end. */
	error = AD5940_SEQGenFetchSeq(&pSeqCmd, SeqLen);
	AD5940_SEQGenCtrl(bFALSE); /* Stop sequencer generator */

    if(error != AD5940ERR_OK) return error;

    SEQInfo_Type SeqInfo;
    SeqInfo.pSeqCmd = pSeqCmd;
	SeqInfo.SeqId = para.SeqId;
    SeqInfo.SeqLen = *SeqLen;
    SeqInfo.SeqRamAddr = para.SeqRamAddr;
	SeqInfo.WriteSRAM = bTRUE;
    AD5940_SEQInfoCfg(&SeqInfo);

	return AD5940ERR_OK;
}
