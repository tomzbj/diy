#include "st7735.h"

static st7735_cfg_t g;

// 写指令 
void ST7735_WriteReg(unsigned char data)
{
    g.cs_f(0);
    g.rs_f(0);
    g.fastwrite_f(&data, 1);
    g.cs_f(1);
}

// 写8位数据
void ST7735_WriteData(unsigned char data)
{
    g.cs_f(0);
    g.rs_f(1);
    g.fastwrite_f(&data, 1);
    g.cs_f(1);
}

void ST7735_ReadDataFast(unsigned char* buf, int nwords)
{
}

void ST7735_WriteDataFast(unsigned char* buf, int nbytes)
{
    g.cs_f(0);
    g.rs_f(1);
    g.fastwrite_f(buf, nbytes);
    g.cs_f(1);
}

void ST7735_Init(st7735_cfg_t* cfg)
{
    g = *cfg;
    g.reset_f(0);
    g.delay_us_f(100 * 1000);
    g.reset_f(1);
    g.delay_us_f(50 * 1000);

    //开始初始化液晶屏
    ST7735_WriteReg(0x11);
    g.delay_us_f(120 * 1000);
    const unsigned short init_seq[] = {0x8011, 0x8026, 0x04, 0x80f2, 0x00,
        0x80b1, 0x0c, 0x14, 0x80c0, 0x0c, 0x05, 0x80c1, 0x02, 0x80c5, 0x32,
        0x3b, 0x80c7, 0x40, 0x802a, 0x00, 0x00, 0x00, 0x7f, 0x802b, 0x00, 0x00,
        0x00, 0x7f, 0x8036, 0xa8, 0x803a, 0x55, 0x8029};    // for 128x128 parallel tftlcd
    for(int i = 0; i < sizeof(init_seq) / sizeof(init_seq[0]); i++) {
        if(init_seq[i] & 0x8000) {
            ST7735_WriteReg(init_seq[i] & 0xff);
        }
        else
            ST7735_WriteData(init_seq[i]);
    }
//    ST7735_WriteReg(0x36);
//    ST7735_WriteData(0x78);
//    WriteReg(0x36, 0x78);
}
