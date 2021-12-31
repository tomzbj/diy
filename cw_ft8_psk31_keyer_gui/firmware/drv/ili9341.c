#include "ili9341.h"
#include "misc.h"

static ili9341_cfg_t g;

void ILI9341_WriteReg(lcd_data_t reg)
{
    g.cs_f(0);
    g.rs_f(0);
    g.readwrite_f(reg);
    g.cs_f(1);
}

void ILI9341_WriteData(lcd_data_t data)
{
    g.cs_f(0);
    g.rs_f(1);
    g.readwrite_f(data);
    g.cs_f(1);
}

void ILI9341_ReadDataFast(lcd_data_t* buf, int count)
{
    g.cs_f(0);
    g.rs_f(1);
    g.fastread_f(buf, count);
    g.cs_f(1);
}

void ILI9341_WriteDataFast(lcd_data_t* buf, int count)
{
    g.cs_f(0);
    g.rs_f(1);
    g.fastwrite_f(buf, count);
    g.cs_f(1);
}

void ILI9341_Init(ili9341_cfg_t* cfg)
{
    g = *cfg;
    unsigned char cmd_list[] = {
        //
        0xCB, 0x39, 0x2C, 0x00, 0x34, 0x02, 0xCF, 0x00, 0XC1, 0X30, 0xE8, 0x85,
        0x00, 0x78, 0xEA, 0x00, 0x00, 0xED, 0x64, 0x03, 0X12, 0X81, 0xF7, 0x20,
        0xC0, 0x23, 0xC1, 0x10, 0xC5, 0x3e, 0x28, 0xC7, 0x86, 0x36, 0x48, 0x3A,
        0x55, 0xB1, 0x00, 0x18, 0xB6, 0x08, 0x82, 0x27, 0xF2, 0x00, 0x26, 0x01,
        0xE0, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10,
        0x03, 0x0E, 0x09, 0x00, 0XE1, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31,
        0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F, 0x11    //
        };
    unsigned char reg_list[] = {0, 6, 10, 14, 17, 22, 24, 26, 28, 31, 33, 35,
        37, 40, 44, 46, 48, 64, 80};
    g.reset_f(1);
    g.delay_us_f(2000);
    g.reset_f(0);
    g.delay_us_f(2000);
    g.reset_f(1);
    g.delay_us_f(2000);
    int i, k;

    for(i = 0, k = 0; i < sizeof(cmd_list); i++) {
        if(reg_list[k] == i) {
            ILI9341_WriteReg(cmd_list[i]);
            k++;
        }
        else
            ILI9341_WriteData(cmd_list[i]);
    }
    g.delay_us_f(200000UL);
    ILI9341_WriteReg(0x36);
    ILI9341_WriteData(0x28);
    ILI9341_WriteReg(0x29);            //Display on
    ILI9341_WriteReg(0x2c);
}
