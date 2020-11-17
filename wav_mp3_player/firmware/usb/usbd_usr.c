#include "usbd_usr.h"
#include <stdio.h>

USBD_Usr_cb_TypeDef USR_cb = {
    USBD_USR_Init,
    USBD_USR_DeviceReset,
    USBD_USR_DeviceConfigured,
    USBD_USR_DeviceSuspended,
    USBD_USR_DeviceResumed,
    USBD_USR_DeviceConnected,
    USBD_USR_DeviceDisconnected,
};
#define USER_INFORMATION1  (uint8_t*)"INFO : Single Lun configuration"
#define USER_INFORMATION2  (uint8_t*)"INFO : microSD is used"

void USBD_USR_Init(void)
{
    /*
#ifdef USE_EMBEDDED_PHY
    LCD_LOG_SetHeader((uint8_t *) " USB OTG HS_IN_FS MSC Device");
#else
    LCD_LOG_SetHeader((uint8_t *) " USB OTG HS MSC Device");
#endif
#else
    LCD_LOG_SetHeader((uint8_t *) " USB OTG FS MSC Device");
#endif
    LCD_UsrLog("> USB device library started.\n");
    LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1");
    LCD_SetTextColor(Green);
    LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 42, USER_INFORMATION1);
    LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 30, USER_INFORMATION2);
    LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR);
    */
}
void USBD_USR_DeviceReset(uint8_t speed)
{
    /*
    switch (speed)
    {
        case USB_OTG_SPEED_HIGH:
            LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1  [HS]");
            break;
        case USB_OTG_SPEED_FULL:
            LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1  [FS]");
            break;
        default:
            LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1  [??]");
    }
    */
}
void USBD_USR_DeviceConfigured(void)
{
//    LCD_UsrLog("> MSC Interface started.\n");
}
void USBD_USR_DeviceSuspended(void)
{
//    LCD_UsrLog("> Device In suspend mode.\n");
}
void USBD_USR_DeviceResumed(void)
{
}
void USBD_USR_DeviceConnected(void)
{
//    LCD_UsrLog("> USB Device Connected.\n");
}
void USBD_USR_DeviceDisconnected(void)
{
//    LCD_UsrLog("> USB Device Disconnected.\n");
}


