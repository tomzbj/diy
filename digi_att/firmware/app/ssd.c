#include "misc.h"
#include "platform.h"

typedef struct {
    typeof(PORTB)* portx;
    int pin;
} gpio_t;

const gpio_t segments[7] = { {&PORTB, 6}, {&PORTB, 1}, {&PORTB, 0}, {&PORTD, 6},
    {&PORTD, 7}, {&PORTD, 4}, {&PORTD, 5}};

const gpio_t digits[2] = { {&PORTB, 2}, {&PORTD, 3}};

static void set_digit(unsigned int digit, int state)
{
    if(digit < 2) {
        if(state)
            *(digits[digit].portx) |= (1UL << digits[digit].pin);
        else
            *(digits[digit].portx) &= ~(1UL << digits[digit].pin);
    }
}

static void set_segment(unsigned int segment, int state)
{
    if(segment < 7) {
        if(state)
            *(segments[segment].portx) |= (1UL << segments[segment].pin);
        else
            *(segments[segment].portx) &= ~(1UL << segments[segment].pin);
    }
}

void SSD_Config(void)
{
    DDRB |= 0xc7;    // PB01267
    DDRD |= 0xf8;    // PD34567
    {
        static zss_cfg_t cfg;
        cfg.set_digit_f = set_digit;
        cfg.set_segment_f = set_segment;
        cfg.digits = 2;
        cfg.segments = 7;
        zss_init(&cfg);
    }
    zss_set_num(23);
//    zss_set_raw("\xff\xff");
}
