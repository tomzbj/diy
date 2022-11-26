#include "platform.h"
#include <math.h>

#define FB_WIDTH 128
#define FB_HEIGHT 32

static void setsda(int state)
{
    (state) ? (GPIO_BOP(GPIOA) = GPIO_PIN_8) : (GPIO_BC(GPIOA) = GPIO_PIN_8);
    _delay_us(2);
}

static void setscl(int state)
{
    (state) ? (GPIO_BOP(GPIOA) = GPIO_PIN_9) : (GPIO_BC(GPIOA) = GPIO_PIN_9);
    _delay_us(2);
}

static int getsda(void)
{
    _delay_us(2);
    return (GPIO_ISTAT(GPIOA) & GPIO_PIN_8);
}

static struct {
    unsigned char buf[FB_WIDTH * FB_HEIGHT / 8];
    UG_GUI gui;
} g;

static void pset(signed short x, signed short y, unsigned long c)
{
    if(x >= FB_WIDTH || y >= FB_HEIGHT || x < 0 || y < 0) {
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

void DISP_Config(void)
{
    static zi2c_t i2c;
    i2c.setscl_f = setscl;
    i2c.setsda_f = setsda;
    i2c.getsda_f = getsda;
    i2c.addr = 0x3c;
    zi2c_init(&i2c);

    GPIO_BC (GPIOA) = GPIO_PIN_10;
    _delay_ms(50);
    GPIO_BOP (GPIOA) = GPIO_PIN_10;
    _delay_ms(50);
    SSD1306_Init(&i2c);

    UG_Init(&g.gui, pset, FB_WIDTH, FB_HEIGHT);
    UG_SelectGUI(&g.gui);
    UG_FillScreen(0x00);
    UG_FontSetHSpace(0);
    UG_FontSetVSpace(0);

    DISP_Update();
    print_log();
}

//static int calc_str_width(const UG_FONT* font, char* s) { return (font->char_width + g.gui.char_h_space) * strlen(s); }

void DISP_Log(char* s, int n)
{
    char buf[128];
    sprintf(buf, "%s %d\n", s, n);
    UG_FontSelect(&FONT_8X8);
    UG_ConsolePutString(buf);
    SSD1306_Write(g.buf, 512);
}

void DISP_Update(void)
{
    DataConfig_t* pdc = DC_Get();
    const UG_FONT* font = &FONT_8X16;

    UG_FillScreen(0x00);
    UG_FontSelect(font);
    UG_PutString(115, 0, "V");
    UG_PutString(111, 16, "mA");
    for(int i = 1; i <= 3; i++)
        UG_DrawLine(i * 35, 0, i * 35, 31, 1);
    UG_PutString(18, 0, "5");
    UG_PutString(53, 0, "9");
    UG_PutString(80, 0, "12");

    for(int i = 0; i < 3; i++) {
        char buf[32];
        const int xt[] = {2, 37, 72};
        int x;
        int cur = pdc->data.cur[i];
        if(cur / 10 >= 100) {
            sprintf(buf, "%d", cur / 10);
            x = xt[i] + 4;
        }
        else {
            sprintf(buf, "%d.%d", cur / 10, cur % 10);
            x = xt[i];
        }
        UG_PutString(x, 16, buf);
    }
//    static int count = 0; count++; sprintf(buf, "%d", count); UG_PutString(0, 0, buf);
    /*
     sprintf(buf, "%d %d", pdc->data.cur1, pdc->data.adc_buf[0]); UG_PutString(0, -1, buf);
     sprintf(buf, "%d %d", pdc->data.cur2, pdc->data.adc_buf[1]); UG_PutString(0, 10, buf);
     sprintf(buf, "%d %d", pdc->data.cur3, pdc->data.adc_buf[2]); UG_PutString(0, 21, buf);
     */

    SSD1306_Write(g.buf, 512);
}
