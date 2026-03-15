#include "misc.h"
#include <cmath>
#include "platform.h"

static DAC_HandleTypeDef hdac;
static DMA_HandleTypeDef hdma;

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
  extern class ZDDS dds;
  dds.dma_ftf();
}

extern "C" void DMA2_Channel3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma);
}

void DAC_DDS_Stop(void)
{
  HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
}

void DAC_DDS_Start(uint16_t* buf, int count)
{
  test_if(HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t* )buf, count,    //
    DAC_ALIGN_12B_R));
}

void DAC_Config(void)
{
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

  hdac.Instance = DAC;
  DAC_ChannelConfTypeDef dccis;
  test_if(HAL_DAC_Init(&hdac));
//  dccis.DAC_Trigger = DAC_TRIGGER_NONE;
  dccis.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  dccis.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  test_if(HAL_DAC_ConfigChannel(&hdac, &dccis, DAC_CHANNEL_1));

  HAL_NVIC_SetPriority(DMA2_Channel3_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ (DMA2_Channel3_IRQn);

  print_log();
}
