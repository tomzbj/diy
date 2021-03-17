#include <string.h>
#include "zmenu.h"

#define POS_STACK_DEPTH 8

static struct {
    zm_item_t *item;
    int count, max;
    unsigned char currparent;   // parent item id of current item
    unsigned char pos;
    unsigned char pos_stack[POS_STACK_DEPTH];   // stack for position
    unsigned char pos_stack_ptr;
    unsigned char items_per_page;
    void (*action_f)(int action_id);
} g;

void zm_init(void* buf, int max_items, int items_per_page, void (*action_f)(int action_id))
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

int zm_add_item(int parent, int isdir, const char* name, unsigned char cb_id)
{
    int ret;
    g.item[g.count].parent = parent;
    g.item[g.count].isdir = isdir;
    g.item[g.count].name = name;
    g.item[g.count].cb_id = cb_id;
    ret = g.count;
    g.count++;
    return ret;
}

static int get_count_by_parent(int parent)
{
    int i, count = 0;
    for(i = 0; i < g.count; i++) {
        if(g.item[i].parent == parent && strlen(g.item[i].name))
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

int zm_istoplevel(void)
{
    if(g.currparent == ZM_TOPLEVEL)
        return 1;
    else
        return 0;
}

void zm_op(zm_op_t op)
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
            if(g.currparent != ZM_TOPLEVEL)
                g.currparent = g.item[g.currparent].parent;
            if(g.pos_stack_ptr > 0) {
                g.pos_stack_ptr--;
                g.pos = g.pos_stack[g.pos_stack_ptr];
            }
            else
                g.pos = 0;
            break;
        case ZM_OP_RUN: {
            int index = get_index_by_parent_pos(g.currparent, g.pos);
            if(g.item[index].isdir > 0) {
                g.currparent = index;
                g.pos_stack[g.pos_stack_ptr] = g.pos;
                g.pos_stack_ptr++;
                g.pos = 0;
            }
            else {
                g.action_f(g.item[index].cb_id);
            }
        }
            break;
    }
}

const char* zm_get_item(int ypos, int* checked)
{
    int pos = g.pos;
    int i, ipos = 0;
    *checked = 0;

    for(i = 0; i < g.count; i++) {
        if(g.item[i].parent == g.currparent && strlen(g.item[i].name)) {
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
    return "";
}
