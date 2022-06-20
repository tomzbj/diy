#include <string.h>
#include "zm_menu.h"

#define POS_STACK_DEPTH 8

typedef struct {
    signed char type;      // 1byte
    unsigned char parent;         // 1byte
    unsigned char id;      // 1byte
    const unsigned char* name;
} zm_item_t;

static struct {
    zm_item_t* item;
    int count, max;
    unsigned char currparent;   // parent item id of current item
    unsigned char pos;
    unsigned char pos_stack[POS_STACK_DEPTH];   // stack for position
    unsigned char pos_stack_ptr;
    unsigned char items_per_page;
    void (*action_f)(int action_type, int action_id);
} g;

void zm_init(void* buf, int max_items, int items_per_page,
    void (*action_f)(int action_type, int action_id))
{
    g.item = buf;
    g.max = max_items;
    g.items_per_page = items_per_page;
    g.count = 0;
    g.currparent = ZM_TOPLEVEL;
    g.pos = 0;
    g.pos_stack_ptr = 0;
    g.action_f = action_f;
}

int zm_add_item(int parent, int type, const void* name, unsigned char id)
{
    int ret;
    g.item[g.count].parent = parent;
    g.item[g.count].type = type;
    g.item[g.count].name = (const unsigned char*)name;
    g.item[g.count].id = id;
    ret = g.count;
    g.count++;
    return ret;
}

static int get_count_by_parent(int parent)
{
    int i, count = 0;
    for(i = 0; i < g.count; i++) {
        if(g.item[i].parent == parent && strlen((char*)g.item[i].name) > 0)
            count++;
    }
    return count;
}

static int get_index_by_parent_pos(int parent, int pos)
{
    int i, ipos = 0;
    for(i = 0; i < g.count; i++) {
        if(g.item[i].parent == parent) {
            if(pos == ipos)
                return i;
            ipos++;
        }
    }
    return -1;
}

#if 0
static int get_id_by_parent_pos(int parent, int pos)
{
    int i, ipos = 0;
    for(i = 0; i < g.count; i++) {
        if(g.item[i].parent == parent) {
            if(pos == ipos)
                return g.item[i].id;
            ipos++;
        }
    }
    return -1;
}
#endif

int zm_istoplevel(void)
{
    if(g.currparent == ZM_TOPLEVEL)
        return 1;
    else
        return 0;
}

void zm_op(int op)
{
    switch(op) {
        case ZM_OP_UP:
            if(g.pos > 0)
                g.pos--;
            break;
        case ZM_OP_DOWN:
            if(g.pos < get_count_by_parent(g.currparent) - 1)
                g.pos++;
            break;
        case ZM_OP_BACK:
            if(g.currparent != ZM_TOPLEVEL) {
                for(int i = 0; i < g.count; i++) {
                    if(g.item[i].id == g.currparent)
                        g.currparent = g.item[i].parent;
                }
            }
            if(g.pos_stack_ptr > 0) {
                g.pos_stack_ptr--;
                g.pos = g.pos_stack[g.pos_stack_ptr];
            }
            else
                g.pos = 0;
            break;
        case ZM_OP_RUN: {
            int index = get_index_by_parent_pos(g.currparent, g.pos);
            if(g.item[index].type == ZM_ITEM_DIR) {
                g.currparent = g.item[index].id;
                g.pos_stack[g.pos_stack_ptr] = g.pos;
                g.pos_stack_ptr++;
                g.pos = 0;
            }
            else {
                g.action_f(g.item[index].type, g.item[index].id);
            }
        }
            break;
    }
}

const unsigned char* zm_get_item(int ypos, int* checked)
{
    int pos = g.pos;
    int i, ipos = 0;
    *checked = 0;

    for(i = 0; i < g.count; i++) {
        if(g.item[i].parent == g.currparent
            && strlen((char*)g.item[i].name) > 0) {
            if(pos < g.items_per_page) {      // cursor in this page
                if(ypos == 0) {
                    if(pos == ypos)
                        *checked = 1;
                    return g.item[i].name;
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
    }
    return (const unsigned char*)"";
}
