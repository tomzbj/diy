#include "misc.h"

void TIMER_Config(void)
{
    TCNT2 = 0x06; //setup
    OCR2A = 0xFA;
    OCR2B = 0x00;
    TCCR2A = 0x00;
    TCCR2B = 0x03; //start
    TIMSK2 = 0x1;
}

