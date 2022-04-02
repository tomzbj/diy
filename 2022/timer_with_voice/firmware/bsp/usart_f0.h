#ifndef _USART_H
#define _USART_H

#include "misc.h"

#define ZU_DEBUG 0

void USART_Config(void);
unsigned char USART_ReadByte(int source);
void USART_WriteByte(unsigned char c, int source);
void USART_SetBaudrate(unsigned long baudrate);

#endif
