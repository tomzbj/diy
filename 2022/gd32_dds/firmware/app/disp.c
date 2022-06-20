#include "platform.h"
#include <math.h>

#define FB_WIDTH 128
#define FB_HEIGHT 32

static void setsda(int state)
{
    (state) ? (GPIO_BOP(GPIOB) = GPIO_PIN_5) : (GPIO_BC(GPIOB) = GPIO_PIN_5);
    _delay_us(10);
}

static void setscl(int state)
{
    (state) ? (GPIO_BOP(GPIOB) = GPIO_PIN_6) : (GPIO_BC(GPIOB) = GPIO_PIN_6);
    _delay_us(10);
}

static int getsda(void)
{
    _delay_us(10);
    return (GPIO_ISTAT(GPIOB) & GPIO_PIN_5);
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
//    x = 127 - x;
//    y = 31 - y;
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

    GPIO_BC (GPIOB) = GPIO_PIN_7;
    _delay_ms(50);
    GPIO_BOP (GPIOB) = GPIO_PIN_7;
    _delay_ms(50);
    SSD1306_Init(&i2c);

    UG_Init(&g.gui, pset, FB_WIDTH, FB_HEIGHT);
    UG_SelectGUI(&g.gui);
    UG_FillScreen(0x00);
    UG_FontSetHSpace(0);
    UG_FontSetVSpace(0);
    MENU_Config();

    DC_Get()->state.disp_state = DC_STATE_NORMAL;
    DISP_Update();
    print_log();
}

//static int calc_str_width(const UG_FONT* font, char* s) { return (font->char_width + g.gui.char_h_space) * strlen(s); }

void DISP_Normal(void)
{
//_dbg();
    DataConfig_t* pdc = DC_Get();
    const char* ampl_names[] = {"12dBm", "9dBm", "6dBm", "3dBm", "0dBm"};
    const char* waveform_names[] = {"SINE", "RECT.", "TRI.", "SAW."};

    int curr_preset = pdc->state.curr_preset;
    int freq = pdc->cfg.freq[curr_preset];
    char buf[32];
    int unit;
    if(pdc->cfg.unit == DC_UNIT_AUTO) {
        if(freq > 1000000)
            unit = DC_UNIT_MHZ;
        else if(freq > 1000)
            unit = DC_UNIT_KHZ;
        else
            unit = DC_UNIT_HZ;
    }
    else
        unit = pdc->cfg.unit;
    switch(unit) {
        case DC_UNIT_HZ:
            sprintf(buf, "%d Hz", freq);
            break;
        case DC_UNIT_KHZ:
            sprintf(buf, "%d.%02dkHz", freq / 1000, (freq % 1000) / 10);
            break;
        case DC_UNIT_MHZ:
            sprintf(buf, "%d.%05dMHz", freq / 1000000, (freq % 1000000) / 10);
            break;
    }
    UG_FontSetHSpace(0);
    UG_FontSelect(&FONT_12X16);
    UG_PutString(0, 12, buf);

    UG_FontSelect(&FONT_8X8);
    sprintf(buf, "#%d %s %s", curr_preset + 1, ampl_names[pdc->state.ampl],
        waveform_names[pdc->state.waveform]);
    UG_PutString(0, 0, buf);
}

void DISP_Update(void)
{
    DataConfig_t* pdc = DC_Get();
    UG_FillScreen(0x00);

    switch(pdc->state.disp_state) {
        case DC_STATE_NORMAL:
            DISP_Normal();
            break;
        case DC_STATE_SELECT:
            SELECT_View();
            break;
        case DC_STATE_MENU:
            MENU_View();
            break;
        case DC_STATE_EDIT:
            MENU_View_Edit();
            break;
    }
    SSD1306_Write(g.buf, 512);
}
