#ifndef _USART_H
#define _USART_H

#include "misc.h"

void USART_Config(void);

unsigned char USART_ReadByte(int usart_id);
void USART_WriteByte(unsigned char data, int usart_id);

#endif
