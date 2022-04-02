#include "misc.h"

const unsigned char mask[] = {0x1, 0x8, 0x5, 0x14, 0x15, 0x1c};
enum {
    STATE_POWERDOWN, STATE_RANDOM, STATE_FREEZE, STATE_MAX = STATE_FREEZE
};
volatile int state;

void bsp_init(void)
{
    DDRB = _bitval(0) | _bitval(2) | _bitval(3) | _bitval(4);
    PORTB = _bitval(1);
    TCCR0B = _bitval(CS01) | _bitval(CS00);     // 600k / 8 / 75 = 1kHz
    TIMSK0 = _bitval(TOIE0);
    GIMSK |= _bitval(INT0);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sei();
}

ISR( INT0_vect)
{
    _delay_ms(3);
    while(!(PINB & _bitval(PB1)));
    _delay_ms(3);
    state++;
    if(state > STATE_MAX)
        state = STATE_POWERDOWN;
}

ISR( TIM0_OVF_vect)
{
    if(state == STATE_RANDOM)
        PORTB = mask[rand() % 6] | _bitval(1);
}

int main(void)
{
    bsp_init();
    while(1) {
        if(state == STATE_POWERDOWN) {
            PORTB = _bitval(1);
            TIMSK0 &= ~_bitval(TOIE0);
            sleep_enable();
            sleep_cpu();
            sleep_disable();
        }
        else
            TIMSK0 |= _bitval(TOIE0);
    };
    return 0;
}
