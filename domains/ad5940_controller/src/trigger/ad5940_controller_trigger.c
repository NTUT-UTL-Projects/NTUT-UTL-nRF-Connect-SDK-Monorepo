#include "ad5940_controller_trigger.h"
#include "ad5940_utils.h"
#include "ad5940_apps.h"

AD5940Err AD5940_controller_trigger(
    AD5940_CONTROLLER_TRIGGER_PARA para
)
{
    AD5940Err err = AD5940ERR_OK;

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

    AD5940_REFCfgS(&(para.aferef_cfg));
    AD5940_LPLoopCfgS(&(para.lploop_cfg));
    AD5940_HSLoopCfgS(&(para.hsloop_cfg));
    AD5940_DSPCfgS(&(para.dsp_cfg));

    /* Enable all of them. They are automatically turned off during hibernate mode to save power */
    AD5940_AFECtrlS(para.AfeCtrlSet, bTRUE);

    // Ensure it is cleared as ad5940.c relies on the INTC flag as well.
    AD5940_INTCClrFlag(AFEINTSRC_ALLINT);

    switch (para.event)
    {
    case AD5940_CONTROLLER_EVENT_CA:
        // TODO
        break;
    case AD5940_CONTROLLER_EVENT_CV:
        // TODO
        break;
    case AD5940_CONTROLLER_EVENT_DPV:
    {
        uint32_t ADC_SeqId = SEQID_0;
        uint32_t Dpv_Step_SeqId = SEQID_1;
        uint32_t Dpv_Pulse_SeqId = SEQID_2;
        uint32_t SeqRamAddr = 0;
        uint32_t SeqLen;

        BoolFlag EnterSleepEn = bFALSE;

        ClksCalInfo_Type clks_cal;
        AD5940_write_ClksCalInfo_Type(
            (AD5940_write_ClksCalInfo_Type_Para) {
                .DataCount = 1,
                .DataType = para.DataType,
                .dsp_cfg = para.dsp_cfg,
                .RatioSys2AdcClk = para.RatioSys2AdcClk,
            },
            &clks_cal
        );

        AD5940_ADCSEQCmdWrite(
            (AD5940_ADCSEQCmdWrite_Para) {
                .clks_cal = clks_cal,
                .EnterSleepEn = EnterSleepEn,
                .SeqId = ADC_SeqId,
                .SeqRamAddr = SeqRamAddr,
            },
            &SeqLen
        );
        SeqRamAddr += SeqLen;

        err = AD5940_DPV_PARAM_check(para.param.dpv);
        if(err != AD5940ERR_OK) return err;
        err = AD5940_DPV_writeSEQ(
            (AD5940_DPV_writeSEQ_Para) {
                .base_volt = 0.0f,
                .EnterSleepEn = EnterSleepEn,
                .param = para.param.dpv,
                .SeqIdPulse = Dpv_Pulse_SeqId,
                .SeqIdStep = Dpv_Step_SeqId,
                .SeqRamAddr = SeqRamAddr,
            },
            &SeqLen
        );

        BoolFlag AFEINTC_0_En, AFEINTC_1_En;
        AD5940_FuncSet2AFEINTCx(
            para.gpio_cfg.FuncSet,
            &AFEINTC_0_En,
            &AFEINTC_1_En
        );
        AD5940_AFEINTCxCfg(
            AFEINTC_0_En,
            AFEINTC_1_En,
            AFEINTSRC_DATAFIFOTHRESH
        );

        // TODO
        // Some circuit boards fail to configure gpio for unknown reasons.
        // Executing this command can alleviate this problem, but it may still occur.
        // {
        //     AGPIOCfg_Type no_gpio = {0};
        //     for(size_t i=0; i<10; i++)
        //     {
        //         // AD5940_FIFOCtrlS(para.FifoSrc, bFALSE);
        //         AD5940_AGPIOCfg(&(no_gpio));
        //         AD5940_Delay10us(10);
        //         AD5940_AGPIOCfg(&(para.gpio_cfg));
        //         // AD5940_FIFOCtrlS(para.FifoSrc, bTRUE);
        //     }
        // }
        
        AD5940_AGPIOCfg(&(para.gpio_cfg));

        /* Configure FIFO and Sequencer for normal Amperometric Measurement */
        uint32_t FifoThresh = 1;    // Set to 1, so that when there is data in FIFO, GPIO will be triggered.
        AD5940_FIFOThrshSet(FifoThresh);
        AD5940_FIFOCtrlS(para.FifoSrc, bTRUE);

        AD5940_SEQCtrlS(bTRUE);

        WUPTCfg_Type wupt_cfg;
        err = AD5940_DPV2WUPT(
            (AD5940_DPV2WUPT_Para) {
                .LFOSCClkFreq = para.LFOSCClkFreq,
                .param = para.param.dpv,
                .SeqIdAdc = ADC_SeqId,
                .SeqIdPulse = Dpv_Pulse_SeqId,
                .SeqIdStep = Dpv_Step_SeqId,
            },
            &wupt_cfg
        );
        if(err != AD5940ERR_OK) return err;
        AD5940_WUPTCfg(&wupt_cfg);
        
        break;
    }
    case AD5940_CONTROLLER_EVENT_EIS:
        // TODO
        break;
    default:
        return AD5940ERR_PARA;
    }

    return AD5940ERR_OK;
}
