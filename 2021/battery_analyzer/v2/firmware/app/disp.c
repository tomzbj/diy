#include "platform.h"

#define FB_WIDTH 128
#define FB_HEIGHT 32
#define FB_BUF_SIZE (FB_WIDTH * FB_HEIGHT / 8)

static struct {
    unsigned char buf[FB_BUF_SIZE];
    UG_GUI gui;
} g;

static void pset(signed short x, signed short y, unsigned long c)
{
    if(x < 0 || y < 0 || x >= FB_WIDTH || y >= FB_HEIGHT)
        return;
//    x = 127 - x; y = 31 - y;
    unsigned char y1 = y >> 3;
    unsigned char yt = y & 0x7;
    if(c)
        g.buf[y1 * FB_WIDTH + x] |= (1 << yt);
    else
        g.buf[y1 * FB_WIDTH + x] &= ~(1 << yt);
}

void setsda(int state)
{
    (state) ? (GPIOB->BSRR = GPIO_Pin_0) : (GPIOB->BRR = GPIO_Pin_0);
}

void setscl(int state)
{
    (state) ? (GPIOB->BSRR = GPIO_Pin_1) : (GPIOB->BRR = GPIO_Pin_1);
}

int getsda(void)
{
    return (GPIOB->IDR & GPIO_Pin_0);
}

void DISP_Config(void)
{
    static zi2c_t i2c;
    i2c.setscl_f = setscl;
    i2c.setsda_f = setsda;
    i2c.getsda_f = getsda;
    i2c.addr = 0x0;
    zi2c_init(&i2c);

    GPIOB->BRR = GPIO_Pin_2;
    _delay_ms(50);
    GPIOB->BSRR = GPIO_Pin_2;
    _delay_ms(50);
    SSD1306_Init(&i2c);

    UG_Init(&g.gui, pset, FB_WIDTH, FB_HEIGHT);
    UG_SelectGUI(&g.gui);
    UG_FillScreen(0x00);
    UG_FontSetHSpace(0);
    UG_FontSetVSpace(0);
}

//static int calc_str_width(const UG_FONT* font, char* s) { return (font->char_width + g.gui.char_h_space) * strlen(s); }

void DISP_Update(void)
{
    DataConfig_t* pdc = DC_Get();
    UG_FillScreen(g.gui.back_color);
    switch(pdc->status.disp_state) {
        case DC_DISP_STATE_NORMAL:
            UG_FontSelect(&FONT_8X16);
            UG_FontSetHSpace(0);
            char buf[48];
            static int count;
            count++;
            const char* arrow[] = {">--", "->-", "-->"};
            const char* pstate;
            if(pdc->status.discharge_state == DC_DISCHARGE_STATE_ON)
                pstate = arrow[count % 3];
            else
                pstate = "---";
            sprintf(buf, "%.2fV %s %.2fV", pdc->data.vbat1, pstate,
                pdc->cfg.end_volt / 1000.0);
            UG_PutString(0, 0, buf);
            sprintf(buf, "%.0fmA %.0fmAh", pdc->data.curr * 1000.0,
                pdc->data.total_capacity * 1000.0);
            UG_PutString(0, 16, buf);
            break;
        case DC_DISP_STATE_MENU:
            MENU_View();
            break;
        case DC_DISP_STATE_SELECT:
            SELECT_View();
            break;
    }
    SSD1306_Write(g.buf, FB_BUF_SIZE);
}
