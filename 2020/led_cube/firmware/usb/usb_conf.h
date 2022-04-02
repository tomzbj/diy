#ifndef __USB_CONF__H__
#define __USB_CONF__H__
#include "stm32f0xx.h"
 
#define INTERNAL_PULLUP

#define EP_NUM    (3) 
#define BTABLE_ADDRESS    (0x00)
#define ENDP0_RX_ADDRESS    (0x18)
#define ENDP0_TX_ADDRESS    (0x58)
#define MSC_IN_TX_ADDRESS     (0x98)
    
#define MSC_OUT_RX_ADDRESS    (0xD8)
#endif 

