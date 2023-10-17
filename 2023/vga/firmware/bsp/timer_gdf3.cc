#include "misc.h"
#include "dataconfig.h"

typedef struct {
    int prescaler;
    int h_total;
    int v_total;
    int h_sync;
    int v_sync;
    int h_back_porch;
} vga_timing_t;

//const vga_timing_t timing = {4, 1024, 625, 71, 2, 128};    // CPU=144M, PIX=36M, 800x600x56Hz
//const vga_timing_t timing = {4, 1056, 628, 128, 4, 88};    // CPU=160M, PIX=40M, 800x600x60Hz
const vga_timing_t timing = {8, 800, 525, 96, 2, 28};    // CPU=200M, PIX=25M, 640x480x60Hz
//const vga_timing_t timing= {5, 800, 525, 96, 2, 48};    // CPU=128M, PIX=25.6M, 640x480x60Hz
//const vga_timing_t timing = {4, 840, 500, 64, 3, 120};    // CPU=128M, PIX=32M, 640x480x75Hz
//const vga_timing_t timing = {8, 800, 449, 96, 2, 0};    // CPU=128M, PIX=25.6M, 640x400x70Hz
//const vga_timing_t timing= {2, 1680, 828, 136, 3, 200};    // CPU=168M, PIX=84M, 1280x800x60Hz
//vga_timing_t timing= {2, 1904, 932, 152, 3};    // CPU=216M, PIX=108M, 1440x900x60Hz
//vga_timing_t timing= {1, 2592, 1242, 208, 3};    // CPU=192M, PIX=192M, 1920x1200x60Hz
//vga_timing_t timing= {1, 2576, 1120, 200, 5, 328};    // CPU=176M, PIX=176M, 1920x1080x60Hz not working
//const vga_timing_t timing = {2, 1500, 800, 56, 64};    // CPU=144M, PIX=72M, 1366x768x60Hz

static void TIMER_SYNC_Config(void)
{
    rcu_periph_clock_enable (RCU_GPIOB);
    rcu_periph_clock_enable (RCU_AF);
    rcu_periph_clock_enable (RCU_TIMER2);
    rcu_periph_clock_enable (RCU_TIMER1);

    // PB0 as HSYNC, PB10 as VSYNC
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_0);
    gpio_init(GPIOB, GPIO_MODE_AIN, GPIO_OSPEED_MAX, GPIO_PIN_1);
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_10);
    gpio_pin_remap_config(GPIO_TIMER1_FULL_REMAP, ENABLE);

    nvic_irq_enable(TIMER2_IRQn, 0, 0);
    nvic_irq_enable(SysTick_IRQn, 2, 2);

    timer_oc_parameter_struct tocis;
    timer_parameter_struct tis;

    // PB0/T2_C2 as HSYNC
    timer_deinit (TIMER2);
    tis.prescaler = timing.prescaler - 1;
    tis.alignedmode = TIMER_COUNTER_EDGE;
    tis.counterdirection = TIMER_COUNTER_UP;
    tis.period = timing.h_total - 1;
    tis.clockdivision = TIMER_CKDIV_DIV1;
    tis.repetitioncounter = 0;
    timer_init(TIMER2, &tis);

    tocis.ocpolarity = TIMER_OC_POLARITY_HIGH;
    tocis.outputstate = TIMER_CCX_ENABLE;
    timer_channel_output_config(TIMER2, TIMER_CH_2, &tocis);
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, timing.h_sync);
    timer_channel_output_mode_config(TIMER2, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_2,
        TIMER_OC_SHADOW_DISABLE);

    timer_channel_output_config(TIMER2, TIMER_CH_1, &tocis);
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1,
        timing.h_sync + timing.h_back_porch);
    timer_channel_output_mode_config(TIMER2, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_1,
        TIMER_OC_SHADOW_DISABLE);

    timer_interrupt_enable(TIMER2, TIMER_INT_CH1);
    timer_enable(TIMER2);

    // PB10/T1_C2 as VSYNC

    timer_deinit (TIMER1);
    tis.prescaler = timing.prescaler * timing.h_total - 1;
    tis.alignedmode = TIMER_COUNTER_EDGE;
    tis.counterdirection = TIMER_COUNTER_UP;
    tis.period = timing.v_total - 1;
    tis.clockdivision = TIMER_CKDIV_DIV1;
    tis.repetitioncounter = 0;
    timer_init(TIMER1, &tis);

    tocis.outputstate = TIMER_CCX_ENABLE;
    tocis.ocpolarity = TIMER_OC_POLARITY_HIGH;
    tocis.ocidlestate = TIMER_OC_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER1, TIMER_CH_2, &tocis);

    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_2, timing.v_sync);    //tis.period / 2);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_2,
        TIMER_OC_SHADOW_DISABLE);
    timer_enable(TIMER1);

    print_log();
}

static void TIMER_DAC_Config(void)
{
    rcu_periph_clock_enable (RCU_TIMER5);
    rcu_periph_clock_enable (RCU_DMA1);
    rcu_periph_clock_enable (RCU_DAC);

    nvic_irq_enable(DMA1_Channel2_IRQn, 0, 0);

    dma_parameter_struct dis;
    dma_deinit(DMA1, DMA_CH2);
    dma_struct_para_init(&dis);
    static const unsigned long tmp = 0xff00ff00;

    dis.periph_addr = (unsigned long)&GPIO_OCTL(GPIOA);
    dis.memory_addr = (unsigned long)&tmp;
    dis.direction = DMA_MEMORY_TO_PERIPHERAL;
    dis.number = 2;

    dis.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dis.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dis.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dis.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dis.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA1, DMA_CH2, &dis);

    dma_circulation_disable(DMA1, DMA_CH2);
//    dma_memory_to_memory_disable(DMA_CH2);
    dma_interrupt_enable(DMA1, DMA_CH2, DMA_INT_FTF);
//    dma_interrupt_enable(DMA1, DMA_CH2, DMA_INT_HTF);
    dma_channel_enable(DMA1, DMA_CH2);

    timer_parameter_struct tis;

    // dummy
    dac_deinit();
    dac_trigger_source_config(DAC0, DAC_TRIGGER_T5_TRGO);
    dac_trigger_enable (DAC0);
    dac_enable(DAC0);
    dac_dma_enable(DAC0);

    // T5 as DDS CLK, 10M
    timer_deinit (TIMER5);
    tis.prescaler = 0UL;
    tis.alignedmode = TIMER_COUNTER_EDGE;
    tis.counterdirection = TIMER_COUNTER_UP;
    tis.period = timing.prescaler * 2 - 1;     // pixel clock
    tis.clockdivision = TIMER_CKDIV_DIV1;
    tis.repetitioncounter = 0;
    timer_init(TIMER5, &tis);

    timer_auto_reload_shadow_enable(TIMER5);
    timer_master_output_trigger_source_select(TIMER5, TIMER_TRI_OUT_SRC_UPDATE);
    timer_enable(TIMER5);

    print_log();

}

void TIMER_Config(void)
{
    TIMER_SYNC_Config();
    TIMER_DAC_Config();
}
