#ifndef _USART_HAL_H
#define _USART_HAL_H

void USART_Config(void);

uint8_t USART1_ReadByte(void);
uint8_t USART2_ReadByte(void);
void USART1_WriteByte(uint8_t c);
void USART2_WriteByte(uint8_t c);

#endif
