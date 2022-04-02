#include "misc.h"

const static struct {
    typeof(PORTB)* port_on, * port_off;
    int pin_on, pin_off;
} rel[4] = { {&PORTD, &PORTC, PD2, PC5}, {&PORTC, &PORTC, PC4, PC3}, {&PORTC,
    &PORTC, PC2, PC1}, {&PORTC, &PORTB, PC0, PB5}};

void REL_Set(int rel_id, int state)
{
    if(rel_id >= 4 || rel_id < 0)
        return;
    if(state) {
        *(rel[rel_id].port_on) |= _BV(rel[rel_id].pin_on);
        _delay_ms(3);
        *(rel[rel_id].port_on) &= ~_BV(rel[rel_id].pin_on);
        _delay_ms(3);
    }
    else {
        *(rel[rel_id].port_off) |= _BV(rel[rel_id].pin_off);
        _delay_ms(3);
        *(rel[rel_id].port_off) &= ~_BV(rel[rel_id].pin_off);
        _delay_ms(3);
    }
}

void REL_Config(void)
{
    DDRD |= _BV(PD2);
    DDRC |= 0x3f;
    DDRB |= _BV(PB5);

//    PORTC &= ~_BV(PC4); PORTC |= _BV(PC3); while(1) { PORTC ^= _BV(PC4); PORTC ^= _BV(PC3); _delay_ms(10); }
}
