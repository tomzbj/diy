#include "spi3_hal.h"

static SPI_HandleTypeDef hspi3;
static DMA_HandleTypeDef hdma_tx;

void SPI3_Write(const void* msg, int size)
{
  HAL_SPI_Transmit(&hspi3, (uint8_t*)msg, size, 100);
}

void SPI3_Write_DMA(const void* msg, int size)
{
//printf("%p %d\n", msg, size);
  HAL_SPI_Transmit_DMA(&hspi3, (uint8_t*)msg, size);
  while(hspi3.State != HAL_SPI_STATE_READY);
}

extern "C" void DMA2_Channel2_IRQHandler(void)
{
  if(0) {
    static int count = 0;
    ++count %= 100;
    if(!count)
      printf("#\n");
  }
  HAL_DMA_IRQHandler(&hdma_tx);
}

void SPI3_Config(void)
{
  GPIO_InitTypeDef gis;
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_RCC_SPI3_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  hdma_tx.Instance = DMA2_Channel2;
  hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_tx.Init.Mode = DMA_NORMAL;
  hdma_tx.Init.Priority = DMA_PRIORITY_HIGH;
  HAL_DMA_Init(&hdma_tx);
  __HAL_LINKDMA(&hspi3, hdmatx, hdma_tx);

  HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ (DMA2_Channel2_IRQn);

  gis.Pin = GPIO_PIN_3 | GPIO_PIN_5;
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
//  gis.Alternate = GPIO_AF0_SPI3;
  HAL_GPIO_Init(GPIOB, &gis);
  __HAL_AFIO_REMAP_SWJ_NOJTAG();

  hspi3.Instance = SPI3;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi3.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  if(HAL_SPI_Init(&hspi3) != HAL_OK)
    Error_Handler();

  print_log();
}
