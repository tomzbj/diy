#ifndef _ZM_H
#define _ZM_H

#define ZM_TOPLEVEL 255

typedef enum {
    ZM_OP_UP, ZM_OP_DOWN, ZM_OP_BACK, ZM_OP_RUN
} zm_op_t;

typedef unsigned char zm_id_t;

typedef struct {
    zm_id_t parent;         // 1byte
    signed char isdir;      // 1byte
    unsigned char cb_id;      // 1byte
    const char* name;
} zm_item_t;

void zm_init(void* buf, int max_items, int items_per_page,
    void (*action_f)(int action_id));
int zm_istoplevel(void);
void zm_op(zm_op_t op);
int zm_add_item(int parent, int isdir, const char* name, unsigned char cb_id);

const char* zm_get_item(int ypos, int* checked);

#endif
