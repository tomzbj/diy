#include "misc.h"
#include "platform.h"

const uint8_t AHBPrescTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[] = {0, 0, 0, 0, 1, 2, 3, 4};
uint32_t SystemCoreClock = 64'000'000UL;

void set_clk_to_64m(void);
void set_clk_to_32m(void);

extern "C" void SystemInit(void)
{
  SCB->VTOR = 0x08002800UL;
  HAL_DeInit();
  HAL_Init();
  set_clk_to_32m();
  __enable_irq();
}

void set_clk_to_64m(void)
{
  HAL_RCC_DeInit();
  RCC_ClkInitTypeDef rcis = {0};
  RCC_OscInitTypeDef rois = {0};
  rois.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  rois.HSIState = RCC_HSI_ON;
  rois.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  rois.PLL.PLLState = RCC_PLL_ON;
  rois.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  rois.PLL.PLLMUL = RCC_PLL_MUL16;    // 144M max for gd32f303, or usart baudrate will be wrong
  if(HAL_RCC_OscConfig(&rois) != HAL_OK)
    while(1);

  rcis.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1
    | RCC_CLOCKTYPE_PCLK2);
  rcis.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  rcis.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcis.APB2CLKDivider = RCC_HCLK_DIV1;
  rcis.APB1CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&rcis, FLASH_LATENCY_0) != HAL_OK)
    while(1);
}

void set_clk_to_24m(void)
{
  HAL_RCC_DeInit();
  RCC_ClkInitTypeDef rcis = {0};
  RCC_OscInitTypeDef rois = {0};
  rois.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  rois.HSIState = RCC_HSI_ON;
  rois.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  rois.PLL.PLLState = RCC_PLL_ON;
  rois.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  rois.PLL.PLLMUL = RCC_PLL_MUL6;    // 144M max for gd32f303, or usart baudrate will be wrong
  if(HAL_RCC_OscConfig(&rois) != HAL_OK)
    while(1);

  rcis.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1
    | RCC_CLOCKTYPE_PCLK2);
  rcis.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  rcis.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcis.APB2CLKDivider = RCC_HCLK_DIV1;
  rcis.APB1CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&rcis, FLASH_LATENCY_0) != HAL_OK)
    while(1);
}

void set_clk_to_32m(void)
{
  HAL_RCC_DeInit();
  RCC_ClkInitTypeDef rcis = {0};
  RCC_OscInitTypeDef rois = {0};
  rois.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  rois.HSIState = RCC_HSI_ON;
  rois.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  rois.PLL.PLLState = RCC_PLL_ON;
  rois.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  rois.PLL.PLLMUL = RCC_PLL_MUL8;    // 144M max for gd32f303, or usart baudrate will be wrong
  if(HAL_RCC_OscConfig(&rois) != HAL_OK)
    while(1);

  rcis.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1
    | RCC_CLOCKTYPE_PCLK2);
  rcis.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  rcis.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcis.APB2CLKDivider = RCC_HCLK_DIV1;
  rcis.APB1CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&rcis, FLASH_LATENCY_0) != HAL_OK)
    while(1);
}

