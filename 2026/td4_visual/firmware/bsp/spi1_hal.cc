#include "spi1_hal.h"

static SPI_HandleTypeDef hspi1;
static DMA_HandleTypeDef hdma_tx;

/* Polling TX: avoid F0 HAL 8-bit packing (broken on GD32E230). */
void SPI1_Write(const void* msg, int size)
{
  const uint8_t* p = (const uint8_t*)msg;

  if((hspi1.Instance->CR1 & SPI_CR1_SPE) == 0)
    __HAL_SPI_ENABLE(&hspi1);

  while(size > 0) {
    if(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE)) {
      *(__IO uint8_t*)&hspi1.Instance->DR = *p++;
      size--;
    }
  }
  while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_BSY));
  __HAL_SPI_CLEAR_OVRFLAG(&hspi1);
}

void SPI1_Write_DMA(const void* msg, int size)
{
  /* ST7789 init/cmds are many 1-byte transfers; DMA+IRQ overhead is useless
   * and easy to fault if the IRQ handler is missing. */
  if(size <= 4) {
    SPI1_Write(msg, size);
    return;
  }

  while(hspi1.State != HAL_SPI_STATE_READY);
  if(HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)msg, (uint16_t)size) != HAL_OK)
    Error_Handler();
  while(hspi1.State != HAL_SPI_STATE_READY);
}

extern "C" void DMA1_Channel2_3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_tx);
}

void SPI1_Config(void)
{
  GPIO_InitTypeDef gis;
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_SPI1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  gis.Pin = GPIO_PIN_3 | GPIO_PIN_5;
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  gis.Alternate = GPIO_AF0_SPI1;
  HAL_GPIO_Init(GPIOB, &gis);

  hspi1.Instance = SPI1;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  if(HAL_SPI_Init(&hspi1) != HAL_OK)
    Error_Handler();

  /* Link DMA after SPI init (DeInit would not clear hdmatx, but keep order clear). */
  hdma_tx.Instance = DMA1_Channel3; /* F0/GD32E230: SPI1_TX */
  hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_tx.Init.Mode = DMA_NORMAL;
  hdma_tx.Init.Priority = DMA_PRIORITY_HIGH;
  if(HAL_DMA_Init(&hdma_tx) != HAL_OK)
    Error_Handler();
  __HAL_LINKDMA(&hspi1, hdmatx, hdma_tx);

  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ (DMA1_Channel2_3_IRQn);

  __HAL_SPI_ENABLE(&hspi1);
  print_log();
}
