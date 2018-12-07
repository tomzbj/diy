#include "nrf24le1.h"
#include <stdbool.h>

#define MAXLEN 40

unsigned char g_rxbuf[MAXLEN];
volatile bool flag_tx_busy = false;
volatile bool flag_rx_ne = false;
static int n = 0;

void putchar(unsigned char c)
{
    unsigned short timeout = 1000;

    while(flag_tx_busy == true && timeout--)
        ;
    flag_tx_busy = true;
    S0BUF = c;
//    return 0;
}

UART_IRQHandler()
{
    if(S0CON & _BV(0)) {    // rx irq
        S0CON &= ~_BV(0);
        putchar(S0BUF);
//        g_rxbuf[n] = S0BUF;
    }
    if(S0CON & _BV(1)) {    // tx irq
        S0CON &= ~_BV(1);
        flag_tx_busy = false;
    }
}
