#include "usb_lib.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include <stdbool.h>

volatile unsigned char bDeviceState = UNCONNECTED; /* USB device status */
volatile bool fSuspendEnabled = true;   /* true when suspend is possible */
struct {
    volatile RESUME_STATE eState;
    volatile unsigned char bESOFcnt;
} ResumeS;

RESULT PowerOn(void)
{
    u16 wRegVal;
    USB_Cable_Config(ENABLE); /*** cable plugged-in ? ***/
    wRegVal = CNTR_FRES; /*** CNTR_PWDN = 0 ***/
    _SetCNTR(wRegVal);
    wInterrupt_Mask = 0; /*** CNTR_FRES = 0 ***/
    _SetCNTR(wInterrupt_Mask);
    _SetISTR(0); /*** Clear pending interrupts ***/
    wInterrupt_Mask = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM; /*** Set interrupt mask ***/
    _SetCNTR(wInterrupt_Mask);
    return USB_SUCCESS;
}

RESULT PowerOff()
{
    _SetCNTR(CNTR_FRES); /* disable all ints and force USB reset */
    _SetISTR(0); /* clear interrupt status register */
    USB_Cable_Config(DISABLE); /* Disable the Pull-Up */
    _SetCNTR(CNTR_FRES + CNTR_PDWN); /* switch-off device */
    return USB_SUCCESS;
}

void Suspend(void)
{
    u16 wCNTR;
    wCNTR = _GetCNTR(); /* macrocell enters suspend mode */
    wCNTR |= CNTR_FSUSP;
    _SetCNTR(wCNTR);
    /* ONLY WITH BUS-POWERED DEVICES */ /* power reduction */ /* ... on connected devices */ 
    wCNTR = _GetCNTR(); /* force low-power mode in the macrocell */
    wCNTR |= CNTR_LPMODE;
    _SetCNTR(wCNTR);
    /* switch-off the clocks */ /* ... */
    Enter_LowPowerMode();
}

void Resume_Init(void)
{
    u16 wCNTR;
    /* ONLY WITH BUS-POWERED DEVICES */ /* restart the clocks */ /* ...  */
    wCNTR = _GetCNTR(); /* CNTR_LPMODE = 0 */
    wCNTR &= (~CNTR_LPMODE);
    _SetCNTR(wCNTR); 
    Leave_LowPowerMode(); /* restore full power */ /* ... on connected devices */ 
    _SetCNTR(IMR_MSK); /* reset FSUSP bit */
    /* reverse suspend preparation */ /* ... */ 
}

void Resume(RESUME_STATE eResumeSetVal)
{
    u16 wCNTR;
    if (eResumeSetVal != RESUME_ESOF)
        ResumeS.eState = eResumeSetVal;
    switch (ResumeS.eState) {
    case RESUME_EXTERNAL:
        Resume_Init();
        ResumeS.eState = RESUME_OFF;
        break;
    case RESUME_INTERNAL:
        Resume_Init();
        ResumeS.eState = RESUME_START;
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
        if (ResumeS.bESOFcnt == 0) {
            wCNTR = _GetCNTR();
            wCNTR &= (~CNTR_RESUME);
            _SetCNTR(wCNTR);
            ResumeS.eState = RESUME_OFF;
        }
        break;
    case RESUME_OFF:
    case RESUME_ESOF:
    default:
        ResumeS.eState = RESUME_OFF;
        break;
    }
} 
