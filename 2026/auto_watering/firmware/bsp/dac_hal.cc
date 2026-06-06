#include "misc.h"
#include <cmath>

static DAC_HandleTypeDef hdac;
static DMA_HandleTypeDef hdma;

#define DAC_BUF_SIZE 256
static uint16_t dac_buf[DAC_BUF_SIZE] = {0};

extern "C" void DMA2_Channel3_IRQHandler(void)
{
  if(0) {
//    static int count = 0; ++count %= 100; if(!count) printf("#\n");
  }
  HAL_DMA_IRQHandler(&hdma);
}

static void gentab(void)
{
  for(int i = 0; i < DAC_BUF_SIZE; i++) {
    dac_buf[i] = (uint16_t)(sinf(i * 2 * 3.141593 / DAC_BUF_SIZE) * 1500 + 2048);
  }
}

void DAC_Config(void)
{
  gentab();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_DAC_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();
  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_4;
  gis.Mode = GPIO_MODE_ANALOG;
  gis.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &gis);

  hdma.Instance = DMA2_Channel3;
  hdma.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma.Init.MemInc = DMA_MINC_ENABLE;
  hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma.Init.Mode = DMA_CIRCULAR;
  hdma.Init.Priority = DMA_PRIORITY_HIGH;
  HAL_DMA_Init(&hdma);
  __HAL_LINKDMA(&hdac, DMA_Handle1, hdma);

  hdac.Instance = DAC1;
  test_if(HAL_DAC_Init(&hdac));

  DAC_ChannelConfTypeDef dccis;
//  dccis.DAC_Trigger = DAC_TRIGGER_NONE;
  dccis.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  dccis.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  test_if(HAL_DAC_ConfigChannel(&hdac, &dccis, DAC_CHANNEL_1));
  test_if(HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t* )dac_buf, DAC_BUF_SIZE,    //
    DAC_ALIGN_12B_R));

  HAL_NVIC_SetPriority(DMA2_Channel3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ (DMA2_Channel3_IRQn);
  print_log();
}
