#ifndef _FT8_H
#define _FT8_H


typedef struct {
    void (*tx_start_f)(void);
    void (*tx_symbol_f)(int c);
//    void (*tx_char_f)(int c);
    void (*tx_end_f)(void);
} ft8_cbs_t;

void ft8_tx(const char* msg, int len);
void ft8_poll(void);
void ft8_init(ft8_cbs_t* cbs);
void ft8_tx_repeat(void);    // for beacon mode

#endif
