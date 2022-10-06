#ifndef _ZAPRS_H
#define _ZAPRS_H

typedef struct {
    void (*tx_start_f)(void);
    void (*tx_char_f)(int c);
    void (*tx_symbol_f)(char bit);
    void (*tx_phase_f)(unsigned short phase);
    void (*tx_end_f)(void);
} zaprs_cb_t;

void zaprs_init(zaprs_cb_t* cfg, const unsigned char* callsign);
void zaprs_tx(const unsigned char* msg);
void zaprs_poll(void);

#endif
