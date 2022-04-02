#include <stdio.h>
#include "si5351.h"

#define  CMD_WRITE (g.addr << 1)
#define  CMD_READ (CMD_WRITE + 1)

#define REG_CLK_CTRL(x) (16 + (x)) // x=0..7
#define REG_PLLA_PARAMS(x) (26 + (x)) // x=0..7
#define REG_PLLB_PARAMS(x) (34 + (x)) // x=0..7
#define REG_MS_PARAMS(x) (42 + (x) * 8) // x=0..5
#define REG_SSC_PARAMS_BASE (149)
#define REG_SSC_PARAMS(x) (REG_SSC_PARAMS_BASE + (x))   // x=0..12
#define REG_CLK_PHASE_OFS(x) (165 + (x))  // x=0..5

enum {
    REG_DEVICE_STATUS = 0,
    REG_INTERRUPT_STATUS = 1,
    REG_INTERRUPT_MASK = 2,
    REG_OUTPUT_ENABLE_CTRL = 3,
    REG_OEB_PIN_ENABLE_CTRL = 9,
    REG_PLL_INPUT_SOURCE = 15,
    REG_CLK3_0_DISABLE_STATE = 24,
    REG_CLK7_4_DISABLE_STATE = 25,
    REG_CLK6_7_OUTPUT_DIVIDER = 92,
    REG_VXCO_PARAMETERS_LOW = 162,
    REG_VXCO_PARAMETERS_MID = 163,
    REG_VXCO_PARAMETERS_HIGH = 164,
    REG_PLL_RESET = 177,
    REG_CRYSTAL_LOAD = 183,
    REG_FANOUT_ENABLE = 187,
};

enum {
    REG_STATUS_SYS_INIT = 0x80,
    REG_STATUS_LOL_B = 0x40,
    REG_STATUS_LOL_A = 0x20,
    REG_STATUS_LOS = 0x10,
    REG_CLKIN_DIV_MASK = 0xc0,
    REG_CLKIN_DIV_1 = 0x00,
    REG_CLKIN_DIV_2 = 0x40,
    REG_CLKIN_DIV_4 = 0x80,
    REG_CLKIN_DIV_8 = 0xc0,
    REG_PLLB_SOURCE = 0x08,
    REG_PLLA_SOURCE = 0x04,
    REG_CLK_POWERDOWN = 0x80,
    REG_CLK_INTEGER_MODE = 0x40,
    REG_CLK_PLL_SELECT = 0x20,
    REG_CLK_INVERT = 0x10,
    REG_CLK_INPUT_MASK = 0x0c,
    REG_CLK_INPUT_XTAL = 0x00,
    REG_CLK_INPUT_CLKIN = 0x04,
    REG_CLK_INPUT_MULTISYNTH_0_4 = 0x08,
    REG_CLK_INPUT_MULTISYNTH_N = 0x0c,
    REG_CLK_DRIVE_STRENGTH_MASK = (3 << 0),
    REG_CLK_DRIVE_STRENGTH_2MA = (0 << 0),
    REG_CLK_DRIVE_STRENGTH_4MA = (1 << 0),
    REG_CLK_DRIVE_STRENGTH_6MA = (2 << 0),
    REG_CLK_DRIVE_STRENGTH_8MA = (3 << 0),
    REG_CLK_DISABLE_STATE_MASK = 3,
    REG_CLK_DISABLE_STATE_LOW = 0,
    REG_CLK_DISABLE_STATE_HIGH = 1,
    REG_CLK_DISABLE_STATE_FLOAT = 2,
    REG_CLK_DISABLE_STATE_NEVER = 3,
    REG_PARAMETERS_LENGTH = 8,
    REG_OUTPUT_CLK_DIV_MASK = (7 << 4),
    REG_OUTPUT_CLK6_DIV_MASK = (7 << 0),
    REG_OUTPUT_CLK_DIV_SHIFT = 4,
    REG_OUTPUT_CLK_DIV6_SHIFT = 0,
    REG_OUTPUT_CLK_DIV_1 = 0,
    REG_OUTPUT_CLK_DIV_2 = 1,
    REG_OUTPUT_CLK_DIV_4 = 2,
    REG_OUTPUT_CLK_DIV_8 = 3,
    REG_OUTPUT_CLK_DIV_16 = 4,
    REG_OUTPUT_CLK_DIV_32 = 5,
    REG_OUTPUT_CLK_DIV_64 = 6,
    REG_OUTPUT_CLK_DIV_128 = 7,
    REG_OUTPUT_CLK_DIVBY4 = (3 << 2),
    REG_PLL_RESET_B = 0x80,
    REG_PLL_RESET_A = 0x20,
    REG_CRYSTAL_LOAD_MASK = 0xc0,
    REG_CRYSTAL_LOAD_6PF = 0x40,
    REG_CRYSTAL_LOAD_8PF = 0x80,
    REG_CRYSTAL_LOAD_10PF = 0xc0,
    REG_CLKIN_ENABLE = 0x80,
    REG_XTAL_ENABLE = 0x40,
    REG_MULTISYNTH_ENABLE = 0x10
};

static zi2c_t g;

static void WriteReg(unsigned char reg, unsigned char data)
{
    g.start_f(&g);
    g.writebyte_f(&g, CMD_WRITE);
    g.writebyte_f(&g, reg);
    g.writebyte_f(&g, data);
    g.stop_f(&g);
}

static void WriteData(unsigned char reg, unsigned char* data, int nbytes)
{
    int i;

    g.start_f(&g);
    g.writebyte_f(&g, CMD_WRITE);
    g.writebyte_f(&g, reg);
    for(i = 0; i < nbytes; i++) {
        g.writebyte_f(&g, data[i]);
    }
    g.stop_f(&g);
}

unsigned char ReadReg(unsigned char reg)
{
    unsigned char ret;
    g.start_f(&g);
    g.writebyte_f(&g, CMD_WRITE);
    g.writebyte_f(&g, reg);
    g.start_f(&g);
    g.writebyte_f(&g, CMD_READ);
    ret = g.readbyte_nack_f(&g);
    g.stop_f(&g);
    return ret;
}

void SI5351_Init(zi2c_t* cfg)
{
    g = *cfg;
    WriteReg(REG_CRYSTAL_LOAD, REG_CRYSTAL_LOAD_6PF);
    WriteReg(REG_OUTPUT_ENABLE_CTRL, 0x00);    // enable ms0~ms2
    WriteReg(REG_PLL_RESET, 0xa0);
}

int SI5351_SetPLL(int pllid, int a, int b, int c)
{
    unsigned long p1, p2, p3;
    unsigned char vals[8];    // reg 26~33 for PLLA, reg 34~41 for PLLB
    unsigned char pllreg;

    if(a < 15 || a > 90)
        return SI5351_ERR;
    if(b > 1048574)
        return SI5351_ERR;
    if(c > 1048575)
        return SI5351_ERR;

    p1 = 128 * a + 128 * b / c - 512;
    p2 = 128 * b - c * (128 * b / c);
    p3 = c;

    vals[0] = p3 >> 8;
    vals[1] = p3;
    vals[2] = p1 >> 16;
    vals[3] = p1 >> 8;
    vals[4] = p1;
    vals[5] = ((p3 >> 12) & 0xf0) | ((p2 >> 16) & 0x0f);
    vals[6] = p2 >> 8;
    vals[7] = p2;
    switch(pllid) {
        case SI5351_PLLA:
            pllreg = REG_PLLA_PARAMS(0);
            break;
        case SI5351_PLLB:
            pllreg = REG_PLLB_PARAMS(0);
            break;
        default:
            return SI5351_ERR;
    }
    WriteData(pllreg, vals, sizeof(vals));
    return SI5351_OK;
}

int SI5351_SetFreq(int freq)    //int ms_id, int pll_id, int a, int b, int c)
{
    int a, b;
    const int c = 0x80000;
    a = 900000000UL / freq;
    b = ((900000000ULL << 19) / freq) & 0x7ffff;
//    c = 0x80000;

    unsigned long p1, p2, p3;
    unsigned char vals[8];    // reg42~49 for MS0, 50~57 for MS1, ...82~89 for MS5
    unsigned char msreg, msctrl;

    p1 = 128 * a + 128 * b / c - 512;
    p2 = 128 * b - c * (128 * b / c);
    p3 = c;

//    vals[0] = p3 >> 8;
//    vals[1] = p3;
//    vals[2] = p1 >> 16;
    vals[3] = p1 >> 8;
    vals[4] = p1;
    vals[5] = ((p3 >> 12) & 0xf0) | ((p2 >> 16) & 0x0f);
    vals[6] = p2 >> 8;
    vals[7] = p2;

    msreg = REG_MS_PARAMS(SI5351_MS2);
    msctrl = REG_CLK_CTRL(SI5351_MS2);
    WriteData(msreg + 3, vals + 3, sizeof(vals) - 3);
    WriteReg(msctrl, 0x0f);
    return SI5351_OK;
}

int SI5351_SetMSParams(int ms_id, int pll_id, int a, int b, int c)
{
    unsigned long p1, p2, p3;
    unsigned char vals[8];    // reg42~49 for MS0, 50~57 for MS1, ...82~89 for MS5
    unsigned char msreg, msctrl;

    if(a < 6 || a > 900)
        return SI5351_ERR;
    if(b > 1048574)
        return SI5351_ERR;
    if(c > 1048575)
        return SI5351_ERR;

    p1 = 128 * a + 128 * b / c - 512;
    p2 = 128 * b - c * (128 * b / c);
    p3 = c;

    vals[0] = p3 >> 8;
    vals[1] = p3;
    vals[2] = p1 >> 16;
    vals[3] = p1 >> 8;
    vals[4] = p1;
    vals[5] = ((p3 >> 12) & 0xf0) | ((p2 >> 16) & 0x0f);
    vals[6] = p2 >> 8;
    vals[7] = p2;
//    ms_id %= 6;

    if(ms_id >= 0 && ms_id <= 5) {
        msreg = REG_MS_PARAMS(ms_id);
        msctrl = REG_CLK_CTRL(ms_id);
    }
    else
        return SI5351_ERR;
    WriteData(msreg, vals, sizeof(vals));
//    const unsigned char t[] = {[SI5351_PLLA] = 0x0f, [SI5351_PLLB] = 0x2f};
//    pll_id %= 2;
//    /*
    switch(pll_id) {
        case SI5351_PLLA:
            WriteReg(msctrl, 0x0f);
            break;
        case SI5351_PLLB:
            WriteReg(msctrl, 0x2f);
            break;
    }
//     */
//    WriteReg(msctrl, t[pll_id]);
    return SI5351_OK;
}

int SI5351_SetRDiv(int msid, int r)
{
    unsigned char msreg, data;
    if(msid >= 0 && msid <= 5)
        msreg = REG_MS_PARAMS(msid);
    else
        return SI5351_ERR;
    data = ReadReg(msreg + 2);
    data = (data & 0x0f) | (r << 4);
    WriteReg(msreg + 2, data);

    return SI5351_OK;
}

int SI5351_SetOutputState(int clk_id, int state)
{
    unsigned char val = ReadReg(REG_OUTPUT_ENABLE_CTRL);
    if(clk_id >= 0 && clk_id <= 7) {
        if(state)
            val &= ~(1 << clk_id);
        else
            val |= (1 << clk_id);
        WriteReg(REG_OUTPUT_ENABLE_CTRL, val);
    }
    else
        return SI5351_ERR;
    return SI5351_OK;
}
