#ifndef _ST7735_H
#define _ST7735_H

#include "misc.h"

typedef struct {
    void (*reset_f)(int);
    void (*cs_f)(int);
    void (*rs_f)(int);
//    unsigned char (*writebyte_f)(unsigned char);
    void (*fastwrite_f)(void*, int);
    void (*delay_us_f)(volatile int);
} st7735_cfg_t;

void ST7735_Init(st7735_cfg_t* cfg);

void ST7735_WriteData(unsigned char data);
void ST7735_WriteReg(unsigned char data);
void ST7735_WriteDataFast(unsigned char* buf, int nbytes);
void ST7735_ReadDataFast(unsigned char* buf, int nwords);

#endif
