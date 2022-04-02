#ifndef _ZSELECT_H
#define _ZSELECT_H

#define USE_XPRINTF 0

typedef enum {
    ZS_OP_INC, ZS_OP_DEC, ZS_OP_CANCEL, ZS_OP_SAVE
} zs_op_t;

void zs_init(int items_per_page, void (*save_f)(void));
void zs_set_val(void* pvar, void* vals, int item_size, int n_items,
        const char* suffix);
void zs_set_string(void* pvar, void* vals, int item_size,
        const unsigned char* names[], int n_items);

void zs_op(int op);
const unsigned char* zs_get_item(int ypos, int* current, int* checked);

#endif
