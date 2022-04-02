#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "math.h"
#include "ugui.h"
#include "misc.h"
#include "disp.h"
#include "lcd12832.h"
#include "strings.h"
#include "current.h"
#include "menu.h"
#include "conf.h"
#include "gpio_f0.h"
#include "spi_f0.h"

#define IsClick(key) ((key & 0x7) == 0x4)
#define IsLongPress(key) ((key & 0x1ffffff) == 0x1fffffe)

static UG_GUI gui;
static struct {
    unsigned char disp_buf[768];
    bool sleep;
    bool flag_update;
    DISP_State_t state;
} g;

DISP_State_t DISP_GetState(void)
{
    return g.state;
}

void DISP_SetState(DISP_State_t state)
{
    g.state = state;
}

void DISP_Brightness(int brightness)
{
    int br[] = {0, 26, 39, 58, 87, 131, 197, 296, 444, 666, 999};
    if(brightness > 10)
        brightness = 10;
    if(brightness < 0)
        brightness = 0;
    TIM_SetCompare4(TIM2, br[brightness]);
}

void DISP_Config(void)
{
    GPIO_SetPB2(1);

    // do not initialize dma now
    lcd12832_config_t lcfg;
    lcfg.cs_f = GPIO_SetPB0;
    lcfg.rs_f = GPIO_SetPB1;
    lcfg.reset_f = GPIO_SetPB2;
    lcfg.fastwrite_f = SPI2_Write_DMA;

    LCD12832_Init(&lcfg);
    LCD12832_Clear(0xaa);

    UG_Init(&gui, (void (*)(UG_S16, UG_S16, UG_COLOR))DISP_DrawPixel, 128, 32);
    UG_SelectGUI(&gui);
    UG_FillScreen(0x00);
    UG_FontSetHSpace(0);
    UG_FontSetVSpace(0);
    UG_FontSelect(&FONT_8X12);
    UG_PutString(0, 0, "Hello, world.");
    MENU_Config();
    DISP_SetUpdate();
    DISP_Update();
}

void DISP_DrawPixel(unsigned short x, unsigned short y, unsigned long c)
{
    if(x >= 128 || y >= 32) {
//        uprintf(USART3, "%d %d,\t", x, y);
        return;
    }
    unsigned char y1 = y >> 3;
    unsigned char yt = y & 0x7;
    if(c)
        g.disp_buf[y1 * 128 + x] |= _BV(yt);
    else
        g.disp_buf[y1 * 128 + x] &= ~_BV(yt);
}

void DISP_SetUpdate(void)
{
    g.flag_update = true;
}

void DISP_Clear(void)
{
    bzero(g.disp_buf, 512);
}

static void DISP_Normal(void)
{
//    float volt = round(ADC_Get() * 10000) / 10000;
    int channel = CURRENT_GetChannel();
    char buf[32];
//    char* fmt[] = {"%s %.4fuA ", "%s %.3fuA ", "%s %.2fuA ", "%s %.4fmA "};
    char* fmt2[] = {"%s %.2fuA ", "%s %.1fuA ", "%s %.0fuA ", "%s %.2fmA "};
    int scale[] = {1, 10, 100, 1};

    DISP_Clear();
    UG_FontSelect(&FONT_8X16);
//    sprintf(buf, fmt[channel], STRING_Get(STR_MEAS), volt * scale[channel]);
//    UG_PutString(0, 0, buf);
    sprintf(buf, fmt2[channel], STRING_Get(STR_PRESET), CURRENT_GetVal() * scale[channel]);
    UG_PutString(0, 16, buf);
}

void DISP_Update(void)
{
    if(g.flag_update) {
        GlobalConfig_t* conf = GlobalConfigRead();
        DISP_Brightness(conf->disp_brightness * 2);
        switch(g.state) {
            case DISP_STATE_NORMAL:
                DISP_Normal();
                break;
            case DISP_STATE_MENU:
                MENU_View();
                break;
        }
        LCD12832_Write(g.disp_buf, 512);
        g.flag_update = false;
    }
}
