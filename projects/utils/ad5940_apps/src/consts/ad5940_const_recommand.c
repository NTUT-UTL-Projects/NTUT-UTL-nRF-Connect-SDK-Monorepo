#include "ad5940_const_recommand.h"

void LPDAC2LPTIA_AFERefCfg(AFERefCfg_Type *const type)
{
    /**
     * Enable the high-precision voltage references.
     * Refer to page 25 and Figure 37 (page 87) of the datasheet.
     */
    type->HpBandgapEn = bTRUE;

    /**
     * Recommended high-precision buffer settings.
     * Refer to page 87 of the datasheet for details.
     */
    type->Hp1V1BuffEn = bTRUE;
    type->Hp1V8BuffEn = bTRUE;
    type->Disc1V1Cap = bFALSE;
    type->Disc1V8Cap = bFALSE;
    type->Hp1V8ThemBuff = bFALSE;
    type->Hp1V8Ilimit = bFALSE;
    type->Lp1V1BuffEn = bFALSE;
    type->Lp1V8BuffEn = bFALSE;
    
    // LP reference control - turn off them to save power
    type->LpRefBoostEn = bFALSE;

    // Refer to page 34 of the datasheet. See also the routing diagrams in Figure 22 (page 39) and Figure 37 (page 87).
    // This output is used to drive the LPDAC, or to generate a bias voltage via the LPDAC when using the HSDAC.
    type->LpBandgapEn = bTRUE;
    type->LpRefBufEn = bTRUE;

    type->HSDACRefEn = bFALSE;

    return;
}

void LPDAC2HSTIA_AFERefCfg(AFERefCfg_Type *const type)
{
    LPDAC2LPTIA_AFERefCfg(type);
    return;
}


void HSDAC2HSTIA_AFERefCfg(AFERefCfg_Type *const type, BoolFlag VBias0En)
{

    LPDAC2LPTIA_AFERefCfg(type);
    // Refer to page 34 of the datasheet. See also the routing diagrams in Figure 22 (page 39) and Figure 37 (page 87).
    // This output is used to drive the LPDAC, or to generate a bias voltage via the LPDAC when using the HSDAC.
    if(VBias0En == bTRUE)
    {
        type->LpBandgapEn = bTRUE;
        type->LpRefBufEn = bTRUE;
    }
    else
    {
        type->LpBandgapEn = bFALSE;
        type->LpRefBufEn = bFALSE;
    }
    return;
}

void LPDAC2LPTIA_LPDACCfg_Type(LPDACCfg_Type *const type)
{

    #if defined(CHIPSEL_594X)
    type->LpdacSel = LPDAC0;
    #endif
    type->DataRst = bFALSE;                     // Do not reset data register. @see p. 33 of the datasheet.
    type->PowerEn = bTRUE;                      // Power up. @see p. 33 of the datasheet.

    // Refer to page 39 of the datasheet, LPPA receives feedback to support LPDAC in producing a precise output signal.
    type->LpDacSW = LPDACSW_VBIAS2LPPA | LPDACSW_VZERO2LPTIA;

    type->LpDacVzeroMux = LPDACVZERO_6BIT;
    type->LpDacVbiasMux = LPDACVBIAS_12BIT;
    return;
}

void LPDAC2HSTIA_LPDACCfg_Type(LPDACCfg_Type *const type)
{
    LPDAC2LPTIA_LPDACCfg_Type(type);
    // Refer to page 39 of the datasheet, LPPA receives feedback to support LPDAC in producing a precise output signal.
    type->LpDacSW = LPDACSW_VBIAS2LPPA | LPDACSW_VZERO2HSTIA;
    return;
}

void LPDAC2LPTIA_LPAmpCfg_Type(LPAmpCfg_Type *const type, uint32_t LpTiaRtia)
{
    #if defined(CHIPSEL_594X)
    type->LpAmpSel = LPAMP0;
    #endif
    type->LpPaPwrEn = bTRUE;
    type->LpTiaPwrEn = bTRUE;
    type->LpTiaRtia = LpTiaRtia;
    if(type->LpTiaRtia == LPTIARTIA_OPEN)
    {
        type->LpTiaSW = 0
            | LPTIASW(2)
            | LPTIASW(4)
            | LPTIASW(5)
            | LPTIASW(9)
            // | LPTIASW(12)
            // | LPTIASW(13)
        ;
    }
    else
    {
        type->LpTiaSW = 0
            | LPTIASW(2)
            | LPTIASW(4)
            // | LPTIASW(5)
            // | LPTIASW(12)
            // | LPTIASW(13)
        ;
    }
    return;
}

void LPDAC2HSTIA_LPAmpCfg_Type(LPAmpCfg_Type *const type)
{
    #if defined(CHIPSEL_594X)
    type->LpAmpSel = LPAMP0;
    #endif
    type->LpTiaRf = 0;
    type->LpTiaRtia = 0;
    type->LpPaPwrEn = bTRUE;
    type->LpTiaPwrEn = bFALSE;
    type->LpTiaRload = LPTIARLOAD_SHORT;
    type->LpTiaSW = 0
        | LPTIASW(2)
        | LPTIASW(4)

        // When using LPDAC with HATIA to start the electrochemical reaction, 
        // close LPTIASW(6) to prevent the signal from being generated on SE0.
        | LPTIASW(6)

        | LPTIASW(7)
    ;
    return;
}

void LPTIA2HSTIACfg_HSTIACfg_Type(HSTIACfg_Type *const type, BoolFlag VBias0En)
{
    // @see page 45 of the datasheet, With bias voltage */
    if(VBias0En)
    {
        type->HstiaBias = HSTIABIAS_VZERO0;
    }
    else
    {
        type->HstiaBias = HSTIABIAS_1P1;
    }
}
