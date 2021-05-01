#ifndef _SSD1306_I2C_H
#define _SSD1306_I2C_H

#include "misc.h"
#include "zi2c.h"

void SSD1306_DisplayOn(void);
void SSD1306_DisplayOff(void);
void SSD1306_Init(zi2c_t* i2c);
void SSD1306_Clear(unsigned char data);
void SSD1306_Write(unsigned char* buf, int size);
void SSD1306_SetBrightness(unsigned char brightness);
void SSD1306_SetContrast(unsigned char contrast);

#endif
