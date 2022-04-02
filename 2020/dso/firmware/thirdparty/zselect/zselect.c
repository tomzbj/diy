#include <string.h>
#include "zselect.h"
#include "ugui.h"

#include "misc.h" // debug

#if USE_XPRINTF
#include "xprintf.h"
#define sprintf xsprintf
#else
#include <stdio.h>
#endif

enum {
    ZS_TYPE_INTEGER, ZS_TYPE_STRING
};

static struct {
    void* pvar;
    void* vals;
    const unsigned char** names;
    int n_items, items_per_page, pos, item_size, type;
    const char* suffix;
    void (*save_f)(void);
} g;

void zs_init(int items_per_page, void (*save_f)(void))
{
    g.items_per_page = items_per_page;
    g.save_f = save_f;
}

void zs_set_val(void* pvar, void* vals, int item_size, int n_items, const char* suffix)
{
    g.pvar = pvar;
    g.vals = vals;
    g.item_size = item_size;
    g.n_items = n_items;
    g.suffix = suffix;
    g.pos = 0;
    g.type = ZS_TYPE_INTEGER;
}

void zs_set_string(void* pvar, void* vals, int item_size, const unsigned char* names[], int n_items)
{
    g.pvar = pvar;
    g.vals = vals;
    g.item_size = item_size;
    g.n_items = n_items;
    g.names = names;
    g.suffix = "";
    g.pos = 0;
    g.type = ZS_TYPE_STRING;
}

const unsigned char* zs_get_item(int ypos, int* current, int* checked)
{
    int pos = g.pos;
    int i, ipos = 0;
    *current = 0;
    *checked = 0;
    static unsigned char buf[32];
    memset(buf, 0, 16);
    for(i = 0; i < g.n_items; i++) {
        if(pos < g.items_per_page) {      // cursor in this page
            if(ypos == 0) {
                if(pos == ypos)
                    *current = 1;
                if(memcmp(g.pvar, g.vals + i * g.item_size, g.item_size) == 0)
                    *checked = 1;
                if(g.type == ZS_TYPE_INTEGER) {
                    unsigned long val = 0;
                    memcpy(&val, g.vals + i * g.item_size, g.item_size);
                    sprintf((char*)buf, "%lu%s", val, g.suffix);
                }
                else if(g.type == ZS_TYPE_STRING) {
                    sprintf((char*)buf, "%s", g.names[i]);
                }
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
    return (const unsigned char*)"";
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
            memcpy(g.pvar, g.vals + g.pos * g.item_size, g.item_size);
            g.save_f();
            break;
        }
    }
}
