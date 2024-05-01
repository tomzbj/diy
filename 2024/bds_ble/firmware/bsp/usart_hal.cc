#include "misc.h"
#include "platform.h"

static auto Error_Handler = +[] {};
static UART_HandleTypeDef huart1, huart2;

extern "C" void USART1_IRQHandler(void)
{
    if(__HAL_UART_GET_IT(&huart1, UART_IT_RXNE)) {
        u1.rxne_irqhandler();
        __HAL_UART_CLEAR_IT(&huart1, UART_IT_RXNE);
    }
    HAL_UART_IRQHandler(&huart1);
}
extern "C" void USART2_IRQHandler(void)
{
    if(__HAL_UART_GET_IT(&huart2, UART_IT_RXNE)) {
        u2.rxne_irqhandler();
        __HAL_UART_CLEAR_IT(&huart2, UART_IT_RXNE);
    }
    HAL_UART_IRQHandler(&huart2);
}

void USART2_WriteByte(uint8_t c)
{
    while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TXE) == RESET);
    USART2->TDR = c;
}

uint8_t USART2_ReadByte(void)
{
    return USART2->RDR;
}

void USART1_WriteByte(uint8_t c)
{
    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) == RESET);
    USART1->TDR = c;
}

uint8_t USART1_ReadByte(void)
{
    return USART1->RDR;
}

void HAL_UARTEx_WakeupCallback(UART_HandleTypeDef* huart)
{
}

static void _usart_config(UART_HandleTypeDef& huart, USART_TypeDef* usartx,
    uint32_t baudrate)
{
    huart.Instance = usartx;
    huart.Init.BaudRate = baudrate;
    huart.Init.WordLength = UART_WORDLENGTH_8B;
    huart.Init.StopBits = UART_STOPBITS_1;
    huart.Init.Parity = UART_PARITY_NONE;
    huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart.Init.Mode = UART_MODE_TX_RX;
    huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if(HAL_UART_DeInit(&huart) != HAL_OK)
        Error_Handler();
    if(HAL_UART_Init(&huart) != HAL_OK)
        Error_Handler();
    __HAL_UART_ENABLE_IT(&huart, UART_IT_RXNE);
//    __HAL_UART_ENABLE_IT(&huart, UART_IT_IDLE);
}

void USART_Config(void)
{
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gis;
    gis.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    gis.Mode = GPIO_MODE_AF_PP;
    gis.Pull = GPIO_PULLUP;
    gis.Speed = GPIO_SPEED_FREQ_HIGH;
    gis.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &gis);
    gis.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    gis.Alternate = GPIO_AF1_USART2;
    HAL_GPIO_Init(GPIOA, &gis);

    _usart_config(huart1, USART1, 500000UL);
    _usart_config(huart2, USART2, 9600UL);

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ (USART1_IRQn);
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ (USART2_IRQn);

    printf("\n\n");
}
