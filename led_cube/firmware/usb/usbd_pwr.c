#include "usbd_pwr.h"

struct {
    __IO RESUME_STATE eState;
    __IO uint8_t bESOFcnt;
} ResumeS;

__IO uint32_t remotewakeupon=0;
void Suspend(void)
{
    uint16_t wCNTR;
    wCNTR = _GetCNTR();   
    wCNTR |= CNTR_FSUSP;
    _SetCNTR(wCNTR);
    wCNTR = _GetCNTR();
    wCNTR |= CNTR_LPMODE;
    _SetCNTR(wCNTR);
#ifdef USB_DEVICE_LOW_PWR_MGMT_SUPPORT
    if((_GetISTR()&ISTR_WKUP)==0)
    { 
        PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_SLEEPONEXIT);
    }
    else
    {
        _SetISTR(CLR_WKUP);
        wCNTR = _GetCNTR();
        wCNTR&=~CNTR_FSUSP;
        _SetCNTR(wCNTR);
    }
#endif
}
void Resume_Init(void)
{
    uint16_t wCNTR;
    wCNTR = _GetCNTR();
    wCNTR &= (~CNTR_LPMODE);
    _SetCNTR(wCNTR);    
#ifdef USB_DEVICE_LOW_PWR_MGMT_SUPPORT   
    Leave_LowPowerMode();
#endif
    _SetCNTR(IMR_MSK);
}
void Resume(RESUME_STATE eResumeSetVal)
{
    uint16_t wCNTR;
    if (eResumeSetVal != RESUME_ESOF)
        ResumeS.eState = eResumeSetVal;
    switch (ResumeS.eState)
    {
        case RESUME_EXTERNAL:
            if (remotewakeupon ==0)
            {
                Resume_Init();
                ResumeS.eState = RESUME_OFF;
            }
            else 
            {
                ResumeS.eState = RESUME_ON;
            }
            break;
        case RESUME_INTERNAL:
            Resume_Init();
            ResumeS.eState = RESUME_START;
            remotewakeupon = 1;
            break;
        case RESUME_LATER:
            ResumeS.bESOFcnt = 2;
            ResumeS.eState = RESUME_WAIT;
            break;
        case RESUME_WAIT:
            ResumeS.bESOFcnt--;
            if (ResumeS.bESOFcnt == 0)
                ResumeS.eState = RESUME_START;
            break;
        case RESUME_START:
            wCNTR = _GetCNTR();
            wCNTR |= CNTR_RESUME;
            _SetCNTR(wCNTR);
            ResumeS.eState = RESUME_ON;
            ResumeS.bESOFcnt = 10;
            break;
        case RESUME_ON:    
            ResumeS.bESOFcnt--;
            if (ResumeS.bESOFcnt == 0)
            {
                wCNTR = _GetCNTR();
                wCNTR &= (~CNTR_RESUME);
                _SetCNTR(wCNTR);
                ResumeS.eState = RESUME_OFF;
                remotewakeupon = 0;
            }
            break;
        case RESUME_OFF:
        case RESUME_ESOF:
        default:
            ResumeS.eState = RESUME_OFF;
            break;
    }
}
void Leave_LowPowerMode(void)
{
#if defined USB_CLOCK_SOURCE_CRS
    RCC_HSI48Cmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_HSI48RDY) == RESET)
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI48);
#else
    RCC_HSEConfig(RCC_HSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {}
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08)
    {} 
#endif 
    NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT, DISABLE);
}

