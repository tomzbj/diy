#ifndef _USART_H
#define _USART_H

#include "misc.h"
#include <stdbool.h>

void USART_Config(void);
void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx);
void uprintf(USART_TypeDef* USARTn, const char* __restrict fmt, ...);
void USART_WriteByte(USART_TypeDef* USARTx, unsigned char byte);
void USART_WriteData_s(USART_TypeDef* USARTx, const void* data, int num);
bool USART_FrameToParse(void);
void USART_ParseFrame(void);

#endif
