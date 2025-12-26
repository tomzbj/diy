#include "misc.h"
#include "platform.h"
#include "usart2_hal.h"

static UART_HandleTypeDef huart2;

extern "C" void USART2_IRQHandler(void)
{
  if(__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_RXNE)) {
    uint8_t c = USART2_ReadByte();
    u2.push(&c, 1);
  }
  HAL_UART_IRQHandler(&huart2);
}

void USART2_Write(uint8_t* data, int count)
{
  for(int i = 0; i < count; i++) {
    USART2_WriteByte(data[i]);
  }
}

void USART2_WriteByte(uint8_t c)
{
  while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TXE) == RESET);
  USART2->DR = c;
}

uint8_t USART2_ReadByte(void)
{
  return USART2->DR;
}

//void HAL_UARTEx_WakeupCallback(UART_HandleTypeDef* huart) { }

void USART2_Config(void)
{
  GPIO_InitTypeDef gis;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART2_CLK_ENABLE();
  gis.Pin = GPIO_PIN_2 | GPIO_PIN_3;
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  gis.Alternate = GPIO_AF4_USART2;
  HAL_GPIO_Init(GPIOA, &gis);
//  gis.Mode = GPIO_MODE_INPUT;
//  gis.Pin = GPIO_PIN_9;
//  HAL_GPIO_Init(GPIOA, &gis);

  HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ (USART2_IRQn);

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600UL;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  if(HAL_UART_DeInit(&huart2) != HAL_OK)
    Error_Handler();
  if(HAL_UART_Init(&huart2) != HAL_OK)
    Error_Handler();
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_ERR);

  printf("\n\n");
//  _dbg();
//  while(1) { USART2_WriteByte('K'); HAL_Delay(10); }

  print_log();
}
