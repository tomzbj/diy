#ifndef _HC595_H
#define _HC595_H

typedef struct {
    unsigned char (*writebyte_f)(unsigned char);
    void (*cs_f)(int);
    void (*delay_f)(volatile int);
} hc595_cfg_t;

void HC595_Config(hc595_cfg_t* cfg);
void HC595_WriteByte(unsigned char data);
void HC595_Write(void* buf, int nbytes);

#endif
