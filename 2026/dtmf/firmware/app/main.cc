#include "misc.h"
#include "platform.h"

void* __dso_handle = 0;
uint32_t SystemCoreClock = 16000000UL;
const uint8_t AHBPrescTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[] = {0, 0, 0, 0, 1, 2, 3, 4};

extern "C" void SystemInit(void)
{
//  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); // enable fpu
  __enable_irq();
  HAL_Init();
  RCC_ClkInitTypeDef rcis = {0};
  RCC_OscInitTypeDef rois = {0};
  rois.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  rois.HSEState = RCC_HSE_ON;
  rois.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  rois.PLL.PLLState = RCC_PLL_ON;
  rois.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  rois.PLL.PLLMUL = RCC_PLL_MUL15;    // 144M max for gd32f303, or usart baudrate will be wrong
  if(HAL_RCC_OscConfig(&rois) != HAL_OK)
    while(1);
  rcis.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
    | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  rcis.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  rcis.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcis.APB2CLKDivider = RCC_HCLK_DIV2;
  rcis.APB1CLKDivider = RCC_HCLK_DIV2;
  if(HAL_RCC_ClockConfig(&rcis, FLASH_LATENCY_2) != HAL_OK)
    while(1);
}

int main(void)
{
  void BSP_Init(void);
  BSP_Init();
  while(1) {
//    _delay_us(1);
    zt.poll();
    dem.poll();
  }
}
