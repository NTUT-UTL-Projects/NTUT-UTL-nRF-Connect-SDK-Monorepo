#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

#define LPDAC2LPTIA_AfeCtrlSet 0
#define LPDAC2HSTIA_AfeCtrlSet AFECTRL_HSTIAPWR

    // set all
    void AD5940_LPDAC2LPTIA_AFERefCfg(AFERefCfg_Type *const type);

    // set all
    void AD5940_LPDAC2HSTIA_AFERefCfg(AFERefCfg_Type *const type);

    // set all
    void
    AD5940_HSDAC2HSTIA_AFERefCfg(AFERefCfg_Type *const type, BoolFlag VBias0En);

    // unset
    // - DacData12Bit;
    // - DacData6Bit;
    // - LpDacRef;
    // - LpDacSrc;
    void AD5940_LPDAC2LPTIA_LPDACCfg_Type(LPDACCfg_Type *const type);

    // unset
    // - DacData12Bit;
    // - DacData6Bit;
    // - LpDacRef;
    // - LpDacSrc;
    void AD5940_LPDAC2HSTIA_LPDACCfg_Type(LPDACCfg_Type *const type);

    // unset
    // - LpAmpPwrMod;
    // - LpTiaRf;
    // - LpTiaRload;
    void AD5940_LPDAC2LPTIA_LPAmpCfg_Type(
        LPAmpCfg_Type *const type,
        uint32_t             LpTiaRtia
    );

    // unset
    // - LpAmpPwrMod;
    void AD5940_LPDAC2HSTIA_LPAmpCfg_Type(LPAmpCfg_Type *const type);

    // set
    // - HstiaBias
    void AD5940_LPTIA2HSTIACfg_HSTIACfg_Type(
        HSTIACfg_Type *const type,
        BoolFlag             VBias0En
    );

#ifdef __cplusplus
}
#endif
