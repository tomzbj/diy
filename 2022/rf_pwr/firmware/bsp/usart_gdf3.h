#ifndef _USART_H
#define _USART_H

void USART_Config(void);
void USART_WriteByte(unsigned char c, int source);
unsigned char USART_ReadByte(int source);
unsigned char USART_SPI_ReadWrite(unsigned char data);

#endif
