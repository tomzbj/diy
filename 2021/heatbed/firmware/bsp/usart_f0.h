/// @file usart.h
/// @brief 
/// @author Zhang Hao, stavrosatic@gmail.com
/// @version R0.1
/// @date 2017-10-13
#ifndef _USART_H
#define _USART_H

#include "misc.h"

enum {
    USART_RT_CONNECTED, USART_RT_DISCONNECTED
};
void USART_WriteData(USART_TypeDef* USARTx, const void* data, int num);
void USART_Config(void);
void USART_RX_IDLE_IRQHandler(USART_TypeDef* USARTx);
void USART_Poll(void);
void USART_ChangeBaudRate(USART_TypeDef* USARTx, unsigned long baudrate);

#endif
