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
    (state) ? (GPIOA->BSRR = GPIO_Pin_2) : (GPIOA->BRR = GPIO_Pin_2);
//    _delay_us(1);
}

void setscl(int state)
{
    (state) ? (GPIOA->BSRR = GPIO_Pin_3) : (GPIOA->BRR = GPIO_Pin_3);
//    _delay_us(1);
}

int getsda(void)
{
//    _delay_us(1);
    return (GPIOA->IDR & GPIO_Pin_2);
}

void DISP_Config(void)
{
    zi2c_t i2c;
    i2c.setscl_f = setscl;
    i2c.setsda_f = setsda;
    i2c.getsda_f = getsda;
    i2c.addr = 0x3c;
    zi2c_init(&i2c);

    GPIOA->BRR = GPIO_Pin_4;
    _delay_ms(50);
    GPIOA->BSRR = GPIO_Pin_4;
    _delay_ms(50);
    SSD1306_Init(&i2c);

    UG_Init(&g.gui, pset, FB_WIDTH, FB_HEIGHT);
    UG_SelectGUI(&g.gui);
    UG_FillScreen(0x00);
    UG_FontSelect(&FONT_8X12);
    UG_FontSetHSpace(0);
    UG_FontSetVSpace(0);
}

void DISP_Update(void)
{
    char buf[32];
    memset(g.buf, 0, sizeof(g.buf));
    DataConfig_t* pdc = DC_Get();
    if(pdc->data.card_id == -1)
        xsprintf(buf, "NO CARD!");
    else if(pdc->data.card_id == -2 || pdc->data.card_id == -3)
        xsprintf(buf, "Read CARD Error!");
    else
        xsprintf(buf, "CardId %08lX", pdc->data.card_id);
    UG_PutString(0, -1, buf);
    xsprintf(buf, "Buffer %08lX", pdc->data.buffer_id);
    UG_PutString(0, 10, buf);
    unsigned long crc = 0xffffffff;
    crc = crc32(crc, pdc->data.buffer, 1024);
    xsprintf(buf, "CRC    %08lX", ~crc);
    UG_PutString(0, 21, buf);

    SSD1306_Write(g.buf, FB_BUF_SIZE);
}
