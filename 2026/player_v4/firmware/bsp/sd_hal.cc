#include "sd_hal.h"
#include "misc.h"

SD_HandleTypeDef hsd;

extern "C" void SDIO_IRQHandler(void)
{
  HAL_SD_IRQHandler(&hsd);
}

extern "C" void DMA2_Stream3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hsd.hdmarx);
}

extern "C" void DMA2_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hsd.hdmatx);
}

uint8_t BSP_SD_Init(void)
{
  BSP_SD_DeInit();
  uint8_t sd_state = MSD_OK;
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
//  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_FALLING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
//  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_ENABLE;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
//  hsd.Init.ClockDiv = SDIO_TRANSFER_CLK_DIV;
  hsd.Init.ClockDiv = 5;
//  hsd.Init.ClockDiv = SDIO_INIT_CLK_DIV;
//  hsd.Init.ClockDiv = 250;
//  BSP_SD_Detect_MspInit(&hsd, NULL);
  if(BSP_SD_IsDetected() != SD_PRESENT) {
    return MSD_ERROR_SD_NOT_PRESENT;
  }
  BSP_SD_MspInit(&hsd, NULL);
  HAL_Delay(100);
  if(HAL_SD_Init(&hsd) != HAL_OK) {
    sd_state = MSD_ERROR;
  }

  HAL_SD_CardInfoTypeDef info = {0};
  BSP_SD_GetCardInfo(&info);
  printf("%8lu %8lu %8lu %8lu %8lu %8lu %8lu %8lu\n", info.CardType, info.CardVersion,
    info.Class, info.RelCardAdd, info.BlockNbr, info.BlockSize, info.LogBlockNbr,
    info.LogBlockSize);

  if(sd_state == MSD_OK) {
    if(HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK) {
      sd_state = MSD_ERROR;
    }
    else {
      sd_state = MSD_OK;
    }
  }
  if(sd_state == MSD_OK)
    printf("SD initialized successfully.\n");
  print_log();
  return sd_state;
}
uint8_t BSP_SD_DeInit(void)
{
  uint8_t sd_state = MSD_OK;
  hsd.Instance = SDIO;
  if(HAL_SD_DeInit(&hsd) != HAL_OK) {
    sd_state = MSD_ERROR;
  }
  hsd.Instance = SDIO;
  BSP_SD_MspDeInit(&hsd, NULL);
  return sd_state;
}

/*uint8_t BSP_SD_ITConfig(void)
 {
 GPIO_InitTypeDef gis;
 gis.Pin = SD_DETECT_PIN;
 gis.Pull = GPIO_PULLUP;
 gis.Speed = GPIO_SPEED_FAST;
 gis.Mode = GPIO_MODE_IT_RISING_FALLING;
 HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &gis);
 HAL_NVIC_SetPriority((IRQn_Type)(SD_DETECT_EXTI_IRQn), 0x0F, 0x00);
 HAL_NVIC_EnableIRQ ((IRQn_Type)(SD_DETECT_EXTI_IRQn));
 return MSD_OK;
 }*/

uint8_t BSP_SD_IsDetected(void)
{
//  __IO uint8_t status = SD_PRESENT;
//  if(HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) == GPIO_PIN_SET) { status = SD_NOT_PRESENT; }
//  return status;
  return SD_PRESENT;
}

uint8_t BSP_SD_ReadBlocks(uint32_t* pData, uint32_t addr, uint32_t nblocks,
  uint32_t Timeout)
{
  if(HAL_SD_ReadBlocks(&hsd, (uint8_t*)pData, addr, nblocks, Timeout) != HAL_OK)
    return MSD_ERROR;
  else
    return MSD_OK;
}

uint8_t BSP_SD_WriteBlocks(uint32_t* pData, uint32_t WriteAddr, uint32_t nblocks,
  uint32_t Timeout)
{
  if(HAL_SD_WriteBlocks(&hsd, (uint8_t*)pData, WriteAddr, nblocks, Timeout) != HAL_OK)
    return MSD_ERROR;
  else
    return MSD_OK;
}
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t* pData, uint32_t addr, uint32_t nblocks)
{
  if(HAL_SD_ReadBlocks_DMA(&hsd, (uint8_t*)pData, addr, nblocks) != HAL_OK)
    return MSD_ERROR;
  else
    return MSD_OK;
}
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t* pData, uint32_t WriteAddr, uint32_t nblocks)
{
  if(HAL_SD_WriteBlocks_DMA(&hsd, (uint8_t*)pData, WriteAddr, nblocks) != HAL_OK)
    return MSD_ERROR;
  else
    return MSD_OK;
}

uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
  if(HAL_SD_Erase(&hsd, StartAddr, EndAddr) != HAL_OK)
    return MSD_ERROR;
  else
    return MSD_OK;
}

void BSP_SD_MspInit(SD_HandleTypeDef* hsd, void* Params)
{
  static DMA_HandleTypeDef hdma_rx;
  static DMA_HandleTypeDef hdma_tx;
  GPIO_InitTypeDef gis = {0};
  __HAL_RCC_SDIO_CLK_ENABLE();
  __DMAx_TxRx_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Pull = GPIO_PULLUP;
//  gis.Speed = GPIO_SPEED_HIGH;
  gis.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gis.Alternate = GPIO_AF12_SDIO;
  gis.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOC, &gis);
  gis.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gis);

  HAL_NVIC_SetPriority(SDIO_IRQn, 14, 0);
  HAL_NVIC_EnableIRQ (SDIO_IRQn);
  hdma_rx.Init.Channel = SD_DMAx_Rx_CHANNEL;
  hdma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_rx.Init.Mode = DMA_PFCTRL;
  hdma_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
  hdma_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma_rx.Init.MemBurst = DMA_MBURST_INC4;
  hdma_rx.Init.PeriphBurst = DMA_PBURST_INC4;
  hdma_rx.Instance = SD_DMAx_Rx_STREAM;
  __HAL_LINKDMA(hsd, hdmarx, hdma_rx);
  HAL_DMA_DeInit(&hdma_rx);
  HAL_DMA_Init(&hdma_rx);
  hdma_tx.Init.Channel = SD_DMAx_Tx_CHANNEL;
  hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_tx.Init.Mode = DMA_PFCTRL;
  hdma_tx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
  hdma_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma_tx.Init.MemBurst = DMA_MBURST_INC4;
  hdma_tx.Init.PeriphBurst = DMA_PBURST_INC4;
  hdma_tx.Instance = SD_DMAx_Tx_STREAM;
  __HAL_LINKDMA(hsd, hdmatx, hdma_tx);
  HAL_DMA_DeInit(&hdma_tx);
  HAL_DMA_Init(&hdma_tx);
  HAL_NVIC_SetPriority(SD_DMAx_Rx_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ (SD_DMAx_Rx_IRQn);
  HAL_NVIC_SetPriority(SD_DMAx_Tx_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ (SD_DMAx_Tx_IRQn);
}

/*void BSP_SD_Detect_MspInit(SD_HandleTypeDef* hsd, void* Params)
 {
 GPIO_InitTypeDef gis;
 SD_DETECT_GPIO_CLK_ENABLE();
 gis.Pin = SD_DETECT_PIN;
 gis.Mode = GPIO_MODE_INPUT;
 gis.Pull = GPIO_PULLUP;
 gis.Speed = GPIO_SPEED_HIGH;
 HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &gis);
 }*/

void BSP_SD_MspDeInit(SD_HandleTypeDef* hsd, void* Params)
{
  static DMA_HandleTypeDef dma_rx_handle;
  static DMA_HandleTypeDef dma_tx_handle;
  HAL_NVIC_DisableIRQ (SD_DMAx_Rx_IRQn);
  HAL_NVIC_DisableIRQ (SD_DMAx_Tx_IRQn);
  dma_rx_handle.Instance = SD_DMAx_Rx_STREAM;
  HAL_DMA_DeInit(&dma_rx_handle);
  dma_tx_handle.Instance = SD_DMAx_Tx_STREAM;
  HAL_DMA_DeInit(&dma_tx_handle);
  HAL_NVIC_DisableIRQ (SDIO_IRQn);
  __HAL_RCC_SDIO_CLK_DISABLE();
}

uint8_t BSP_SD_GetCardState(void)
{
  return ((HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER) ?    //
    SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}
void BSP_SD_GetCardInfo(HAL_SD_CardInfoTypeDef* CardInfo)
{
  HAL_SD_GetCardInfo(&hsd, CardInfo);
}

volatile int writestatus = 0, readstatus = 0;

void BSP_SD_AbortCallback(void)
{
}
void BSP_SD_WriteCpltCallback(void)
{
  _delay_us(1);
  writestatus = 1;
}

void BSP_SD_ReadCpltCallback(void)
{
  _delay_us(1);
  readstatus = 1;
}

extern "C" void HAL_SD_AbortCallback(SD_HandleTypeDef* hsd)
{
  BSP_SD_AbortCallback();
}
extern "C" void HAL_SD_TxCpltCallback(SD_HandleTypeDef* hsd)
{
  BSP_SD_WriteCpltCallback();
}
extern "C" void HAL_SD_RxCpltCallback(SD_HandleTypeDef* hsd)
{
  BSP_SD_ReadCpltCallback();
}

//__weak void BSP_SD_AbortCallback(void) { }
//__weak void BSP_SD_WriteCpltCallback(void) { }
//__weak void BSP_SD_ReadCpltCallback(void) { }

