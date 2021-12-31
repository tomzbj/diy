#ifndef _USART_AVR_H
#define _USART_AVR_H

void USART_Config(void);
unsigned char USART_ReadByte(int source);
void USART_WriteByte(unsigned char c, int source);

#endif
