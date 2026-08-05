#include "misc.h"
#include "platform.h"

static UART_HandleTypeDef huart1;

extern "C" void USART1_IRQHandler(void)
{
  if(__HAL_UART_GET_IT(&huart1, UART_IT_RXNE)) {
    uint8_t c = USART1->RDR;    //_ReadByte();
    u1.push(&c, 1);
  }
  HAL_UART_IRQHandler(&huart1);
}

void USART1_Write(uint8_t* data, int size)
{
  HAL_UART_Transmit(&huart1, data, size, 100);
}

//void HAL_UARTEx_WakeupCallback(UART_HandleTypeDef* huart) { }
void USART1_Config(void)
{
  __HAL_RCC_USART1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_9 | GPIO_PIN_10;
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  gis.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIOA, &gis);

  HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ (USART1_IRQn);

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 1500000UL;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;    //UART_ADVFEATURE_NO_INIT;
  huart1.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;

  if(HAL_UART_DeInit(&huart1) != HAL_OK)
    Error_Handler();
  if(HAL_UART_Init(&huart1) != HAL_OK)
    Error_Handler();
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
//    __HAL_UART_ENABLE_IT(&huart, UART_IT_IDLE);
  printf("\n\n");
}
