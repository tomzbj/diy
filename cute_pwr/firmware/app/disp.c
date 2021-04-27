#include "misc.h"
#include "platform.h"

#define LCD_GPIO   GPIOC
#define LCD_RS     GPIO_Pin_14
#define LCD_CS     GPIO_Pin_13
#define LCD_RESET  GPIO_Pin_15

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
    (state) ? (LCD_GPIO->BSRR = LCD_CS) : ( LCD_GPIO->BRR = LCD_CS);
}

static void fastread(void* buf, int count)
{
}

void DISP_Config(void)
{
    static st7735_cfg_t cfg;
    cfg.reset_f = LCD_SetRESET;
    cfg.cs_f = LCD_SetCS;
    cfg.rs_f = LCD_SetRS;
    cfg.fastwrite_f = SPI1_Write_DMA;
    cfg.delay_us_f = _delay_us;
    ST7735_Init(&cfg);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    GUI_Init();
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_SetFont(GUI_FONT_13B_1);
    GUI_DispStringHCenterAt("8CH TEMPERATURE MONITOR", 80, 74);
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_DispStringAt("4.096K", 20, 38);

    WM_SetCreateFlags(WM_CF_MEMDEV);
    void CreateWindow(void);
    CreateWindow();
}
