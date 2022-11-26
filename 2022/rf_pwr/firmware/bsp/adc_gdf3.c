#include "misc.h"
#include "platform.h"

void ADC_Config(void)
{
    print_log();
    rcu_periph_clock_enable(RCU_ADC);
    rcu_periph_clock_enable(RCU_DMA);
    print_log();

    dma_parameter_struct dis;
    dma_deinit(DMA_CH0);
    dis.periph_addr = (uint32_t)(&ADC_RDATA);
    dis.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dis.memory_addr = (unsigned long)(DC_Get()->data.adc_buf);
    dis.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dis.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dis.memory_width = DMA_MEMORY_WIDTH_16BIT;
    dis.direction = DMA_PERIPHERAL_TO_MEMORY;
    dis.number = DC_ADC_BUF_SIZE * DC_ADC_CHANNEL;
    dis.priority = DMA_PRIORITY_HIGH;
    dma_init(DMA_CH0, &dis);
    dma_circulation_enable(DMA_CH0);
    dma_interrupt_enable(DMA_CH0, DMA_INT_FTF);
    dma_channel_enable(DMA_CH0);
    adc_dma_mode_enable();

    print_log();
    rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);
    print_log();

    adc_special_function_config(ADC_CONTINUOUS_MODE, ENABLE);
    adc_special_function_config(ADC_SCAN_MODE, ENABLE);
//    adc_tempsensor_vrefint_enable();

    print_log();
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
    adc_channel_length_config(ADC_REGULAR_CHANNEL, 3);
    adc_regular_channel_config(0, ADC_CHANNEL_2, ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(1, ADC_CHANNEL_1, ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(2, ADC_CHANNEL_0, ADC_SAMPLETIME_239POINT5);
//    adc_regular_channel_config(1, ADC_CHANNEL_17, ADC_SAMPLETIME_239POINT5);
    //    adc_regular_channel_config(1, ADC_CHANNEL_5, ADC_SAMPLETIME_239POINT5);
    adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);
    adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,
        ADC_EXTTRIG_REGULAR_NONE);
    //    adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_T0_CH0);

    print_log();
    adc_resolution_config(ADC_RESOLUTION_12B);
    adc_special_function_config(ADC_CONTINUOUS_MODE, ENABLE);
    print_log();

    adc_oversample_mode_config(ADC_OVERSAMPLING_ALL_CONVERT, ADC_OVERSAMPLING_SHIFT_8B, ADC_OVERSAMPLING_RATIO_MUL256);
    adc_oversample_mode_enable();

    print_log();
    adc_enable();
    adc_software_trigger_enable(ADC_REGULAR_CHANNEL);

    _delay_ms(10);
    adc_calibration_enable();
    print_log();

    nvic_irq_enable(DMA_Channel0_IRQn, 1, 0);
    print_log();
}
