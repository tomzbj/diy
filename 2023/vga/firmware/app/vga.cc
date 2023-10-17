#include "misc.h"
#include "ugui.h"
#include "zhz.h"

#define RES_Y 240
#define RES_X 320
#define RES_X_P2 (RES_X + 2)

static struct {
    unsigned char buf[RES_X_P2 * RES_Y];
    UG_GUI gui;
} g;

static void pset(signed short x, signed short y, unsigned long c)
{
    if(x >= RES_X || y >= RES_Y || x < 0 || y < 0) {
        return;
    }
    g.buf[y * RES_X_P2 + x] = c & 0xff;
}

ZHZ zhz(pset);

void VGA_Handler(void)
{
    if(TIMER_CNT(TIMER1) / 2 < 20)
        return;
    unsigned long addr = (TIMER_CNT(TIMER1) / 2 - 20) * RES_X_P2;
    dma_channel_disable(DMA1, DMA_CH2);
    dma_memory_address_config(DMA1, DMA_CH2, (unsigned long)&g.buf[addr]);
    dma_transfer_number_config(DMA1, DMA_CH2, RES_X);
//    nvic_irq_enable(DMA1_Channel2_IRQn, 0, 0);
//    dma_circulation_enable(DMA1, DMA_CH2);
    dma_channel_enable(DMA1, DMA_CH2);

    if(0) {
        static int count = 0;
        ++count %= 35700;
        if(count == 0)
            printf("$\n");
    }
}

void VGA_Config(void)
{
    rcu_periph_clock_enable (RCU_GPIOA);
    // PA9&10 as USART0
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, 0x00ff);    // PA0~PA7

    UG_Init(&g.gui, pset, RES_X_P2, RES_Y);
    UG_SelectGUI(&g.gui);
    UG_FillScreen(0x00);
    UG_FontSetHSpace(0);
    UG_FontSetVSpace(0);

    const UG_FONT* font = &FONT_8X12;
    UG_FontSelect(font);

    zhz.set_en_char_width(8);
    zhz.setbc(0);
    zhz.setfont(ZHFONT_12X12);

    zhz.set_en_char_width(FONT_8X12.char_width);
    zhz.setfc(0xe0);
    zhz.puts(10, 10, (char*)"早岁那知世事艰");
    zhz.setfc(0x1c);
    zhz.puts(10, 30, (char*)"中原北望气如山");
    zhz.setfc(0x03);
    zhz.puts(10, 50, (char*)"楼船夜雪瓜洲渡");
    zhz.setfc(0xfc);
    zhz.puts(10, 70, (char*)"铁马秋风大散关");
    zhz.setfc(0x1f);
    zhz.puts(10, 90, (char*)"塞上长城空自许");
    zhz.setfc(0xe3);
    zhz.puts(10, 110, (char*)"镜中衰鬓已先斑");
    zhz.setfc(0x49);
    zhz.puts(10, 130, (char*)"出师一表真名世");
    zhz.setfc(0xff);
    zhz.puts(10, 150, (char*)"千载谁堪伯仲间");

    UG_SetForecolor(0xe0);
    /*
     UG_PutString(80, 80, (char*)"HELLO, WORLD.");
     UG_SetForecolor(0x1c);
     UG_PutString(80, 120, (char*)"HELLO, WORLD.");
     UG_SetForecolor(0x03);
     UG_PutString(80, 160, (char*)"HELLO, WORLD.");
     */
    for(int j = 0; j < 16; j++) {
        for(int i = 0; i < 16; i++) {
            UG_PutChar(j * 16 + i, i * 15 + 20, j * 16 + 0, j * 16 + i, 0);
        }
    }

    UG_DrawFrame(0, 0, RES_X - 1, RES_Y - 1, 0xc0);
}

void VGA_Clear(void)
{
//    bzero(g.linebuf, sizeof(g.linebuf));
}
