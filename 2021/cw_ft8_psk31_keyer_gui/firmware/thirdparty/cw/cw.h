#ifndef _CW_H
#define _CW_H

typedef struct {
    void (*tx_start_f)(void);
    void (*tx_symbol_f)(int c);
    void (*tx_char_f)(int c);
    void (*tx_end_f)(void);
} cw_cbs_t;

void cw_tx(const char* msg, int len);
void cw_poll(void);
void cw_init(cw_cbs_t* cbs);
void cw_tx_repeat(void);    // for beacon mode

#endif

