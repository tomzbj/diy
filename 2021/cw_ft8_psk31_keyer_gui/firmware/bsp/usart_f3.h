#ifndef _USART_H
#define _USART_H

#include "misc.h"

void USART_Config(void);
unsigned char USART_ReadByte(int source);
void USART_WriteByte(unsigned char c, int source);
unsigned char SPI_USART1_ReadWrite(unsigned char data);

#endif
