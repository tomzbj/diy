#include "misc.h"
#include "platform.h"

#include <cmath>
#include <memory>

static std::unique_ptr<uint16_t[]> g_sinetab, g_recttab;
static int g_tab_size = 240;

static void gen_tab(void)
{
  for(int i = 0; i < g_tab_size; i++) {
    g_sinetab[i] = 2048 + 1500 * std::sin(i * 2 * M_PI / g_tab_size);
    g_recttab[i] = i < g_tab_size / 2 ? 500 : 3500;
  }
}

void DAC_ViewTab(int (*printf_f)(const char* fmt, ...))
{
  printf("addr: %p\n", g_sinetab.get());
  for(int i = 0; i < g_tab_size; i++) {
    printf_f("%5d/%5d", g_sinetab[i], g_recttab[i]);
    if(i % 12 == 11)
      printf_f("\n");
  }
}

void DAC_SetFreq(int freq)
{
  uint32_t sysclk = rcu_clock_freq_get(CK_SYS);
  int lower_limit = sysclk / g_tab_size / 65535 + 1;
  int upper_limit = sysclk / 10 + 1;
  freq = saturate(freq, lower_limit, upper_limit);
  int div = sysclk / g_tab_size / freq - 1;
  timer_autoreload_value_config(TIMER5, div);
}

void DAC_SetTabSize(int tab_size)
{
  g_tab_size = tab_size;
  g_sinetab = std::make_unique<uint16_t[]>(g_tab_size);
  g_recttab = std::make_unique<uint16_t[]>(g_tab_size);
  gen_tab();
  dma_channel_disable(DMA0, DMA_CH5);
  dma_channel_disable(DMA0, DMA_CH6);
  dma_transfer_number_config(DMA0, DMA_CH5, g_tab_size);
  DMA_CHM0ADDR(DMA0, DMA_CH5) = (uint32_t)g_sinetab.get();
  DMA_CHM0ADDR(DMA0, DMA_CH6) = (uint32_t)g_recttab.get();
  dma_channel_enable(DMA0, DMA_CH5);
  dma_channel_enable(DMA0, DMA_CH6);
  dac_dma_enable(DAC0, DAC_OUT0);
  dac_dma_enable(DAC0, DAC_OUT1);
}

void DAC_Config(void)
{
  rcu_periph_clock_enable (RCU_DAC);
  rcu_periph_clock_enable (RCU_DMA0);
  rcu_periph_clock_enable (RCU_GPIOA);

  // PA4&5 as DAC
  auto pins = GPIO_PIN_0 | GPIO_PIN_4 | GPIO_PIN_5;
  gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, pins);

  /*
  dma_single_data_parameter_struct dis;

  dma_deinit(DMA0, DMA_CH5);
  dma_deinit(DMA0, DMA_CH6);

  dma_channel_subperipheral_select(DMA0, DMA_CH5, DMA_SUBPERI7);
  dma_channel_subperipheral_select(DMA0, DMA_CH6, DMA_SUBPERI7);

  dis.periph_addr = (uint32_t)&DAC_OUT0_R12DH(DAC0);
  dis.memory0_addr = (uint32_t)g_sinetab.get();
  dis.direction = DMA_MEMORY_TO_PERIPH;
  dis.number = 240;    // todo!
  dis.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
  dis.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
  dis.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
  dis.circular_mode = DMA_CIRCULAR_MODE_ENABLE;
  dis.priority = DMA_PRIORITY_ULTRA_HIGH;
  dma_single_data_mode_init(DMA0, DMA_CH5, &dis);
  dis.periph_addr = (uint32_t)&DAC_OUT1_R12DH(DAC0);
  dma_single_data_mode_init(DMA0, DMA_CH6, &dis);
  */

  dac_deinit (DAC0);
//  dac_trigger_source_config(DAC0, DAC_OUT0, DAC_TRIGGER_T5_TRGO);
//  dac_trigger_source_config(DAC0, DAC_OUT1, DAC_TRIGGER_T5_TRGO);
//  dac_trigger_enable(DAC0, DAC_OUT0);
//  dac_trigger_enable(DAC0, DAC_OUT1);
  dac_wave_mode_config(DAC0, DAC_OUT0, DAC_WAVE_DISABLE);
//  dac_wave_mode_config(DAC0, DAC_OUT1, DAC_WAVE_DISABLE);
  dac_output_buffer_enable(DAC0, DAC_OUT0);
//  dac_output_buffer_enable(DAC0, DAC_OUT1);
  dac_enable(DAC0, DAC_OUT0);
//  dac_enable(DAC0, DAC_OUT1);
//  dac_dma_enable(DAC0, DAC_OUT0);
//  dac_dma_enable(DAC0, DAC_OUT1);

  print_log();
}
