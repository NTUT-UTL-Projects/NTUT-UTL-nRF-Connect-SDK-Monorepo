#include "ad5940_controller_trigger.h"
#include "ad5940_apps.h"

AD5940Err AD5940_controller_trigger(
    AD5940_CONTROLLER_TRIGGER_PARA *const para
)
{
	// ========================================
    /* Wakeup AFE by read register, read 10 times at most */
    if(AD5940_WakeUp(10) > 10) return AD5940ERR_WAKEUP;  /* Wakeup Failed */

	// ========================================
    /**
     * Before the application begins, INTC are used for configuring parameters.
     * Therefore, they should not be used during the configuration process itself.
     */
    AD5940_GPIOClr();

    /* Configure sequencer and stop it */
    SEQCfg_Type seq_cfg = {
        .SeqBreakEn = bFALSE,
        .SeqCntCRCClr = bTRUE,
        .SeqEnable = bFALSE,            // Refer to page 93 of the datasheet, it stop the SEQ.
        .SeqIgnoreEn = bTRUE,
        .SeqMemSize = SEQMEMSIZE_4KB,
        .SeqWrTimer = 0,
    };
    AD5940_SEQCfg(&seq_cfg);

    /* Reconfigure FIFO */
    FIFOCfg_Type fifo_cfg = {
        .FIFOEn = bFALSE,               // Refer to page 94 of the datasheet, it reset the FIFO pointer.
        .FIFOMode = FIFOMODE_FIFO,      // Refer to page 94 of the datasheet
        .FIFOSize = FIFOSIZE_2KB,       // Refer to page 94 of the datasheet,
                                        // The strategy is to read the FIFO when there is data in the FIFO,
                                        // so there is no need to keep a large FIFOSIZE.
        .FIFOSrc = 0,                   // This will be set later.
        .FIFOThresh = 0,                // This will be set later.
    };
    AD5940_FIFOCfg(&fifo_cfg);

    // ========================================
    // AD5940 task
    AD5940_AFECtrlS(AFECTRL_ALL, bFALSE);  /* Init all to disable state */

    AD5940_REFCfgS(&(para->aferef_cfg));
    AD5940_LPLoopCfgS(&(para->lploop_cfg));
    AD5940_HSLoopCfgS(&(para->hsloop_cfg));
    AD5940_DSPCfgS(&(para->dsp_cfg));

    /* Enable all of them. They are automatically turned off during hibernate mode to save power */
    AD5940_AFECtrlS(para->AfeCtrlSet, bTRUE);

    // Ensure it is cleared as ad5940.c relies on the INTC flag as well.
    AD5940_INTCClrFlag(AFEINTSRC_ALLINT);

    #define ADC_SEQID SEQID_0
    #define CV_0_SEQID SEQID_1
    #define CV_1_SEQID SEQID_2
    #define DPV_STEP_SEQID SEQID_1
    #define DPV_PULSE_SEQID SEQID_2

    // ========================================
    // Write SEQ
    switch (para->event.event)
    {
    case AD5940_CONTROLLER_EVENT_CA:
    {
        uint32_t SeqRamAddr = 0;
        uint32_t SeqLen;

        ClksCalInfo_Type clks_cal;
        AD5940_write_ClksCalInfo_Type(
            (AD5940_write_ClksCalInfo_Type_Para) {
                .DataCount = 1,
                .DataType = para->DataType,
                .dsp_cfg = para->dsp_cfg,
                .RatioSys2AdcClk = para->RatioSys2AdcClk,
            },
            &clks_cal
        );

        AD5940_ADCSEQCmdWrite(
            (AD5940_ADCSEQCmdWrite_Para) {
                .clks_cal = clks_cal,
                .EnterSleepEn = para->EnterSleepEn,
                .TemperatureEn = bFALSE,
                .AfeCtrlSet = AFECTRL_SINC2NOTCH,
                .SeqId = ADC_SEQID,
                .SeqRamAddr = SeqRamAddr,
            },
            &SeqLen
        );
        break;
    }
    case AD5940_CONTROLLER_EVENT_CV:
    {
        uint32_t SeqRamAddr = 0;
        uint32_t SeqLen;

        ClksCalInfo_Type clks_cal;
        AD5940_write_ClksCalInfo_Type(
            (AD5940_write_ClksCalInfo_Type_Para) {
                .DataCount = 1,
                .DataType = para->DataType,
                .dsp_cfg = para->dsp_cfg,
                .RatioSys2AdcClk = para->RatioSys2AdcClk,
            },
            &clks_cal
        );

        AD5940_ADCSEQCmdWrite(
            (AD5940_ADCSEQCmdWrite_Para) {
                .clks_cal = clks_cal,
                .EnterSleepEn = para->EnterSleepEn,
                .TemperatureEn = bFALSE,
                .AfeCtrlSet = AFECTRL_SINC2NOTCH,
                .SeqId = ADC_SEQID,
                .SeqRamAddr = SeqRamAddr,
            },
            &SeqLen
        );
        SeqRamAddr += SeqLen;

        ELECTROCHEMICAL_DEV_KIT_ENERGY_TYPE e_current;
        uint32_t lpdac_dat_12bit;
        uint32_t lpdac_dat_6bit;
        uint32_t lpdac_dat_bits;
	    BoolFlag now_is_0 = bTRUE;
        #define SEQLEN_ONESTEP ((para->EnterSleepEn == bTRUE) ? 4L : 3L)  /* How many sequence commands are needed to update LPDAC. */
        uint32_t SeqCmdBuff[SEQLEN_ONESTEP];
        uint32_t current_address = SeqRamAddr;
        ELECTROCHEMICAL_DEV_KIT_NUMBER_TYPE n_step_total = ELECTROCHEMICAL_DEV_KIT_cv_get_n_step_total(&para->event.param.cv.cv);
        for(size_t i=0; i<n_step_total; i++)
        {
            e_current = ELECTROCHEMICAL_DEV_KIT_cv_get_e_at_index(&para->event.param.cv.cv, i);
            lpdac_dat_6bit = AD5940_volts_to_LPDACData6Bit(
                e_current
            );
            lpdac_dat_12bit = AD5940_volts_to_LPDACData12Bit(
                e_current,
                lpdac_dat_6bit
            );
            lpdac_dat_bits = AD5940_LPDACDataBits_to_LPDACDAT(
                lpdac_dat_12bit,
                lpdac_dat_6bit
            );
            SeqCmdBuff[0] = SEQ_WR(REG_AFE_LPDACDAT0, lpdac_dat_bits);
            SeqCmdBuff[1] = SEQ_WAIT(10); /* !!!NOTE LPDAC need 10 clocks to update data. Before send AFE to sleep state, wait 10 extra clocks */
            BoolFlag is_final_step = (i == (n_step_total - 1)) ? bTRUE : bFALSE;
            AD5940_SEQ_WR_SEQID(
                (now_is_0) ? CV_1_SEQID : CV_0_SEQID,
                is_final_step ? SeqRamAddr : (current_address + SEQLEN_ONESTEP),
                SEQLEN_ONESTEP,
                SeqCmdBuff + 2
            );
            if(para->EnterSleepEn) SeqCmdBuff[3] = SEQ_SLP();
            AD5940_SEQCmdWrite(current_address, SeqCmdBuff, SEQLEN_ONESTEP);
            now_is_0 = (now_is_0 == bTRUE) ? bFALSE : bTRUE;
            current_address += SEQLEN_ONESTEP;
        }
        SeqLen = current_address - SeqRamAddr;
        AD5940_WriteSEQID(
            CV_0_SEQID,
            SeqRamAddr,
            SEQLEN_ONESTEP
        );
        AD5940_WriteSEQID(
            CV_1_SEQID,
            SeqRamAddr + SEQLEN_ONESTEP,
            SEQLEN_ONESTEP
        );
        break;
    }
    case AD5940_CONTROLLER_EVENT_DPV:
    {
        uint32_t SeqRamAddr = 0;
        uint32_t SeqLen;

        ClksCalInfo_Type clks_cal;
        AD5940_write_ClksCalInfo_Type(
            (AD5940_write_ClksCalInfo_Type_Para) {
                .DataCount = 1,
                .DataType = para->DataType,
                .dsp_cfg = para->dsp_cfg,
                .RatioSys2AdcClk = para->RatioSys2AdcClk,
            },
            &clks_cal
        );

        AD5940_ADCSEQCmdWrite(
            (AD5940_ADCSEQCmdWrite_Para) {
                .clks_cal = clks_cal,
                .EnterSleepEn = para->EnterSleepEn,
                .TemperatureEn = bFALSE,
                .AfeCtrlSet = AFECTRL_SINC2NOTCH,
                .SeqId = ADC_SEQID,
                .SeqRamAddr = SeqRamAddr,
            },
            &SeqLen
        );
        SeqRamAddr += SeqLen;

        ELECTROCHEMICAL_DEV_KIT_ENERGY_TYPE e_current;
        uint32_t lpdac_dat_12bit;
        uint32_t lpdac_dat_6bit;
        uint32_t lpdac_dat_bits;
	    BoolFlag now_is_step = bTRUE;
        #define SEQLEN_ONESTEP ((para->EnterSleepEn == bTRUE) ? 4L : 3L)  /* How many sequence commands are needed to update LPDAC. */
        uint32_t SeqCmdBuff[SEQLEN_ONESTEP];
        uint32_t current_address = SeqRamAddr;
        ELECTROCHEMICAL_DEV_KIT_NUMBER_TYPE n_step_total = ELECTROCHEMICAL_DEV_KIT_dpv_get_n_step_total(&para->event.param.dpv);
        for(size_t i=0; i<n_step_total; i++)
        {
            e_current = ELECTROCHEMICAL_DEV_KIT_dpv_get_e_at_index(&para->event.param.dpv, i, ELECTROCHEMICAL_DEV_KIT_dpv_selection_step_pulse);
            lpdac_dat_6bit = AD5940_volts_to_LPDACData6Bit(
                e_current
            );
            lpdac_dat_12bit = AD5940_volts_to_LPDACData12Bit(
                e_current,
                lpdac_dat_6bit
            );
            lpdac_dat_bits = AD5940_LPDACDataBits_to_LPDACDAT(
                lpdac_dat_12bit,
                lpdac_dat_6bit
            );
            SeqCmdBuff[0] = SEQ_WR(REG_AFE_LPDACDAT0, lpdac_dat_bits);
            SeqCmdBuff[1] = SEQ_WAIT(10); /* !!!NOTE LPDAC need 10 clocks to update data. Before send AFE to sleep state, wait 10 extra clocks */
            BoolFlag is_final_step = (i == (n_step_total - 1)) ? bTRUE : bFALSE;
            AD5940_SEQ_WR_SEQID(
                (now_is_step) ? DPV_PULSE_SEQID : DPV_STEP_SEQID,
                is_final_step ? SeqRamAddr : (current_address + SEQLEN_ONESTEP),
                SEQLEN_ONESTEP,
                SeqCmdBuff + 2
            );
            if(para->EnterSleepEn) SeqCmdBuff[3] = SEQ_SLP();
            AD5940_SEQCmdWrite(current_address, SeqCmdBuff, SEQLEN_ONESTEP);
            now_is_step = (now_is_step == bTRUE) ? bFALSE : bTRUE;
            current_address += SEQLEN_ONESTEP;
        }
        SeqLen = current_address - SeqRamAddr;
        AD5940_WriteSEQID(
            DPV_STEP_SEQID,
            SeqRamAddr,
            SEQLEN_ONESTEP
        );
        AD5940_WriteSEQID(
            DPV_PULSE_SEQID,
            SeqRamAddr + SEQLEN_ONESTEP,
            SEQLEN_ONESTEP
        );
        break;
    }
    case AD5940_CONTROLLER_EVENT_TEMPERATURE:
    {
        uint32_t SeqRamAddr = 0;
        uint32_t SeqLen;

        ClksCalInfo_Type clks_cal;
        AD5940_write_ClksCalInfo_Type(
            (AD5940_write_ClksCalInfo_Type_Para) {
                .DataCount = 1,
                .DataType = para->DataType,
                .dsp_cfg = para->dsp_cfg,
                .RatioSys2AdcClk = para->RatioSys2AdcClk,
            },
            &clks_cal
        );

        AD5940_ADCSEQCmdWrite(
            (AD5940_ADCSEQCmdWrite_Para) {
                .clks_cal = clks_cal,
                .EnterSleepEn = para->EnterSleepEn,
                .TemperatureEn = bTRUE,
                .AfeCtrlSet = 0,
                .SeqId = ADC_SEQID,
                .SeqRamAddr = SeqRamAddr,
            },
            &SeqLen
        );
        break;
    }
    default:
        return AD5940ERR_PARA;
    }

    BoolFlag AFEINTC_0_En, AFEINTC_1_En;
    AD5940_FuncSet2AFEINTCx(
        para->gpio_cfg.FuncSet,
        &AFEINTC_0_En,
        &AFEINTC_1_En
    );
    AD5940_AFEINTCxCfg(
        AFEINTC_0_En,
        AFEINTC_1_En,
        AFEINTSRC_DATAFIFOTHRESH
    );
    
    AD5940_AGPIOCfg(&(para->gpio_cfg));

    /* Configure FIFO and Sequencer for normal Amperometric Measurement */
    uint32_t FifoThresh = 1;    // Set to 1, so that when there is data in FIFO, GPIO will be triggered.
    AD5940_FIFOThrshSet(FifoThresh);
    AD5940_FIFOCtrlS(para->FifoSrc, bTRUE);

    AD5940_SEQCtrlS(bTRUE);

    // ========================================
    // Trigger WUPT
    switch (para->event.event)
    {
    case AD5940_CONTROLLER_EVENT_CA:
    {
        /* Configure Wakeup Timer*/
        WUPTCfg_Type wupt_cfg;
        wupt_cfg.WuptEn = bTRUE;
        wupt_cfg.WuptEndSeq = WUPTENDSEQ_A;
        wupt_cfg.WuptOrder[0] = ADC_SEQID;
        wupt_cfg.SeqxSleepTime[ADC_SEQID] = 1; /* The minimum value is 1. Do not set it to zero. Set it to 1 will spend 2 32kHz clock. */
        wupt_cfg.SeqxWakeupTime[ADC_SEQID] = (uint32_t)(para->LFOSCClkFreq * para->event.param.ca.t_interval) - 1;
        AD5940_WUPTCfg(&wupt_cfg);
        break;
    }
    case AD5940_CONTROLLER_EVENT_CV:
    {
        WUPTCfg_Type wupt_cfg;

        /* Configure Wakeup Timer */
        #define SAMPLE_DELAY 0.001f
        wupt_cfg.WuptEn = bTRUE;
        wupt_cfg.WuptEndSeq = WUPTENDSEQ_D;
        wupt_cfg.WuptOrder[0] = CV_0_SEQID;
        wupt_cfg.WuptOrder[1] = ADC_SEQID;
        wupt_cfg.WuptOrder[2] = CV_1_SEQID;
        wupt_cfg.WuptOrder[3] = ADC_SEQID;
        wupt_cfg.SeqxSleepTime[ADC_SEQID] = 1;     // The minimum value is 1. Do not set it to zero. Set it to 1 will spend 2 32kHz clock.
        wupt_cfg.SeqxWakeupTime[ADC_SEQID] = (uint32_t)(para->LFOSCClkFreq * SAMPLE_DELAY) - 1;
        wupt_cfg.SeqxSleepTime[CV_0_SEQID] = 1;    // The minimum value is 1. Do not set it to zero. Set it to 1 will spend 2 32kHz clock.
        wupt_cfg.SeqxWakeupTime[CV_0_SEQID] = (uint32_t)(para->LFOSCClkFreq * (para->event.param.cv.cv.t_step - SAMPLE_DELAY)) - 1;
        wupt_cfg.SeqxSleepTime[CV_1_SEQID] = 1;   // The minimum value is 1. Do not set it to zero. Set it to 1 will spend 2 32kHz clock.
        wupt_cfg.SeqxWakeupTime[CV_1_SEQID] = (uint32_t)(para->LFOSCClkFreq * (para->event.param.cv.cv.t_step - SAMPLE_DELAY)) - 1;
        #undef SAMPLE_DELAY

        AD5940_WUPTCfg(&wupt_cfg);
        break;
    }
    case AD5940_CONTROLLER_EVENT_DPV:
    {
        WUPTCfg_Type wupt_cfg;

        float t_interval;
        t_interval = ELECTROCHEMICAL_DEV_KIT_dpv_get_t_interval(&para->event.param.dpv);

        /* Configure Wakeup Timer */
        #define SAMPLE_DELAY 0.001f
        wupt_cfg.WuptEn = bTRUE;
        wupt_cfg.WuptEndSeq = WUPTENDSEQ_D;
        wupt_cfg.WuptOrder[0] = DPV_STEP_SEQID;
        wupt_cfg.WuptOrder[1] = ADC_SEQID;
        wupt_cfg.WuptOrder[2] = DPV_PULSE_SEQID;
        wupt_cfg.WuptOrder[3] = ADC_SEQID;
        wupt_cfg.SeqxSleepTime[ADC_SEQID] = 1;     // The minimum value is 1. Do not set it to zero. Set it to 1 will spend 2 32kHz clock.
        wupt_cfg.SeqxWakeupTime[ADC_SEQID] = (uint32_t)(para->LFOSCClkFreq * SAMPLE_DELAY) - 1;
        wupt_cfg.SeqxSleepTime[DPV_STEP_SEQID] = 1;    // The minimum value is 1. Do not set it to zero. Set it to 1 will spend 2 32kHz clock.
        wupt_cfg.SeqxWakeupTime[DPV_STEP_SEQID] = (uint32_t)(para->LFOSCClkFreq * (para->event.param.dpv.t_pulse - SAMPLE_DELAY)) - 1;
        wupt_cfg.SeqxSleepTime[DPV_PULSE_SEQID] = 1;   // The minimum value is 1. Do not set it to zero. Set it to 1 will spend 2 32kHz clock.
        wupt_cfg.SeqxWakeupTime[DPV_PULSE_SEQID] = (uint32_t)(para->LFOSCClkFreq * (t_interval - para->event.param.dpv.t_pulse - SAMPLE_DELAY)) - 1;
        #undef SAMPLE_DELAY

        AD5940_WUPTCfg(&wupt_cfg);
        break;
    }
    case AD5940_CONTROLLER_EVENT_TEMPERATURE:
    {
        /* Configure Wakeup Timer*/
        WUPTCfg_Type wupt_cfg;
        wupt_cfg.WuptEn = bTRUE;
        wupt_cfg.WuptEndSeq = WUPTENDSEQ_A;
        wupt_cfg.WuptOrder[0] = ADC_SEQID;
        wupt_cfg.SeqxSleepTime[ADC_SEQID] = 1; /* The minimum value is 1. Do not set it to zero. Set it to 1 will spend 2 32kHz clock. */
        wupt_cfg.SeqxWakeupTime[ADC_SEQID] = (uint32_t)(para->LFOSCClkFreq * para->event.param.temperature.t_interval) - 1;
        AD5940_WUPTCfg(&wupt_cfg);
        break;
    }
    default:
        return AD5940ERR_PARA;
    }

    #undef ADC_SEQID
    #undef CV_0_SEQID
    #undef CV_1_SEQID
    #undef DPV_STEP_SEQID
    #undef DPV_PULSE_SEQID

    return AD5940ERR_OK;
}
