#ifndef _ZSELECT_H
#define _ZSELECT_H

typedef enum {
    ZS_OP_INC, ZS_OP_DEC, ZS_OP_CANCEL, ZS_OP_SAVE
} zs_op_t;

void zs_init(int items_per_page, void (*save_f)(void));
void zs_set_integer(signed short* pvar, const signed short* vals, int n_items,
    const char* suffix);

void zs_set_string(signed short* pvar, const signed short vals[],
    const char* names[], int n_items);

void zs_op(int op);
const char* zs_get_item(int ypos, int* current, int* checked);

#endif
