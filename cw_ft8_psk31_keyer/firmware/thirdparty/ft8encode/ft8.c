#include "ft8.h"
#include "ft8encode.h"
#include <string.h>

#include "misc.h"

static struct {
    unsigned char curr, symbols[100];
    int pos, count;
    ft8_cbs_t cbs;
} g;

void ft8_tx(const char* msg, int len)
{
    ft8_encode(msg, g.symbols);
    g.count = ft8_get_symbol_count();
    g.pos = 0;
    g.curr = g.symbols[0];
printf("%p\n", g.cbs.tx_start_f);
    g.cbs.tx_start_f();
}

void ft8_poll(void)
{
    if(g.count) {
        g.cbs.tx_symbol_f(g.curr);
        g.pos++;
        g.curr = g.symbols[g.pos];
        g.count--;
        if(g.count == 0) {
            g.cbs.tx_end_f();
        }
    }
}

void ft8_init(ft8_cbs_t* cbs)
{
    g.cbs = *cbs;
}

void ft8_tx_repeat(void)    // for beacon mode
{
    g.pos = 0;
    g.count = ft8_get_symbol_count();;
    g.curr = g.symbols[0];
}
