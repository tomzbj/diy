#include "misc.h"
#include "platform.h"

void* __dso_handle = 0;
uint32_t SystemCoreClock = 12000000UL;
const uint8_t AHBPrescTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[] = {0, 0, 0, 0, 1, 2, 3, 4};

extern "C" void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef rcis = {0};
  RCC_OscInitTypeDef rois = {0};
  rois.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  rois.HSEState = RCC_HSE_ON;
  rois.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  rois.PLL.PLLState = RCC_PLL_ON;
  rois.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  rois.PLL.PLLMUL = RCC_PLL_MUL3;    // 16M*3 = 48M
  if(HAL_RCC_OscConfig(&rois) != HAL_OK)
    while(1);
  rcis.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1
    | RCC_CLOCKTYPE_PCLK2);
  rcis.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  rcis.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcis.APB2CLKDivider = RCC_HCLK_DIV2;    // 24MHz — USART1 1.5M → BRR=1 exact
  rcis.APB1CLKDivider = RCC_HCLK_DIV2;    // 24MHz, timers ×2 → 48MHz
  if(HAL_RCC_ClockConfig(&rcis, FLASH_LATENCY_1) != HAL_OK)
    while(1);
}

extern "C" void SystemInit(void)
{
  SCB->VTOR = 0x08002800UL;
  __enable_irq();
  HAL_Init();
  HAL_RCC_DeInit();
  SystemClock_Config();
}
