#ifndef _HC595_H
#define _HC595_H

typedef struct {
    void (*writebyte_f)(unsigned char);
    void (*fastwrite_f)(void* buf, int nbytes);
    void (*cs_f)(int);
    void (*delay_f)(volatile int);
} hc595_cfg_t;

void HC595_Init(hc595_cfg_t* cfg);
void HC595_WriteByte(unsigned char data);
void HC595_Write(void* buf, int nbytes);

#endif
