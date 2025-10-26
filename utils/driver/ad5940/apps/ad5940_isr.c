#include "ad5940_isr.h"

#include "ad5940_utils.h"

AD5940Err AD5940_DATAFIFOISR(
    AD5940_DATAFIFOISR_PARA para, 
    AD5940_DATAFIFOISR_RET ret
)
{
    /* Wakeup AFE by read register, read 10 times at most */
    if(AD5940_WakeUp(10) > 10) return AD5940ERR_WAKEUP;  /* Wakeup Failed */

    AD5940_SleepKeyCtrlS(SLPKEY_LOCK);  /* We need time to read data from FIFO, so, do not let AD5940 goes to hibernate automatically */

    *(ret.pBuffCount) = AD5940_FIFOGetCnt();
    if(*(ret.pBuffCount) > para.BufferSize) return AD5940ERR_BUFF;
    AD5940_FIFORd(ret.pBuff, *(ret.pBuffCount));

    AD5940_INTCClrFlag(AFEINTSRC_DATAFIFOTHRESH);
    
    // Refer to page 107 of the datasheet
    // Enable AFE to enter sleep mode.
    AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK); /* Unlock so sequencer can put AD5940 to sleep */

    if(para.Enable)
    {
        AD5940_FIFOCONRst();
        if(para.FIFOThresh < AD5940_DATAFIFOTHRESH_UNCHANGE)
        {
            AD5940_FIFOThrshSet(para.FIFOThresh);
        }
    }
    else
    {
        /* Stop Wupt right now */
        AD5940_WUPTCtrl(bFALSE);
        /* For detailed reference on the chip's shutdown procedure, see page 129 of the datasheet.*/
        AD5940_ShutDownS();
    }

    return AD5940ERR_OK;
}
