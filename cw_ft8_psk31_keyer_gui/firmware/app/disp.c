#include "misc.h"
#include "platform.h"

#define LCD_GPIO GPIOB
#define LCD_CS GPIO_Pin_6
#define LCD_RS GPIO_Pin_7
#define LCD_RESET GPIO_Pin_8

static void lcd_setrs(int state)
{
    (state) ? (GPIOB->BSRR = LCD_RS) : (GPIOB->BRR = LCD_RS);
}

static void lcd_reset(int state)
{
    (state) ? (GPIOB->BSRR = LCD_RESET) : (GPIOB->BRR = LCD_RESET);
}

static void lcd_setcs(int state)
{
    (state) ? (GPIOB->BSRR = LCD_CS) : (GPIOB->BRR = LCD_CS);
}

static void fastread(void* buf, int count)
{
}

static void readwrite(unsigned char data)
{
    SPI1_Write_DMA(&data, 1);
}

static void fastwrite2(const void* buf, int count)
{
    const unsigned char* p = buf;
    for(int i = 0; i < count; i++) {
        SPI1_ReadWrite(*p);
        p++;
    }
}

static void xpt2046_setcs(int cs)
{
    (cs) ? (GPIOA->BSRR = GPIO_Pin_11) : (GPIOA->BRR = GPIO_Pin_11);
    _delay_us(3);
}

static int _int_pow2(int n)
{
    int s = 1;
    for(int i = 0; i < n; i++) {
        s *= 2;
    }
    return s;
}

void DISP_Dimming(int dim)
{
    int pwm = _int_pow2(dim / 10);
    if(pwm > 980)
        pwm = 980;
    pwm += 10;
    TIM_SetCompare1(TIM17, pwm);
}

void DISP_Config(void)
{
    if(1) {
        static xpt2046_cfg_t cfg;
        cfg.readwrite_f = SPI_USART1_ReadWrite;
        cfg.cs_f = xpt2046_setcs;
        XPT2046_Init(&cfg);
    }
    {
        static ili9341_cfg_t cfg;
        cfg.readwrite_f = readwrite;
//        cfg.readwrite_f = SPI1_ReadWrite;
        cfg.reset_f = lcd_reset;
        cfg.cs_f = lcd_setcs;
        cfg.rs_f = lcd_setrs;
        cfg.fastread_f = fastread;
//        cfg.fastwrite_f = fastwrite2;
        cfg.fastwrite_f = SPI1_Write_DMA;
        cfg.delay_us_f = _delay_us;
        ILI9341_Init(&cfg);
    }
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

    GUI_Init();

    if(0) {
        GUI_SetBkColor(GUI_BLACK);
        GUI_SetColor(GUI_WHITE);
        GUI_Clear();
        GUI_SetFont(GUI_FONT_32B_1);
        GUI_DrawRect(5, 5, 155, 75);
        GUI_SetColor(GUI_RED);
        GUI_DispStringHCenterAt("HELLO, WORLD.", 160, 120 - 16);
    }
    WM_SetCreateFlags(WM_CF_MEMDEV);
    DataConfig_t* pdc = DC_Get();
    DISP_Dimming(pdc->cfg.dim);
    int CreateWindow(void);
    pdc->status.hw_main = CreateWindow();

    int CreateWindow_Config(void);
    pdc->status.hw_cfg = CreateWindow_Config();
    WM_HideWindow(pdc->status.hw_cfg);
    WM_ShowWindow(pdc->status.hw_main);

    printf("[%s: %d] DISP Initialized.\n", __FILE__, __LINE__);
}
