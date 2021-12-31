#ifndef _ILI9341_H
#define _ILI9341_H

#define LCD_MODE_8BIT

#ifdef LCD_MODE_16BIT
typedef unsigned short lcd_data_t;
#else
typedef unsigned char lcd_data_t;
#endif

typedef struct {
    void (*readwrite_f)(lcd_data_t data);
    void (*fastwrite_f)(const void* buf, int count);
    void (*fastread_f)(void* buf, int count);

    void (*reset_f)(int res);
    void (*cs_f)(int cs);
    void (*rs_f)(int rs);

    void (*delay_us_f)(volatile int nus);
} ili9341_cfg_t;

void ILI9341_Init(ili9341_cfg_t* pcfg);

void ILI9341_WriteDataFast(lcd_data_t* buf, int count);
void ILI9341_ReadDataFast(lcd_data_t* buf, int count);
void ILI9341_WriteData(lcd_data_t data);
void ILI9341_WriteReg(lcd_data_t reg);

#endif
