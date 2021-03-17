#ifndef _USART_H
#define _USART_H

#include "misc.h"

void USART_Config(void);
unsigned char USART_ReadByte(void);
void USART_WriteByte(char c);

#endif
