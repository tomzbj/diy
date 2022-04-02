#ifndef _BPSK_CBS_H
#define _BPSK_CBS_H

void bpsk_tx_start(void);
void bpsk_tx_char(int c);
void bpsk_tx_end(void);
void bpsk_tx_symbol(unsigned short data);

#endif
