#ifndef _USART_H
#define _USART_H

#include "misc.h"

void USART_Config(void);
void USART_RXNE_IRQHandler(USART_TypeDef* USARTx);
void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx);
void USART_WriteByte(USART_TypeDef* USARTx, unsigned char byte);
void USART_WriteData(USART_TypeDef* USARTx, const void* data, int num);
void USART_BindParser(void (*parser_f)(const void* msg, int size, int source));

void USART_Poll(void);

#endif
