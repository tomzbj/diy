#ifndef _USART_H
#define _USART_H

#include "misc.h"

void USART_Config(void);
void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx);
void USART_Poll(void);
void USART_WriteByte(USART_TypeDef* USARTx, unsigned char byte);
void USART_BindParser(void (*parser_f)(const unsigned char* msg, int size));
#endif
