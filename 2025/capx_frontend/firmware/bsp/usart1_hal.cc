#include "misc.h"
#include "platform.h"
#include "usart1_hal.h"

static UART_HandleTypeDef huart1;

extern "C" void USART1_IRQHandler(void)
{
  if(__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_RXNE)) {
    uint8_t c = USART1_ReadByte();
    u1.push(&c, 1);
  }
  HAL_UART_IRQHandler(&huart1);
}

void USART1_Write(uint8_t* data, int count)
{
  for(int i = 0; i < count; i++) {
    USART1_WriteByte(data[i]);
  }
}

void USART1_WriteByte(uint8_t c)
{
  while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) == RESET);
  USART1->DR = c;
}

uint8_t USART1_ReadByte(void)
{
  return USART1->DR;
}

//void HAL_UARTEx_WakeupCallback(UART_HandleTypeDef* huart) { }

void USART1_Config(void)
{
  GPIO_InitTypeDef gis;
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_USART1_CLK_ENABLE();
  gis.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  gis.Alternate = GPIO_AF0_USART1;
  HAL_GPIO_Init(GPIOB, &gis);
//  gis.Mode = GPIO_MODE_INPUT;
//  gis.Pin = GPIO_PIN_9;
//  HAL_GPIO_Init(GPIOA, &gis);

  HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ (USART1_IRQn);

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 500000UL;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  if(HAL_UART_DeInit(&huart1) != HAL_OK)
    Error_Handler();
  if(HAL_UART_Init(&huart1) != HAL_OK)
    Error_Handler();
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);

  printf("\n\n");
//  _dbg();
//  while(1) { USART1_WriteByte('K'); HAL_Delay(10); }

  print_log();
}
