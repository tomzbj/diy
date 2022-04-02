#ifndef _USART_F1_H
#define _USART_F1_H

#include "misc.h"

typedef enum {
    USART_MODE_NORMAL, USART_MODE_HMI_EDIT, USART_MODE_NX_EDIT
} USART_Mode_t;

void USART_Config(void);
void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx);
void uprintf(USART_TypeDef* USARTn, const char* __restrict fmt, ...);
void USART_WriteByte(USART_TypeDef* USARTx, unsigned char byte);

bool USART_FrameToParse(void);
void USART_Poll(void);

void USART_Mode_HMIEDIT(void);
#endif
