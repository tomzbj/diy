#include "fm1906.h"

enum {
    C_SYS_DIS = 0, C_SYS_EN = 1, C_LEC_OFF = 2, C_LCD_ON = 3, C_TIMER_DIS = 4,
    C_WDT_DIS = 5, C_TIMER_EN = 6, C_WDT_EN = 7, C_TON_OFF = 8, C_TONE_ON = 9,
    CLR_TIMER = 0xc, C_CLR_WDT = 0xe, C_XTAL32K = 0x14, C_RC256K = 0x18,
    C_EXT256K = 0x1c, C_BIAS_1_2_2 = 0x20, C_BIAS_1_2_3 = 0x24,
    C_BIAS_1_2_4 = 0x28, C_BIAS_1_3_2 = 0x21, C_BIAS_1_3_3 = 0x25,
    C_BIAS_1_3_4 = 0x29, C_TONE_4K = 0x40, C_TONE_2K = 0x60, C_IRQ_DIS = 0x80,
    C_IRQ_EN = 0x88, C_F1 = 0xa0, C_F2 = 0xa1, C_F4 = 0xa2, C_F8 = 0xa3,
    C_F16 = 0xa4, C_F32 = 0xa5, C_F64 = 0xa6, C_F128 = 0xa7, C_TEST = 0xe0,
    C_NORMAL = 0xe3
};

static fm1906_cfg_t g;

static void write_raw(unsigned char data, int nbits)
{
    data <<= (8 - nbits);
    for(int i = 0; i < nbits; i++) {
        if(data & 0x80)
            g.data_f(1);
        else
            g.data_f(0);
        g.wr_f(0);
        g.wr_f(1);
        data <<= 1;
    }
}

static void write_cmd(unsigned char cmd)
{
    g.cs_f(0);
    write_raw(0x4, 3);
    write_raw(cmd, 8);
    write_raw(0, 1);
    g.cs_f(1);
}

void FM1906_WriteData(unsigned char addr, unsigned char data[], int count)
{
    g.cs_f(0);
    write_raw(0x5, 3);
    write_raw(addr, 6);
    for(int i = 0; i < count; i++)
        write_raw(data[i], 4);
    g.cs_f(1);
}

void FM1906_Init(fm1906_cfg_t* cfg)
{
    g = *cfg;
    g.cs_f(1);
    g.wr_f(1);
    write_cmd(C_SYS_EN);
    write_cmd(C_LCD_ON);
    write_cmd(C_BIAS_1_3_4);
}
