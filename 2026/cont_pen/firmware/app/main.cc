#include "platform.h"

static void msp_config(void)
{
  GPIO_InitTypeDef gis = {0};

  // led
  __HAL_RCC_GPIOA_CLK_ENABLE();
  gis.Pin = GPIO_PIN_2;
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gis);

  {
    // will run only if the chip is new
    FLASH_OBProgramInitTypeDef obis = {0};
    HAL_FLASH_OBGetConfig(&obis);
    if((obis.USERConfig & OB_USER_NRST_MODE) != OB_RESET_MODE_GPIO) {
      HAL_FLASH_Unlock();
      HAL_FLASH_OB_Unlock();
      obis.OptionType = OPTIONBYTE_USER;
      obis.USERType = OB_USER_NRST_MODE;
      obis.USERConfig = OB_RESET_MODE_GPIO;
      auto st = HAL_FLASH_OBProgram(&obis);
      HAL_FLASH_OB_Lock();
      HAL_FLASH_Lock();
      if(st == HAL_OK)
        HAL_FLASH_OB_Launch();    // will reboot
    }
  }
  // pin 5 is shared with PF2/NRST & PB0. To use PB0, PF2 should be configured as analog.
  __HAL_RCC_GPIOF_CLK_ENABLE();
  gis.Pin = GPIO_PIN_2;
  gis.Mode = GPIO_MODE_ANALOG;
  gis.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &gis);

  TIMER3_Config();
  CMP_Config();
}

static int low_pwr_cnt;

static void enter_stop(void)
{
  CMP_Stop();
  {
    GPIO_InitTypeDef gis = {0};
    gis.Pin = GPIO_PIN_1;
    gis.Mode = GPIO_MODE_INPUT;
    gis.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gis);

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    gis.Pin = GPIO_PIN_0;
    gis.Mode = GPIO_MODE_IT_FALLING;
    gis.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &gis);
    EXTI->PR = EXTI_PR_PR0;
    HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ (EXTI0_1_IRQn);
  }
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_SuspendTick();
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
  NVIC_SystemReset();
}

void det(void)
{
  if(CMP_Get()) {
    TIMER3_BUZZ_Start();
    low_pwr_cnt = 0;
  }
  else {
    TIMER3_BUZZ_Stop();
    low_pwr_cnt++;
  }
  if(low_pwr_cnt >= 200) /* 50ms * 200 = 10s */
    enter_stop();
}

int main(void)
{
  SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);
  msp_config();
  while(1) {
    det();
    HAL_Delay(50);
  }
  return 0;
}
