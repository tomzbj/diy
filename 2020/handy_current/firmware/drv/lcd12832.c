#include <string.h>
#include "lcd12832.h"

static lcd12832_config_t g;

static inline void WriteCmd(unsigned char cmd)
{
    g.rs_f(0);
    g.cs_f(0);
    g.fastwrite_f(&cmd, 1);
    g.cs_f(1);
}

static inline void WriteData(unsigned char* buf, int size)
{
    g.rs_f(1);
    g.cs_f(0);
    g.fastwrite_f(buf, size);
    g.cs_f(1);
}

static inline void SetXy(unsigned char x, unsigned char y)
{
    WriteCmd(0xb0 + y);
    WriteCmd(0x10 + (x >> 4));
    WriteCmd(x & 0xf);
}

void LCD12832_Clear(unsigned char pattern)
{
    unsigned char buf[132];
    memset(buf, pattern, 132);
    for(int j = 0; j < 8; j++) {
        SetXy(0, j);
        WriteData(buf, 132);
    }
}

void LCD12832_Write(unsigned char* buf, int size)
{
    for(int j = 0; j < 8; j++) {
        SetXy(0, j);
        WriteData(buf, 128);
        buf += 128;
    }
}

void LCD12832_Init(lcd12832_config_t* cfg)
{
    g = *cfg;

    g.reset_f(0);
    _delay_ms_loop(50);
    g.reset_f(1);
    _delay_ms_loop(50);
    WriteCmd(0xe2);
    _delay_ms_loop(20);  // soft reset
    WriteCmd(0xe2);
    _delay_ms_loop(20);  // soft reset
    WriteCmd(0x2c);
    _delay_ms_loop(5);   // boost step 1
    WriteCmd(0x2e);
    _delay_ms_loop(5);   // boost step 2
    WriteCmd(0x2f);
    _delay_ms_loop(5);   // boost step 3
    WriteCmd(0x20);                 // contrast coarse adjust
    WriteCmd(0xc1);                 // contrast fine adjust
    WriteCmd(0x10);                 // contrast value (0x00 ~ 0x3f)
    WriteCmd(0xa2);                 // 1/9 bias 
    WriteCmd(0xc8);                 // horizontal scan sequence: top to bottom
    WriteCmd(0xa0);                 // vertical scan sequence: left to right
    WriteCmd(0x40);                 // start row: 1st
    WriteCmd(0xaf);                 // display on
}
