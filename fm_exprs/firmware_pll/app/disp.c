#include "platform.h"

#define FB_WIDTH 96
#define FB_HEIGHT 16
#define FB_BUF_SIZE (FB_WIDTH * FB_HEIGHT / 8)

static struct {
    unsigned char buf[FB_BUF_SIZE];
    UG_GUI gui;
} g;

static void pset(signed short x, signed short y, unsigned long c)
{
    if(x < 0 || y < 0 || x >= FB_WIDTH || y >= FB_HEIGHT)
        return;
    x = 95 - x;
    y = 15 - y;
    unsigned char y1 = y >> 3;
    unsigned char yt = y & 0x7;
    if(c)
        g.buf[y1 * FB_WIDTH + x] |= (1 << yt);
    else
        g.buf[y1 * FB_WIDTH + x] &= ~(1 << yt);
}

static void setsda_1(void)
{
    GPIOA->BSRR = GPIO_Pin_5;
    _delay_us(1);
}
static void setsda_0(void)
{
    GPIOA->BRR = GPIO_Pin_5;
    _delay_us(1);
}
static void setscl_1(void)
{
    GPIOA->BSRR = GPIO_Pin_6;
    _delay_us(1);
}
static void setscl_0(void)
{
    GPIOA->BRR = GPIO_Pin_6;
    _delay_us(1);
}

static int getsda(void)
{
//    _delay_us(1);
    return (GPIOA->IDR & GPIO_Pin_5);
}

void DISP_Config(void)
{
    zi2c_t i2c;
    i2c.setscl_1_f = setscl_1;
    i2c.setscl_0_f = setscl_0;
    i2c.setsda_1_f = setsda_1;
    i2c.setsda_0_f = setsda_0;
    i2c.getsda_f = getsda;
    i2c.addr = 0x3c;
    zi2c_init(&i2c);

    GPIOA->BRR = GPIO_Pin_7;
    _delay_ms_loop(50);
    GPIOA->BSRR = GPIO_Pin_7;
    _delay_ms_loop(50);
    SSD1306_Init(&i2c);

    UG_Init(&g.gui, pset, FB_WIDTH, FB_HEIGHT);
    UG_SelectGUI(&g.gui);
    UG_FillScreen(0x00);
    UG_FontSelect(&FONT_8X16);
    UG_FontSetHSpace(0);
    UG_FontSetVSpace(0);
    DISP_Update();
}

void DISP_Update(void)
{
    char buf[32];
    memset(g.buf, 0, sizeof(g.buf));
    DataConfig_t* pdc = DC_Get();
    int fi = pdc->data.freq / 1000;
    int ff = pdc->data.freq % 1000;
    xsprintf(buf, "%d.%03dMHz", fi, ff);
    UG_PutString(0, 0, buf);
    SSD1306_Write(g.buf, FB_BUF_SIZE);
}
