#include "platform.h"
#include "misc.h"

#define RXBUF_SIZE 128
static uint8_t rxbuf[RXBUF_SIZE];

static auto USART0_WriteByte = +[](uint8_t c) {usart_data_transmit(USART0, c);    //
  while(RESET == usart_flag_get(USART0, USART_FLAG_TC));};

void USART0_Write(uint8_t* data, int count)
{
  for(int i = 0; i < count; i++)
    USART0_WriteByte(data[i]);
}

void USART0_Write_DMA(uint8_t* data, int count)
{
  dma_channel_disable(DMA1, DMA_CH7);
  DMA_CHCNT(DMA1, DMA_CH7) = count;
  DMA_CHM0ADDR(DMA1, DMA_CH7) = (uint32_t)(data);
  dma_channel_enable(DMA1, DMA_CH7);
  usart_dma_transmit_config(USART0, USART_TRANSMIT_DMA_ENABLE);
//    usart_dma_receive_config(USART0, USART_RECEIVE_DMA_ENABLE);
  while(!dma_flag_get(DMA1, DMA_CH7, DMA_INT_FLAG_FTF));
  dma_flag_clear(DMA1, DMA_CH7, DMA_FLAG_FTF);
}

static void usart_clear_idle(void)
{
  volatile uint32_t tmp = 0;
  tmp = tmp;
  tmp = USART_STAT0(USART0);
  tmp = USART_DATA(USART0);
}

extern "C" void DMA1_Channel5_IRQHandler(void)
{
  if(dma_interrupt_flag_get(DMA1, DMA_CH5, DMA_INT_FLAG_HTF)) {
    dma_interrupt_flag_clear(DMA1, DMA_CH5, DMA_INT_FLAG_HTF);
    u0.push(rxbuf, RXBUF_SIZE / 2);
  }
  if(dma_interrupt_flag_get(DMA1, DMA_CH5, DMA_INT_FLAG_FTF)) {
    dma_interrupt_flag_clear(DMA1, DMA_CH5, DMA_INT_FLAG_FTF);
    u0.push(&rxbuf[RXBUF_SIZE / 2], RXBUF_SIZE / 2);
  }
}

extern "C" void USART0_IRQHandler(void)
{
  if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) != RESET) {
    usart_clear_idle();
    int size = RXBUF_SIZE - dma_transfer_number_get(DMA1, DMA_CH5);
    if(size < RXBUF_SIZE / 2) {
      u0.push(rxbuf, size);
    }
    else {
      u0.push(&rxbuf[RXBUF_SIZE / 2], size - RXBUF_SIZE / 2);
    }
    dma_channel_disable(DMA1, DMA_CH5);
    dma_flag_clear(DMA1, DMA_CH5, DMA_FLAG_HTF);
    dma_flag_clear(DMA1, DMA_CH5, DMA_FLAG_FTF);
    dma_transfer_number_config(DMA1, DMA_CH5, RXBUF_SIZE);
    dma_channel_enable(DMA1, DMA_CH5);
  }
  usart_interrupt_flag_enum errs[] = {USART_INT_FLAG_ERR_NERR, USART_INT_FLAG_ERR_ORERR,
    USART_INT_FLAG_ERR_FERR};
  for(auto& e : errs)
    if(usart_interrupt_flag_get(USART0, e) != RESET)
      usart_clear_idle();
}

void USART_Config(void)
{
  rcu_periph_clock_enable (RCU_GPIOA);
//    PA9& PA10 as USART0
  uint16_t pins = GPIO_PIN_9 | GPIO_PIN_10;
  gpio_af_set(GPIOA, GPIO_AF_7, pins);
  gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, pins);
  gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pins);

  rcu_periph_clock_enable (RCU_USART0); /* enable USART clock */
//    rcu_periph_clock_enable(RCU_AF);
  __disable_irq();
  usart_disable (USART0);
  usart_deinit(USART0); /* USART configure */
  usart_word_length_set(USART0, USART_WL_8BIT);
  usart_stop_bit_set(USART0, USART_STB_1BIT);
  usart_parity_config(USART0, USART_PM_NONE);
  usart_baudrate_set(USART0, 500000UL);
  usart_receive_config(USART0, USART_RECEIVE_ENABLE);
  usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
  usart_enable(USART0);
  usart_interrupt_enable(USART0, USART_INT_ERR);
//    usart_interrupt_enable(USART0, USART_INT_RBNE);
  usart_interrupt_enable(USART0, USART_INT_IDLE);
  nvic_irq_enable(USART0_IRQn, 1, 1);

  rcu_periph_clock_enable (RCU_DMA1);
  dma_single_data_parameter_struct dis;
  dma_deinit(DMA1, DMA_CH5);

  usart_flag_clear(USART0, USART_FLAG_RBNE);
  dis.direction = DMA_PERIPH_TO_MEMORY;
  dis.memory0_addr = (uint32_t)rxbuf;
  dis.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
  dis.periph_addr = (uint32_t)&USART_DATA(USART0);
  dis.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
  dis.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
  dis.priority = DMA_PRIORITY_ULTRA_HIGH;
  dis.number = sizeof(rxbuf);
  dis.circular_mode = DMA_CIRCULAR_MODE_ENABLE;

  dma_single_data_mode_init(DMA1, DMA_CH5, &dis);

  dis.periph_addr = (uint32_t)&USART_DATA(USART0);
  dis.direction = DMA_MEMORY_TO_PERIPH;
  dis.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
  dma_single_data_mode_init(DMA1, DMA_CH7, &dis);

  dma_channel_subperipheral_select(DMA1, DMA_CH5, DMA_SUBPERI4);
  dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI4);

  dma_channel_enable(DMA1, DMA_CH5);
  dma_interrupt_enable(DMA1, DMA_CH5, DMA_INT_FTF);
  dma_interrupt_enable(DMA1, DMA_CH5, DMA_INT_HTF);
  usart_dma_receive_config(USART0, USART_RECEIVE_DMA_ENABLE);
  nvic_irq_enable(DMA1_Channel5_IRQn, 1, 1);
  __enable_irq();

  printf("\n\n");
  print_log();
}
