#include "misc.h"
#include "platform.h"

// even: 0123 -> HEFA   odd: 0123 -> DCGB       // BGCDAFEH
const char mask[] = {0xbe, 0xa0, 0xda, 0xf8, 0xe4, 0x7c, 0x76, 0xa8, 0xfe, 0xec};

static struct {
    unsigned char lcd_data[15];
} g;

static void cs_f(int state)
{
    (state) ? (GPIOC->BSRR = GPIO_Pin_15) : (GPIOC->BRR = GPIO_Pin_15);
    _delay_us(100);
}
static void wr_f(int state)
{
    (state) ? (GPIOC->BSRR = GPIO_Pin_14) : (GPIOC->BRR = GPIO_Pin_14);
    _delay_us(100);
}
static void data_f(int state)
{
    (state) ? (GPIOC->BSRR = GPIO_Pin_13) : (GPIOC->BRR = GPIO_Pin_13);
    _delay_us(100);
}

void LCD_WriteRaw(unsigned char buf[])
{
    for(int i = 0; i < 4; i++) {
        g.lcd_data[i * 4] = buf[i] & 0xf;
        g.lcd_data[i * 4 + 2] = buf[i] >> 4;
    }
}

void LCD_Update(void)
{
    FM1906_WriteData(12, g.lcd_data, sizeof(g.lcd_data));    // start addr = 12
}

void LCD_WriteDots(unsigned char dots)
{
    for(int i = 0; i < 4; i++)
        if(dots & (1 << i))
            g.lcd_data[i * 4] |= 0x1;
        else
            g.lcd_data[i * 4] &= 0xfe;
}

void LCD_WriteInt(int n)
{
    unsigned char buf[4];
    memset(buf, 0, 4);
    buf[3] = mask[n % 10];
    n /= 10;
    buf[2] = mask[n % 10];
    n /= 10;
    buf[1] = mask[n % 10];
    n /= 10;
    buf[0] = mask[n % 10];
    LCD_WriteRaw(buf);
}

void LCD_Config(void)
{
    static fm1906_cfg_t cfg;
    cfg.cs_f = cs_f;
    cfg.wr_f = wr_f;
    cfg.data_f = data_f;
    FM1906_Init(&cfg);
    LCD_WriteInt(0);
    LCD_Update();
    printf("[%s: %d] LCD Initialized.\n", __FILE__, __LINE__);
}
