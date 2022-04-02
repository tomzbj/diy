#ifndef _FM1906_H
#define _FM1906_H

typedef struct {
    void (*cs_f)(int state);
    void (*wr_f)(int state);
//    void (*rd_f)(int state);
    void (*data_f)(int state);
} fm1906_cfg_t;

void FM1906_Init(fm1906_cfg_t* cfg);
void FM1906_WriteData(unsigned char addr, unsigned char data[], int count);

#endif
