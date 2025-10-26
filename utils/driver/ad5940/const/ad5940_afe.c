#include "ad5940_afe.h"

AD5940Err AD5940_AFERefCfg_recommended(
    const BoolFlag Hp1V8BuffEn, 
    const BoolFlag Lp1V1BuffEn, 
    const BoolFlag Hp1V8ThemBuff, 
    const BoolFlag LPDAC_enable,
    AFERefCfg_Type *const aferef_cfg
)
{
    // This option is for the DAC, but other functions are available to set it, so please ignore it.
    aferef_cfg->HpBandgapEn = bFALSE;

    // Set to recommanded options.
    aferef_cfg->Hp1V8ThemBuff = Hp1V8ThemBuff;
    aferef_cfg->Disc1V1Cap = bFALSE;
    aferef_cfg->Lp1V1BuffEn = Lp1V1BuffEn;
    aferef_cfg->Hp1V1BuffEn = bTRUE;
    aferef_cfg->Disc1V8Cap = bFALSE;
    aferef_cfg->Lp1V8BuffEn = bTRUE;
    aferef_cfg->Hp1V8Ilimit = bTRUE;
    aferef_cfg->Hp1V8BuffEn = Hp1V8BuffEn;
    
    // The option is useless
    aferef_cfg->LpRefBoostEn = bFALSE;

    /* LP reference control - turn off them to save power*/
    if(LPDAC_enable == bTRUE)    /* With bias voltage */
    {
        aferef_cfg->LpBandgapEn = bTRUE;
        aferef_cfg->LpRefBufEn = bTRUE;
    }
    else
    {
        aferef_cfg->LpBandgapEn = bFALSE;
        aferef_cfg->LpRefBufEn = bFALSE;
    }

    return AD5940ERR_OK;
}
