#include "platform.h"

enum {
    ZSS_MODE_RAW, ZSS_MODE_NUM
};
static const unsigned char masks[10] = {    //
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f    //
    };

static struct {
    zss_cfg_t cfg;
    int num;
    unsigned char mode, dot_pos;
    unsigned char* raw;
} g;

void zss_set_num(int num)
{
    g.mode = ZSS_MODE_NUM;
    g.num = num;
}

void zss_update(void)
{
    static int curr_digit = 0;
    int i, j;
    static int num;

    for(i = 0; i < g.cfg.digits; i++)
        g.cfg.set_digit_f(i, 1);    // shut down all digits

    switch(g.mode) {
        case ZSS_MODE_NUM:
            if(curr_digit == 0)
                num = g.num;
            else
                num /= 10;

            for(j = 0; j < g.cfg.segments; j++) {
                if(masks[num % 10] & (1 << j))
                    g.cfg.set_segment_f(j, 1);
                else
                    g.cfg.set_segment_f(j, 0);
            }
            if(curr_digit == g.dot_pos)
                g.cfg.set_segment_f(7, 1);
            else
                g.cfg.set_segment_f(7, 0);
            g.cfg.set_digit_f(g.cfg.digits - curr_digit - 1, 0);    // reversed
            break;
        case ZSS_MODE_RAW:
            for(j = 0; j < g.cfg.segments; j++) {
                if(g.raw[curr_digit] & (1 << j))
                    g.cfg.set_segment_f(j, 1);
                else
                    g.cfg.set_segment_f(j, 0);
            }
            g.cfg.set_digit_f(curr_digit, 0);
            break;
    }
    curr_digit++;
    curr_digit %= g.cfg.digits;
}

void zss_set_dot(int pos)
{
    g.dot_pos = pos;
}

void zss_set_raw(unsigned char* raw)
{
    g.raw = raw;
    g.mode = ZSS_MODE_RAW;
}

void zss_init(zss_cfg_t* cfg)
{
    g.cfg = *cfg;
}
