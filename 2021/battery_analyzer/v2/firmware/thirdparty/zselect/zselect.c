#include <string.h>
#include <stdio.h>
#include "zselect.h"
//#include "ugui.h"
//#include "xprintf.h"
//#include "dataconfig.h"

enum {
    ZS_TYPE_INTEGER, ZS_TYPE_STRING
};

static struct {
    signed short* pvar;
    const signed short* vals;
    const char** names;
    int n_items;
    int items_per_page;
    int pos;
    const char* suffix;
    int type;
    void (*save_f)(void);
} g = {.items_per_page = 3};

void zs_init(int items_per_page, void (*save_f)(void))
{
    g.items_per_page = items_per_page;
    g.save_f = save_f;
}

void zs_set_integer(signed short* pvar, const signed short vals[], int n_items,
    const char* suffix)
{
    g.pvar = pvar;
    g.vals = vals;
    g.n_items = n_items;
    g.suffix = suffix;
    g.pos = 0;
    g.type = ZS_TYPE_INTEGER;
}

void zs_set_string(signed short* pvar, const signed short vals[],
    const char* names[], int n_items)
{
    g.pvar = pvar;
    g.vals = vals;
    g.n_items = n_items;
    g.names = names;
    g.suffix = "";
    g.pos = 0;
    g.type = ZS_TYPE_STRING;
}

const char* zs_get_item(int ypos, int* current, int* checked)
{
    int pos = g.pos;
    int i, ipos = 0;
    *current = 0;
    *checked = 0;
    static char buf[16];
    memset(buf, 0, 16);

    for(i = 0; i < g.n_items; i++) {
        if(pos < g.items_per_page) {      // cursor in this page
            if(ypos == 0) {
                if(pos == ypos)
                    *current = 1;
                if(g.vals[i] == *(g.pvar))
                    *checked = 1;
                if(g.type == ZS_TYPE_INTEGER)
                    sprintf(buf, "%d%s", g.vals[i], g.suffix);
                else if(g.type == ZS_TYPE_STRING)
                    sprintf(buf, "%s", g.names[i]);
                return buf;
            }
            else {
                ypos--;
                pos--;
                continue;
            }
        }
        else {
            ipos++;
            if(ipos >= g.items_per_page) {  // next page
                pos -= g.items_per_page;
                ipos -= g.items_per_page;
            }
            if(pos < 0)                 // out of range
                break;
        }
    }
    return "";
}

void zs_op(int op)
{
    switch(op) {
        case ZS_OP_DEC:
            if(g.pos > 0)
                g.pos--;
            break;
        case ZS_OP_INC:
            if(g.pos < g.n_items - 1)
                g.pos++;
            break;
        case ZS_OP_SAVE: {
            *(g.pvar) = g.vals[g.pos];
            g.save_f();
            break;
        }
    }
}
