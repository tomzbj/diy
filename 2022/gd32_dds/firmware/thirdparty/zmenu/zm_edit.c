#include <string.h>
#include "zm_edit.h"

//#include "ugui.h"
//#include "misc.h" // debug

#if USE_XPRINTF
#include "xprintf.h"
#define sprintf xsprintf
#else
#include <stdio.h>
#endif

enum {
    ZE_TYPE_INTEGER, ZE_TYPE_FLOAT
};

static struct {
    void* pvar;
    int cur_var, low_lim, hi_lim, n_digits, type, cursor_pos;
    const char* name;
    void (*save_f)(void);
    void (*discard_f)(void);
} g;

void zm_edit_init(void (*save_f)(void), void (*discard_f)(void))
{
    g.save_f = save_f;
    g.discard_f = discard_f;
}

void zm_edit_set_int(void* pvar, const char* name, int low_lim, int hi_lim,
    int n_digits)
{
    g.pvar = pvar;
    g.low_lim = low_lim;
    g.hi_lim = hi_lim;
    g.n_digits = n_digits;
    g.type = ZE_TYPE_INTEGER;
    g.name = name;
    g.cursor_pos = -2;
    memcpy(&(g.cur_var), g.pvar, sizeof(g.cur_var));
}

const char* zm_edit_get_name(void) //int ypos, int* current, int* checked)
{
    return g.name;
}

const void* zm_edit_get_ptr(void)
{
    return g.pvar;
}

#define abs(x) (((x)>0)?(x):(-x))
const char* zm_edit_get_val(void) //int ypos, int* current, int* checked)
{
    static char buf[32];
    if(g.type == ZE_TYPE_INTEGER) {
        char fmt[8];
        char sign;
        if(g.cur_var > 0)
            sign = '+';
        else if(g.cur_var < 0)
            sign = '-';
        else
            sign = ' ';
        sprintf(fmt, "%c%%0%dd", sign, g.n_digits);
        sprintf(buf, fmt, abs(g.cur_var));
    }
    return buf;
}

void zm_edit_op(int op)
{
    int n = 1;
    for(int i = 0; i < g.cursor_pos; i++)
        n *= 10;
    switch(op) {
        case ZE_OP_DEC:
            if(g.cursor_pos >= 0) {
                g.cur_var -= n;
                if(g.cur_var < g.low_lim)
                    g.cur_var = g.low_lim;
            }
            break;
        case ZE_OP_INC:
            if(g.cursor_pos >= 0) {
                g.cur_var += n;
                if(g.cur_var > g.hi_lim)
                    g.cur_var = g.hi_lim;
            }
            else if(g.cursor_pos == ZE_POS_SAVE) {
                memcpy(g.pvar, &(g.cur_var), sizeof(g.cur_var));
                g.save_f();
            }
            else if(g.cursor_pos == ZE_POS_DISCARD) {
                g.discard_f();
            }
            break;
        case ZE_OP_LEFT:
            g.cursor_pos++;
            if(g.cursor_pos >= g.n_digits)
                g.cursor_pos = -2;
            break;
        case ZE_OP_RIGHT:
            g.cursor_pos--;
            if(g.cursor_pos < -2)
                g.cursor_pos = g.n_digits - 1;
            break;
    }
}

int zm_edit_get_cursor_pos(void)
{
    return g.cursor_pos;
}
