#include "misc.h"
#include "platform.h"

uint32_t SystemCoreClock = 108000000UL;
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};

extern "C" void SystemInit(void)
{
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));    // enable fpu

  __enable_irq();
  HAL_Init();
  RCC_ClkInitTypeDef rcis;
  RCC_OscInitTypeDef rois;
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  rois.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  rois.HSEState = RCC_HSE_ON;
  rois.PLL.PLLState = RCC_PLL_ON;
  rois.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  rois.PLL.PLLM = 16;
  rois.PLL.PLLN = 192;
  rois.PLL.PLLP = RCC_PLLP_DIV2;
  rois.PLL.PLLQ = 4;    // 384M / 8 = 48M for USB
  rois.PLL.PLLR = 2;
  if(HAL_RCC_OscConfig(&rois) != HAL_OK)
    while(1);
  rcis.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1
    | RCC_CLOCKTYPE_PCLK2);
  rcis.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  rcis.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcis.APB1CLKDivider = RCC_HCLK_DIV2;
  rcis.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&rcis, FLASH_LATENCY_4) != HAL_OK)
    while(1);
}
