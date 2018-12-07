#include "nrf24le1.h"

void CLK_Init(CLK_InitTypeDef* CLK_InitStructure)
{
    CLKCTRL = *(unsigned char*)CLK_InitStructure;
}

unsigned long CLK_GetSystemClock(void)
{
    unsigned long sysclk = 16000000UL;

    int i, n = CLKCTRL & 0x7;
    for(i = 0; i < n; i++) {
        sysclk /= 2;
    }

    return sysclk; 
}
