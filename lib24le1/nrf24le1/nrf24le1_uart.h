#ifndef _NRF24LE1_UART_H
#define _NRF24LE1_UART_H

typedef enum {
    UART_Mode_0 = 0x0, UART_Mode_8bit = 0x1, UART_Mode_2 = 0x2,
    UART_Mode_9bit = 0x3
} UART_ModeTypeDef;

typedef enum {
    UART_BaudRate_2400 = 0, UART_BaudRate_4800 = 1, UART_BaudRate_9600 = 2,
    UART_BaudRate_19200 = 3, UART_BaudRate_38400 = 4, UART_BaudRate_57600 = 5,
    UART_BaudRate_250000 = 6, UART_BaudRate_500000 = 7
} UART_BaudRateTypeDef;

typedef struct {
    unsigned char reserved:4;
    unsigned char UART_Rx:1;
    unsigned char UART_MultiprocessorMode:1;
    unsigned char UART_Mode:2;
    UART_BaudRateTypeDef UART_BaudRate;
} UART_InitTypeDef;

void UART_Init(UART_InitTypeDef* UART_InitStructure);

void UART_WriteByte(unsigned char c);

//void isr_uart(void) __interrupt(4);
//#define UART_IRQHandler()       void ISR_UART(void)     __interrupt(4)
//void ISR_UART(void)     __interrupt(4);

#endif
