
#include "usbd_usr.h"
USBD_Usr_cb_TypeDef USR_cb =
{
    USBD_USR_Init,
    USBD_USR_DeviceReset,
    USBD_USR_DeviceConfigured,
    USBD_USR_DeviceSuspended,
    USBD_USR_DeviceResumed,  
};
void USBD_USR_Init(void)
{   
}
void USBD_USR_DeviceReset(uint8_t speed )
{
}
void USBD_USR_DeviceConfigured (void)
{
}
void USBD_USR_DeviceSuspended(void)
{
}
void USBD_USR_DeviceResumed(void)
{
}

