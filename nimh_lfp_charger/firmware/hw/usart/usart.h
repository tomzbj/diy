/// @file usart.h
/// @brief 
/// @author Zhang Hao, stavrosatic@gmail.com
/// @version R0.1
/// @date 2017-10-13
#ifndef _USART_H
#define _USART_H

#include "misc.h"

void USART_WriteByte(USART_TypeDef* USARTx, u8 byte);
void USART_WriteData(USART_TypeDef* USARTx, const void* data, int num);
void USART_Config(void);
void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx);
void uprintf(USART_TypeDef* USARTn, const char* __restrict fmt, ...);
bool USART_FrameToParse(void);
void USART_ParseFrame(void);

#endif
