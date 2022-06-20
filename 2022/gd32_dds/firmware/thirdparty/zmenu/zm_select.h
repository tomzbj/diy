#ifndef _ZSELECT_H
#define _ZSELECT_H

#define USE_XPRINTF 0

typedef enum {
    ZS_OP_INC, ZS_OP_DEC, ZS_OP_CANCEL, ZS_OP_SAVE
} zm_select_op_t;

void zm_select_init(int items_per_page, void (*save_f)(void));
void zm_select_set_val(void* pvar, const void* vals, int item_size, int n_items, const char* suffix);
void zm_select_set_string(void* pvar, const void* vals, int item_size, const char* names[], int n_items);

void zm_select_op(int op);
const char* zm_select_get_item(int ypos, int* current, int* checked);

#endif
