/// @file lcd12832.h
/// @brief 
/// @author Zhang Hao, stavrosatic@gmail.com
/// @version R0.1
/// @date 2017-10-12
#ifndef _LCD12832_H
#define _LCD12832_H

#include "misc.h"

typedef void (*lcd12832_writebyte_func_t)(unsigned char);
typedef void (*lcd12832_cs_func_t)(int);
typedef void (*lcd12832_rs_func_t)(int);
typedef void (*lcd12832_reset_func_t)(int);
typedef void (*lcd12832_fastwrite_func_t)(unsigned char* buf, int nbytes);
typedef struct {
    lcd12832_cs_func_t cs_f;
    lcd12832_rs_func_t rs_f;
    lcd12832_reset_func_t reset_f;
    lcd12832_fastwrite_func_t fastwrite_f;
} lcd12832_config_t;

void LCD12832_Clear(unsigned char byte);
void LCD12832_Init(lcd12832_config_t* pcfg);
void LCD12832_Line(unsigned char col, unsigned char page, unsigned char byte);
void LCD12832_SetInverse(int inverse);
void LCD12832_Write(unsigned char* buf, int size);

#endif
