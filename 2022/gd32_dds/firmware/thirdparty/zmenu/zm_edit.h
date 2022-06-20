#ifndef _ZEELECT_H
#define _ZM_EDIT_H

#define USE_XPRINTF 0

enum {
    ZE_OP_INC, ZE_OP_DEC, ZE_OP_LEFT, ZE_OP_RIGHT
};

enum {
    ZE_POS_SAVE = -1, ZE_POS_DISCARD = -2
};

void zm_edit_init(void (*save_f)(void), void (*discard_f)(void));
void zm_edit_set_int(void* pvar, const char* name, int low_lim, int hi_lim,
    int n_digits);

void zm_edit_op(int op);
const char* zm_edit_get_val(void); //int ypos, int* current, int* checked)
const void* zm_edit_get_ptr(void);
const char* zm_edit_get_name(void);
int zm_edit_get_cursor_pos(void);

#endif
