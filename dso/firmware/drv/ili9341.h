#ifndef _ILI9341_H
#define _ILI9341_H

typedef struct {
    void (*reset_f)(int);
    void (*cs_f)(int);
    void (*rs_f)(int);
    void (*fastwrite_f)(void*, int);
    void (*delay_us_f)(volatile int);
    int width, height;
} ili9341_cfg_t;

void ILI9341_Init(ili9341_cfg_t* pcfg);
void ILI9341_Clear(unsigned short c);
void ILI9341_SetWindow(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void ILI9341_Write(void* buf, int nbytes);

#endif
