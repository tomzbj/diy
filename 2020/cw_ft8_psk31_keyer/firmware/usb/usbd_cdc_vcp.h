#ifndef __USBD_CDC_VCP_H
#define __USBD_CDC_VCP_H

#include "usbd_cdc_core.h"

typedef struct {
    uint32_t bitrate;
    uint8_t format;
    uint8_t paritytype;
    uint8_t datatype;
} LINE_CODING;

#define EVAL_COM_IRQHandler            USART2_IRQHandler
#define USART_IT_PRIO                  0 
#define USB_IT_PRIO                    1 

#define DEFAULT_CONFIG                  0
#define OTHER_CONFIG                    1

void EVAL_COM_IRQHandler(void);
void USB_VCP_WriteData(const void* msg, int size);

#endif 

