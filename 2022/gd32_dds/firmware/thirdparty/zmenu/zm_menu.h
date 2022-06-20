#ifndef _ZM_H
#define _ZM_H

#define ZM_TOPLEVEL 255

enum {
    ZM_OP_UP, ZM_OP_DOWN, ZM_OP_BACK, ZM_OP_RUN
};

enum {
    ZM_ITEM_DIR, ZM_ITEM_SELECT, ZM_ITEM_EDIT
};


void zm_init(void* buf, int max_items, int items_per_page,
    void (*action_f)(int action_type, int action_id));
int zm_istoplevel(void);
void zm_op(int op);
int zm_add_item(int parent, int type, const void* name, unsigned char item_id);

const unsigned char* zm_get_item(int ypos, int* checked);

#endif
