#include <avr/io.h>
#include <avr/interrupt.h>

ISR( TIM0_OVF_vect)
{
    static int k = 0, n = 0;
    const unsigned long v = 0x01d5;    //0xab80;
    k++;
    k %= 50;
    if(k == 0) {
        n++;
        n %= 24;
        if(v & _BV(n)) {
            TCCR0A = 0x83;    //beep
            PORTB |= _BV(PB3);
        }
        else {
            TCCR0A = 0x03;    //nobeep
            PORTB &= ~_BV(PB3);
        }
    }
}

int main(void)
{
    DDRB = _BV(PB0) | _BV(PB3);
    TCCR0B = 0x00;    //stop //    timer0_init();
    TCCR0A = 0x83;
    TCCR0B = 0x02;    //start timer
    OCR0A = 0x80;    // duty 50%
    MCUCR = 0x00;
    TIMSK0 = 0x02;    //timer interrupt sources
    GIMSK = 0x00;    //interrupt sources
    sei();    //re-enable interrupts
    while(1);
    return 0;
}
