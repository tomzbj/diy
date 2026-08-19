#include "misc.h"
#include "platform.h"

void PWR_ClearFlags(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();
  PWR->CR |= PWR_CR_CWUF;
  PWR->CR |= PWR_CR_CSBF;
}

int PWR_FlagStandby(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();
  return (PWR->CSR & PWR_CSR_SBF) != 0;
}

int PWR_FlagWkup(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();
  return (PWR->CSR & PWR_CSR_WUF) != 0;
}

int PWR_ReadWkupPin(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_0;
  gis.Mode = GPIO_MODE_INPUT;
  gis.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &gis);
  return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET;
}

void PWR_EnterStandby(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();

  PWR_ClearFlags();

  // PA0 rising-edge wake
  PWR->CSR |= PWR_CSR_EWUP;

  // STANDBY (not STOP)
  PWR->CR |= PWR_CR_PDDS;
  PWR->CR |= PWR_CR_CWUF;

  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  __DSB();
  __ISB();
  __disable_irq();
  __WFI();

  // Should not return
  NVIC_SystemReset();
}

void PWR_Config(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();
  PWR_ClearFlags();
  PWR->CSR &= ~PWR_CSR_EWUP;
  print_log();
}

void PWR_EN_LDO(void)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_2;
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &gis);
  ZPin::set (PB2);
}
