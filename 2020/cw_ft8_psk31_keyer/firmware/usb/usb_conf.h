#ifndef __USB_CONF__H__
#define __USB_CONF__H__

#include "stm32f0xx.h"

#define INTERNAL_PULLUP
#define EP_NUM     (4)   
#define BTABLE_ADDRESS      (0x000)
#define ENDP0_RX_ADDRESS   (0x40)
#define ENDP0_TX_ADDRESS   (0x80)
#define BULK_IN_TX_ADDRESS  (0xC0) 
#define BULK_OUT_RX_ADDRESS (0x110)
#define INT_IN_TX_ADDRESS   (0x100)

#endif 

