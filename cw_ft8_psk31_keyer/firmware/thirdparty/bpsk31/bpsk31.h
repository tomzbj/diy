#ifndef __BPSK31_H__
#define __BPSK31_H__

typedef struct {
    void (*tx_start_f)(void);
    void (*tx_symbol_f)(unsigned short data);
    void (*tx_char_f)(int c);
    void (*tx_end_f)(void);
} bpsk31_cbs_t;

void bpsk31_poll(void);//uint16_t blocksize);
void bpsk31_tx(const unsigned char* msg, int blocksize);
void bpsk31_init(bpsk31_cbs_t* cbs);

#endif 
