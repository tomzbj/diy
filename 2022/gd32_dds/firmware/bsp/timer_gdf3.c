#include "misc.h"
#include "platform.h"

void TIMER_Config(void)
{
    rcu_periph_clock_enable(RCU_TIMER5);
    rcu_periph_clock_enable(RCU_DMA);
    rcu_periph_clock_enable(RCU_DAC);

    nvic_irq_enable(DMA_Channel1_2_IRQn, 1, 0);

    dma_parameter_struct dis;
    dma_deinit(DMA_CH2);
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
    dma_init(DMA_CH2, &dis);

    dma_circulation_enable(DMA_CH2);
//    dma_memory_to_memory_enable(DMA_CH2);
    dma_interrupt_enable(DMA_CH2, DMA_INT_FTF);
    dma_interrupt_enable(DMA_CH2, DMA_INT_HTF);

//    dma_channel_enable(DMA_CH2);

    timer_parameter_struct tis;

    // dummy
    dac_deinit();
    dac_trigger_enable();
    dac_trigger_source_config(DAC_TRIGGER_T5_TRGO);
    dac_enable();
//    dac_dma_enable();

    // T13 as DDS CLK, 10M
    timer_deinit(TIMER5);
    tis.prescaler = 0UL;
    tis.alignedmode = TIMER_COUNTER_EDGE;
    tis.counterdirection = TIMER_COUNTER_UP;
    tis.period = 9UL;     // 240M / 10 = 24M
    tis.clockdivision = TIMER_CKDIV_DIV1;
    tis.repetitioncounter = 0;
    timer_init(TIMER5, &tis);

    timer_auto_reload_shadow_enable(TIMER5);
    timer_master_output_trigger_source_select(TIMER5,
        TIMER_TRI_OUT_SRC_UPDATE);
//    timer_enable(TIMER5);

    print_log();
}
