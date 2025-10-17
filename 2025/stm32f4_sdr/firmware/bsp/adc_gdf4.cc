#include "misc.h"
#include "adc_gdf4.h"
#include "platform.h"
#include "rf.h"

static uint32_t adc_value[16384];

extern "C" void DMA1_Channel0_IRQHandler(void)
{
  extern class RF rf;
  if(dma_interrupt_flag_get(DMA1, DMA_CH0, DMA_INT_FLAG_FTF)) {
    dma_interrupt_flag_clear(DMA1, DMA_CH0, DMA_INT_FLAG_FTF);
    rf.feed(&adc_value[8192]);
//    rf.poll();
//    if(0) { static int count = 0; ++count %= 10000; if(!count) { putchar('#'); fflush(stdout); } }
  }
  else if(dma_interrupt_flag_get(DMA1, DMA_CH0, DMA_INT_FLAG_HTF)) {
    dma_interrupt_flag_clear(DMA1, DMA_CH0, DMA_INT_FLAG_HTF);
    rf.feed(adc_value);
//   _dbg();
//   test1();
  }
//  rf.poll();
}

void dma_config(void);
void ADC_Start(void* buf, int count)
{
  dma_channel_disable(DMA1, DMA_CH0);
  DMA_CHCNT(DMA1, DMA_CH0) = count;
  DMA_CHM0ADDR(DMA1, DMA_CH0) = (uint32_t)(buf);
  adc_enable (ADC0);
  adc_enable (ADC1);
  adc_enable (ADC2);
  adc_dma_mode_enable(ADC0);
  adc_dma_mode_enable(ADC1);
  adc_dma_mode_enable(ADC2);
  dma_channel_enable(DMA1, DMA_CH0);
  /*
   while(!dma_flag_get(DMA1, DMA_CH0, DMA_FLAG_FTF));
   dma_flag_clear(DMA1, DMA_CH0, DMA_FLAG_FTF);
   adc_disable(ADC0);
   adc_disable(ADC1);
   adc_disable(ADC2);
   dma_channel_disable(DMA1, DMA_CH0);
   */
}

void ADC_GetDataOnce(void* dst)
{
}

void dma_config(void)
{
  dma_single_data_parameter_struct dis;
  dma_deinit(DMA1, DMA_CH0);
  dis.periph_addr = (uint32_t)(&ADC_SYNCDATA);
  dis.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
  dis.memory0_addr = (uint32_t)(adc_value);
  dis.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
  dis.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
  dis.circular_mode = DMA_CIRCULAR_MODE_ENABLE;
  dis.direction = DMA_PERIPH_TO_MEMORY;
  dis.number = 512;
  dis.priority = DMA_PRIORITY_HIGH;
  dma_single_data_mode_init(DMA1, DMA_CH0, &dis);
  dma_interrupt_enable(DMA1, DMA_CH0, DMA_INT_HTF);
  dma_interrupt_enable(DMA1, DMA_CH0, DMA_INT_FTF);
  dma_channel_enable(DMA1, DMA_CH0);
}

void adc_config(void)
{
  adc_disable (ADC0);
  adc_disable (ADC1);
  adc_disable (ADC2);
  adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 1);
  adc_channel_length_config(ADC1, ADC_ROUTINE_CHANNEL, 1);
  adc_channel_length_config(ADC2, ADC_ROUTINE_CHANNEL, 1);
  adc_routine_channel_config(ADC0, 0, ADC_CHANNEL_0, ADC_SAMPLETIME_3);
  adc_routine_channel_config(ADC1, 0, ADC_CHANNEL_0, ADC_SAMPLETIME_3);
  adc_routine_channel_config(ADC2, 0, ADC_CHANNEL_0, ADC_SAMPLETIME_3);
  adc_external_trigger_config(ADC0, ADC_ROUTINE_CHANNEL, EXTERNAL_TRIGGER_RISING);
  adc_external_trigger_config(ADC1, ADC_ROUTINE_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
  adc_external_trigger_config(ADC2, ADC_ROUTINE_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
//  adc_external_trigger_source_config(ADC0, ADC_ROUTINE_CHANNEL, ADC_EXTTRIG_ROUTINE_T1_CH1);
  adc_external_trigger_source_config(ADC0, ADC_ROUTINE_CHANNEL,
    ADC_EXTTRIG_ROUTINE_T1_TRGO);
//    ADC_EXTTRIG_ROUTINE_T1_CH1);
  adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
  adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);
  adc_data_alignment_config(ADC2, ADC_DATAALIGN_RIGHT);
//  adc_sync_mode_config (ADC_DAUL_ROUTINE_PARALLEL);
  adc_sync_mode_config (ADC_ALL_ROUTINE_FOLLOW_UP);
  adc_sync_delay_config (ADC_SYNC_DELAY_6CYCLE);
  adc_sync_dma_config (ADC_SYNC_DMA_MODE1);
  adc_sync_dma_request_after_last_enable();
  adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
  adc_special_function_config(ADC1, ADC_SCAN_MODE, ENABLE);
  adc_special_function_config(ADC2, ADC_SCAN_MODE, ENABLE);
}

void ADC_Config(void)
{
  rcu_periph_clock_enable (RCU_SYSCFG);
  rcu_periph_clock_enable (RCU_ADC0);
  rcu_periph_clock_enable (RCU_ADC1);
  rcu_periph_clock_enable (RCU_ADC2);
  rcu_periph_clock_enable (RCU_DMA1);
  adc_clock_config (ADC_ADCCK_HCLK_DIV5);

  dma_config();
  adc_config();
  nvic_irq_enable(DMA1_Channel0_IRQn, 0, 0);

  ADC_Start(adc_value, 16384);

  print_log();
}

