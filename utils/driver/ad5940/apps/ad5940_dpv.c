#include "ad5940_dpv.h"

#include "ad5940_utils.h"

AD5940Err AD5940_DPV_PARAM_check(
    AD5940_DPV_PARAM param
)
{
    AD5940Err err;
    float t_interval;
    if(param.e_step <= 0) return AD5940ERR_PARA;
    if(param.e_pulse <= 0) return AD5940ERR_PARA;
    if(param.t_pulse <= 0) return AD5940ERR_PARA;
    if(param.scan_rate <= 0) return AD5940ERR_PARA;
    err = AD5940_DPV_get_t_interval(
        param,
        &t_interval
    );
    if(err) return err;
    if(param.t_pulse >= t_interval) return AD5940ERR_PARA;
    if(!AD5940_DPV_is_inversion(param.inversion)) return AD5940ERR_PARA;
    return AD5940ERR_OK;
}

AD5940Err AD5940_DPV_get_t_interval(
    AD5940_DPV_PARAM param,
    float *const t_interval
)
{
    if(param.e_step <= 0) return AD5940ERR_PARA;
    if(param.scan_rate <= 0) return AD5940ERR_PARA;
    *t_interval = param.e_step / param.scan_rate;
    return AD5940ERR_OK;
}

#define _E_STEP_REAL(param) ((param.e_end > param.e_begin)\
        ? param.e_step\
        : -param.e_step)

static inline float _E_PULSE_REAL(
    AD5940_DPV_PARAM param
)
{
    switch (param.inversion)
    {
    case AD5940_DPV_INVERSION_NONE:
        return (param.e_end > param.e_begin) ? param.e_pulse : -(param.e_pulse);

    case AD5940_DPV_INVERSION_BOTH:
        return (param.e_end > param.e_begin) ? (-param.e_pulse) : param.e_pulse;
    
    case AD5940_DPV_INVERSION_CATHODIC:
        return param.e_pulse;
    
    case AD5940_DPV_INVERSION_ANODIC:
        return (-param.e_pulse);
    }
    return 0.0;
}

#define _T_INTERVAL(param) (param.e_step / param.scan_rate)
static inline uint16_t _STEP_NUMBER(
    AD5940_DPV_PARAM param
) {
    float total = fabsf((param.e_end - param.e_begin) / param.e_step);
    float intpart;
    modff(total, &intpart);
    return (uint16_t)(intpart + 1.0f) * 2;
}

static inline float _INDEX_TO_VOLT(
    AD5940_DPV_PARAM param,
    uint16_t index,
    float e_step_real,
    float e_pulse_real
)
{
    if(index % 2 == 0) return param.e_begin + (e_step_real * ((float) floor(((float) index) / 2.0f)));
    else return param.e_begin + e_pulse_real + (e_step_real * ((float) floor(((float) index) / 2.0f)));
}

AD5940Err AD5940_DPV_get_FIFOCnt(
    const AD5940_DPV_PARAM param,
    uint16_t *const FIFOCnt
)
{
    AD5940Err error = AD5940ERR_OK;

    error = AD5940_DPV_PARAM_check(param);
    if(error != AD5940ERR_OK) return error;

    *FIFOCnt = _STEP_NUMBER(param);

    return error;
}

AD5940Err AD5940_DPV_writeSEQ(
    AD5940_DPV_writeSEQ_Para para,
    uint32_t *const SeqLen
)
{
    AD5940Err error = AD5940ERR_OK;

    error = AD5940_DPV_PARAM_check(para.param);
    if(error != AD5940ERR_OK) return error;

    uint16_t step_number = _STEP_NUMBER(para.param);

	#define SEQLEN_ONESTEP ((para.EnterSleepEn == bTRUE) ? 4L : 3L)  /* How many sequence commands are needed to update LPDAC. */
	uint32_t SeqCmdBuff[SEQLEN_ONESTEP];

	uint32_t current_address = para.SeqRamAddr;

    uint16_t lpdac_dat_12bit;
    uint16_t lpdac_dat_6bit;
    uint32_t lpdac_dat_bits;
	BoolFlag now_is_step = bTRUE;

    float e_current;

    float e_step_real;
    float e_pulse_real;
    
    e_step_real = _E_STEP_REAL(
        para.param
    );

    e_pulse_real = _E_PULSE_REAL(
        para.param
    );

	for(size_t i=0; i<step_number; i++)
	{
        e_current = _INDEX_TO_VOLT(
            para.param,
            i,
            e_step_real,
            e_pulse_real
        ) - para.base_volt;
        error = AD5940_Volt2LPDACData12Bit(
            e_current,
            AD5940_VOLT2DAC_OPTION_NEAREST,
            (e_current > para.base_volt),
            &lpdac_dat_12bit
        );
        error = AD5940_Volt2LPDACData6Bit(
            para.base_volt,
            AD5940_VOLT2DAC_OPTION_NEAREST,
            &lpdac_dat_6bit
        );
        error = AD5940_LPDACDataBits2LPDACDAT(
            lpdac_dat_12bit,
            lpdac_dat_6bit,
            &lpdac_dat_bits
        );
        if(error != AD5940ERR_OK) return error;
		SeqCmdBuff[0] = SEQ_WR(REG_AFE_LPDACDAT0, lpdac_dat_bits);
		SeqCmdBuff[1] = SEQ_WAIT(10); /* !!!NOTE LPDAC need 10 clocks to update data. Before send AFE to sleep state, wait 10 extra clocks */
        AD5940_SEQ_WR_SEQID(
            (now_is_step) ? para.SeqIdPulse : para.SeqIdStep,
            (i == (step_number - 1)) ? para.SeqRamAddr : (current_address + SEQLEN_ONESTEP),
            SEQLEN_ONESTEP,
            SeqCmdBuff + 2
        );
        if(para.EnterSleepEn) SeqCmdBuff[3] = SEQ_SLP();
		AD5940_SEQCmdWrite(current_address, SeqCmdBuff, SEQLEN_ONESTEP);
        now_is_step = (now_is_step == bTRUE) ? bFALSE : bTRUE;
        current_address += SEQLEN_ONESTEP;
	}
    *SeqLen = current_address - para.SeqRamAddr;

    AD5940_WriteSEQID(
        para.SeqIdStep,
        para.SeqRamAddr,
        SEQLEN_ONESTEP
    );

    AD5940_WriteSEQID(
        para.SeqIdPulse,
        para.SeqRamAddr + SEQLEN_ONESTEP,
        SEQLEN_ONESTEP
    );

	return AD5940ERR_OK;
}

AD5940Err AD5940_DPV2WUPT(
    AD5940_DPV2WUPT_Para para,
	WUPTCfg_Type *const wupt_cfg
)
{
    AD5940Err error = AD5940ERR_OK;

    float t_interval;
    error = AD5940_DPV_get_t_interval(
        para.param,
        &t_interval
    );
    if(error != AD5940ERR_OK) return error;

    #define SAMPLE_DELAY 0.001f

    /* Configure Wakeup Timer*/
	wupt_cfg->WuptEn = bTRUE;
	wupt_cfg->WuptEndSeq = WUPTENDSEQ_D;
	wupt_cfg->WuptOrder[0] = para.SeqIdStep;
	wupt_cfg->WuptOrder[1] = para.SeqIdAdc;
	wupt_cfg->WuptOrder[2] = para.SeqIdPulse;
	wupt_cfg->WuptOrder[3] = para.SeqIdAdc;
	wupt_cfg->SeqxSleepTime[para.SeqIdAdc] = 1;     // The minimum value is 1. Do not set it to zero. Set it to 1 will spend 2 32kHz clock.
	wupt_cfg->SeqxWakeupTime[para.SeqIdAdc] = (uint32_t)(para.LFOSCClkFreq * SAMPLE_DELAY) - 1;
	wupt_cfg->SeqxSleepTime[para.SeqIdStep] = 1;    // The minimum value is 1. Do not set it to zero. Set it to 1 will spend 2 32kHz clock.
	wupt_cfg->SeqxWakeupTime[para.SeqIdStep] = (uint32_t)(para.LFOSCClkFreq * (para.param.t_pulse - SAMPLE_DELAY)) - 1;
	wupt_cfg->SeqxSleepTime[para.SeqIdPulse] = 1;   // The minimum value is 1. Do not set it to zero. Set it to 1 will spend 2 32kHz clock.
	wupt_cfg->SeqxWakeupTime[para.SeqIdPulse] = (uint32_t)(para.LFOSCClkFreq * (t_interval - para.param.t_pulse - SAMPLE_DELAY)) - 1;

    return AD5940ERR_OK;
}
