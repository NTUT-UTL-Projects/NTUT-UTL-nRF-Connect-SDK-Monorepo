#include "ad5940_str.h"

void AD5940_AfeCtrlSet_to_str(uint32_t AfeCtrlSet, char *const str, uint16_t *const str_len)
{
    if (AfeCtrlSet & AFECTRL_HPREFPWR)
    {
        char s[] = "AFECTRL_HPREFPWR, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_HSDACPWR)
    {
        char s[] = "AFECTRL_HSDACPWR, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_ADCPWR)
    {
        char s[] = "AFECTRL_ADCPWR, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_ADCCNV)
    {
        char s[] = "AFECTRL_ADCCNV, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_EXTBUFPWR)
    {
        char s[] = "AFECTRL_EXTBUFPWR, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_INAMPPWR)
    {
        char s[] = "AFECTRL_INAMPPWR, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_HSTIAPWR)
    {
        char s[] = "AFECTRL_HSTIAPWR, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_TEMPSPWR)
    {
        char s[] = "AFECTRL_TEMPSPWR, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_TEMPCNV)
    {
        char s[] = "AFECTRL_TEMPCNV, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_WG)
    {
        char s[] = "AFECTRL_WG, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_DFT)
    {
        char s[] = "AFECTRL_DFT, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_SINC2NOTCH)
    {
        char s[] = "AFECTRL_SINC2NOTCH, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_ALDOLIMIT)
    {
        char s[] = "AFECTRL_ALDOLIMIT, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_DACREFPWR)
    {
        char s[] = "AFECTRL_DACREFPWR, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_DCBUFPWR)
    {
        char s[] = "AFECTRL_DCBUFPWR, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (AfeCtrlSet & AFECTRL_ALL)
    {
        char s[] = "AFECTRL_ALL, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    return;
}

void AD5940_AFERefCfg_to_str(AFERefCfg_Type *const aferef_cfg, char *const str, uint16_t *const str_len)
{
    if (aferef_cfg->HpBandgapEn)
    {
        char s[] = "HpBandgapEn, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (aferef_cfg->Hp1V8BuffEn)
    {
        char s[] = "Hp1V8BuffEn, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (aferef_cfg->Hp1V1BuffEn)
    {
        char s[] = "Hp1V1BuffEn, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (aferef_cfg->Lp1V8BuffEn)
    {
        char s[] = "Lp1V8BuffEn, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (aferef_cfg->Lp1V1BuffEn)
    {
        char s[] = "Lp1V1BuffEn, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (aferef_cfg->LpBandgapEn)
    {
        char s[] = "LpBandgapEn, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (aferef_cfg->LpRefBufEn)
    {
        char s[] = "LpRefBufEn, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (aferef_cfg->LpRefBoostEn)
    {
        char s[] = "LpRefBoostEn, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (aferef_cfg->HSDACRefEn)
    {
        char s[] = "HSDACRefEn, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (aferef_cfg->Hp1V8ThemBuff)
    {
        char s[] = "Hp1V8ThemBuff, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (aferef_cfg->Hp1V8Ilimit)
    {
        char s[] = "Hp1V8Ilimit, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (aferef_cfg->Disc1V8Cap)
    {
        char s[] = "Disc1V8Cap, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    if (aferef_cfg->Disc1V1Cap)
    {
        char s[] = "Disc1V1Cap, \n";
        memcpy(str + *str_len, s, sizeof(s));
        *str_len += sizeof(s);
    }
    return;
}

void AD5940_DSPCfg_to_str(DSPCfg_Type *const dsp_cfg, char *const str, uint16_t *const str_len)
{
    // TODO
    return;
}

void AD5940_LPLoopCfg_to_str(LPLoopCfg_Type *const lploop_cfg, char *const str, uint16_t *const str_len)
{
    // TODO
    return;
}

void AD5940_HSLoopCfg_to_str(HSLoopCfg_Type *const hsloop_cfg, char *const str, uint16_t *const str_len)
{
    // TODO
    return;
}
