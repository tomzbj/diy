#include "ssd1306_i2c.h"

#define START_X 0      // for 128x32 resolution

enum {
    CMD_DISP_OFF = 0xae, CMD_DISP_ON = 0xaf, CMD_STARTLINE_0 = 0x40,
    CMD_SET_CONTRAST = 0x81, CMD_SEG_REMAP_COL127_SEG0 = 0xa1,
    CMD_COM_SCAN_DIR_REMAPPED = 0xc8, CMD_DISP_NORMAL = 0xa6,
    CMD_DISP_INVERSE = 0xa7, CMD_MULTIPLEX_RATIO = 0xa8,
    CMD_DISP_OFFSET = 0xd3,
    CMD_DISP_CLK_DIVR_FREQ = 0xd5, CMD_PRECHARGE_PERIOD = 0xd9,
    CMD_COM_PINS_HW_CONF = 0xda, CMD_SET_VCOMH_DESELECT_LEVEL = 0xdb,
    CMD_SET_MEM_ADDR_MODE = 0x20, CMD_SET_CHARGE_PUMP_ENABLE_1 = 0x8d,
    CMD_SET_CHARGE_PUMP_ENABLE_2 = 0x14, CMD_SET_ENTIRE_DISP_ON = 0xa4
};

static zi2c_t g;

static void WriteCmd(unsigned char cmd)
{
    g.start_f(&g);
    g.writebyte_f(&g, 0x78);
    g.writebyte_f(&g, 0x00);
    g.writebyte_f(&g, cmd);
    g.stop_f(&g);
}

static void WriteData(unsigned char* data, int size)
{
    g.start_f(&g);
    g.writebyte_f(&g, 0x78);
    g.writebyte_f(&g, 0x40);
    for(int i = 0; i < size; i++)
        g.writebyte_f(&g, *data++);
    g.stop_f(&g);
}

void SSD1306_SetXy(unsigned char x, unsigned char y)
{
    x += START_X;
    x &= 0x7f;
    y &= 0x7;
    WriteCmd(0xb0 | y);
    WriteCmd(0x10 | (x >> 4));
    WriteCmd(x & 0x0f);
}

void SSD1306_Write(unsigned char* buf, int size)
{
    for(int i = 0; i < 4; i++) {
        WriteCmd(0xb0 + i); //����ҳ��ַ��0~7��
        WriteCmd(0x00); //������ʾλ�á��е͵�ַ
        WriteCmd(0x10 | START_X >> 4); //������ʾλ�á��иߵ�ַ
        WriteCmd(START_X & 0x0f);
        WriteData(buf, 128);
        buf += 128;
    }
}

void SSD1306_DisplayOn(void)
{
    WriteCmd(0X8D); //SET DCDC����
    WriteCmd(0X14); //DCDC ON
    WriteCmd(0XAF); //DISPLAY ON
}

void SSD1306_DisplayOff(void)
{
    WriteCmd(0X8D); //SET DCDC����
    WriteCmd(0X10); //DCDC OFF
    WriteCmd(0XAE); //DISPLAY OFF
}

void SSD1306_Clear(unsigned char data)
{
    unsigned char buf[128];

    for(int i = 0; i < sizeof(buf); i++)
        buf[i] = data;
    for(int i = 0; i < 4; i++) {
        WriteCmd(0xb0 + i);
        WriteCmd(0x00);
        WriteCmd(0x10 | START_X >> 4);
        WriteCmd(START_X & 0xf);
        WriteData(buf, sizeof(buf));
    }
}

void SSD1306_SetBrightness(unsigned char brightness)
{
    if(brightness > 0xf)
        brightness = 0xf;
    brightness = (brightness << 4) | brightness;
    WriteCmd(CMD_PRECHARGE_PERIOD);
    WriteCmd(brightness);
}

void SSD1306_SetContrast(unsigned char contrast)
{
    WriteCmd(CMD_SET_CONTRAST);
    WriteCmd(contrast);
}

void SSD1306_Init(zi2c_t* i2c)
{
    g = *i2c;

    const unsigned char cmd_list[] = {
        //
        0xae, 0x00, 0x10, 0x40, 0xb0, 0x81, 0xcf, 0xa1, 0xa6, 0xa8, 0x3f, 0xc8,
        0xa1, 0xd3, 0x00, 0xd5, 0xf0, 0xd9, 0xf1, 0xda, 0x22, 0xdb, 0x49,
        0x8d, 0x14, 0x20, 0x02, 0xaf //
        };
    // a0/a1: hor dir;  c0/c8 & da 22/da02: ver dir

    for(int i = 0; i < sizeof(cmd_list) / sizeof(cmd_list[0]); i++) {
        WriteCmd(cmd_list[i]);
    }
    _delay_ms_loop(100);

    SSD1306_Clear(0x00);
    SSD1306_SetXy(0, 0);
}
