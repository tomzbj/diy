#include "misc.h"
#include "platform.h"

COMP_HandleTypeDef hcmp;

void CMP_Config(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_COMP1_CLK_ENABLE();

  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_1 | GPIO_PIN_0;
  gis.Mode = GPIO_MODE_INPUT;
  gis.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &gis);

  hcmp.Instance = COMP1;
  hcmp.Init.InputMinus = COMP_INPUT_MINUS_IO3;    // PA0 as CMP1_N
  hcmp.Init.InputPlus = COMP_INPUT_PLUS_IO2;    // PA1 as CMP1_P
  hcmp.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED;
  hcmp.Init.Mode = COMP_POWERMODE_MEDIUMSPEED;
  hcmp.Init.Hysteresis = COMP_HYSTERESIS_DISABLE;
  hcmp.Init.WindowMode = COMP_WINDOWMODE_DISABLE;
  hcmp.Init.TriggerMode = COMP_TRIGGERMODE_NONE;
  if(HAL_COMP_Init(&hcmp) != HAL_OK)
    while(1);
  CMP_Start();
}

void CMP_Start(void)
{
  HAL_COMP_Start(&hcmp);
}

void CMP_Stop(void)
{
  HAL_COMP_Stop(&hcmp);
}

bool CMP_Get(void)
{
  return HAL_COMP_GetOutputLevel(&hcmp);
}
