#include <string.h>
#include "platform.h"
#include "math.h"

static struct {
    UG_GUI gui;
} g;

#define LCD_GPIO   GPIOA
#define LCD_RS     GPIO_Pin_15
#define LCD_CS     GPIO_Pin_11
#define LCD_RESET  GPIO_Pin_12

static void LCD_SetRS(int state)
{
    (state) ? (LCD_GPIO->BSRR = LCD_RS) : (LCD_GPIO->BRR = LCD_RS);
}

static void LCD_SetRESET(int state)
{
    (state) ? (LCD_GPIO->BSRR = LCD_RESET) : (LCD_GPIO->BRR = LCD_RESET);
}

static void LCD_SetCS(int state)
{
//    _delay_us(1);
    (state) ? (LCD_GPIO->BSRR = LCD_CS) : ( LCD_GPIO->BRR = LCD_CS);
}

static void LCD_WriteData(void* buf, int nbytes)
{
    unsigned char SPI3_WriteByte(unsigned char data);
    for(int i = 0; i < nbytes; i++) {
        SPI3_WriteByte(*(unsigned char*)buf);
        buf++;
    }
}

static void pset(signed short x, signed short y, UG_COLOR c)
{
    ILI9341_SetWindow(x, y, x, y);
    c = (c >> 8) | (c << 8);
    ILI9341_Write(&c, 1);
}

#define foreach(i, x)    for(int i = 0; i < sizeof(x) / sizeof(x[0]); i++)

#define FILLSIZE 512

static UG_RESULT fillframe(signed short x1, signed short y1, signed short x2,
    signed short y2, unsigned long c)
{
    unsigned short buf[FILLSIZE];
    int n = (x2 - x1 + 1) * (y2 - y1 + 1);
    int size;
    c = (c >> 8) | (c << 8);
    foreach(i, buf)
    {
        buf[i] = c;
    }
    ILI9341_SetWindow(x1, y1, x2, y2);
    size = n > FILLSIZE ? FILLSIZE : n;
    while(n > 0) {
        ILI9341_Write(buf, size);
        n -= size;
        size = n > FILLSIZE ? FILLSIZE : n;
    }
    return UG_RESULT_OK;
}

static void push_pixel(UG_COLOR c)
{
    c = (c >> 8) | (c << 8);
    ILI9341_Write(&c, 1);
}

static void* fillarea(signed short x1, signed short y1, signed short x2,
    signed short y2)
{
    ILI9341_SetWindow(x1, y1, x2, y2);
    return &push_pixel;
}

static UG_RESULT drawline(signed short x1, signed short y1, signed short x2,
    signed short y2, UG_COLOR c)
{
    if(x1 == x2 || y1 == y2) {    // hline or vline
        fillframe(x1, y1, x2, y2, c);
        return UG_RESULT_OK;
    }
    return UG_RESULT_FAIL;
}

void DISP_Config(void)
{
    LCD_SetCS(1);
    static ili9341_cfg_t cfg;
    cfg.width = 320;
    cfg.height = 240;

    cfg.cs_f = LCD_SetCS;
    cfg.rs_f = LCD_SetRS;
    cfg.reset_f = LCD_SetRESET;
    cfg.fastwrite_f = SPI3_Write_DMA;
    cfg.delay_us_f = _delay_us;
    ILI9341_Init(&cfg);
    ILI9341_Clear(0x31e7);

    UG_Init(&g.gui, pset, cfg.width, cfg.height);
    UG_DriverRegister(DRIVER_FILL_FRAME, fillframe);
    UG_DriverRegister(DRIVER_FILL_AREA, fillarea);
    UG_DriverRegister(DRIVER_DRAW_LINE, drawline);
    UG_DriverEnable(DRIVER_FILL_FRAME);
    UG_DriverEnable(DRIVER_FILL_AREA);
    UG_DriverEnable(DRIVER_DRAW_LINE);
    UG_SelectGUI(&g.gui);
    UI_Init();

}
