#include "platform.h"

#define FB_WIDTH 128
#define FB_HEIGHT 32

static struct {
    unsigned char buf[FB_WIDTH * FB_HEIGHT / 8];
    UG_GUI gui;
} g;

static void pset(signed short x, signed short y, unsigned long c)
{
    if(x >= FB_WIDTH || y >= FB_HEIGHT) {
        return;
    }
    x = 127 - x;
    y = 31 - y;
    unsigned char y1 = y >> 3;
    unsigned char yt = y & 0x7;
    if(c)
        g.buf[y1 * FB_WIDTH + x] |= (1 << yt);
    else
        g.buf[y1 * FB_WIDTH + x] &= ~(1 << yt);
}

void setsda(int state)
{
    (state) ? (GPIOB->BSRR = GPIO_Pin_5) : (GPIOB->BRR = GPIO_Pin_5);
}

void setscl(int state)
{
    (state) ? (GPIOB->BSRR = GPIO_Pin_6) : (GPIOB->BRR = GPIO_Pin_6);
//    _delay_us(1);
}

int getsda(void)
{
    return (GPIOB->IDR & GPIO_Pin_5);
}

void DISP_Config(void)
{
    static zi2c_t i2c;
    i2c.setscl_f = setscl;
    i2c.setsda_f = setsda;
    i2c.getsda_f = getsda;
    i2c.addr = 0x0;
    zi2c_init(&i2c);

    GPIOB->BRR = GPIO_Pin_7;
    _delay_ms(50);
    GPIOB->BSRR = GPIO_Pin_7;
    _delay_ms(50);
    SSD1306_Init(&i2c);

    UG_Init(&g.gui, pset, FB_WIDTH, FB_HEIGHT);
    UG_SelectGUI(&g.gui);
    UG_FillScreen(0x00);
    UG_FontSetHSpace(0);
    UG_FontSetVSpace(0);
}

static int calc_str_width(const UG_FONT* font, char* s)
{
    return (font->char_width + g.gui.char_h_space) * strlen(s);
}

void DISP_Update(void)
{
    DataConfig_t* pdc = DC_Get();
    const UG_FONT* font = &FONT_12X16;

    UG_FillScreen(0x00);
    UG_FontSelect(font);
    char buf[32];
    sprintf(buf, "SP %.1f", pdc->status.t_sp);
    UG_PutString(0, 0, buf);
    sprintf(buf, "PV %.1f", pdc->data.t_pv_disp);
    UG_PutString(0, 16, buf);

    font = &FONT_8X12;
    UG_FontSelect(font);
    if(pdc->status.t_step >= 1.0)
        sprintf(buf, "%d", (int)(pdc->status.t_step));
    else
        sprintf(buf, "%.1f", pdc->status.t_step);
    UG_PutString(126 - calc_str_width(font, buf), 0, buf);
//    UG_PutString(110, 0, buf);
    SSD1306_Write(g.buf, 512);
}
