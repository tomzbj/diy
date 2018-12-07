#include "nrf24le1.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

const unsigned short s0rel_tab[] = { 0x330, 0x398, 0x3cc, 0x3e6, 0x3f3, 0x3f7,
        0x3fe, 0x3ff, 0x3ff };

void UART_Init(UART_InitTypeDef* UART_InitStructure)
{
    S0CON = *(unsigned char*) UART_InitStructure;
    ADCON |= 0x80;          // bd bit
    PCON |= 0x80;
    if(PCON & 0x80) {       // SMOD bit
        UART_InitStructure->UART_BaudRate++;
        S0REL = s0rel_tab[UART_InitStructure->UART_BaudRate];
    }
}

void UART_WriteByte(unsigned char c)
{
    putchar(c);
}
