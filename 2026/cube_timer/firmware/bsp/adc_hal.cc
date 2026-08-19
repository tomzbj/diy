#include "misc.h"
#include "platform.h"
#include "adc_hal.h"

static ADC_HandleTypeDef hadc;

void ADC_DividerEnable(int enable)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  // OD: write 0 → pull enable node low (divider on); write 1 → Hi-Z (off)
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void ADC_PrepareStandby(void)
{
  ADC_DividerEnable(0);
}

float ADC_ReadVbat(void)
{
  constexpr int N = 8;
  uint32_t sum = 0;
  for(int i = 0; i < N; i++) {
    if(HAL_ADC_Start(&hadc) != HAL_OK)
      return 0.0f;
    if(HAL_ADC_PollForConversion(&hadc, 20) != HAL_OK)
      return 0.0f;
    sum += HAL_ADC_GetValue(&hadc);
  }
  float vadc = (float)sum / (float)N / 4095.0f * 3.3f;
  return vadc * 2.0f;    // 100k / 100k
}

void ADC_Config(void)
{
  GPIO_InitTypeDef gis = {0};
  RCC_PeriphCLKInitTypeDef rpcis = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_ADC1_CLK_ENABLE();

  rpcis.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  rpcis.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  HAL_RCCEx_PeriphCLKConfig(&rpcis);

  // PA2 = VBAT/2
  gis.Pin = GPIO_PIN_2;
  gis.Mode = GPIO_MODE_ANALOG;
  gis.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &gis);

  // PA11 = divider enable (OD). High = off (no bleed in STANDBY).
  gis.Pin = GPIO_PIN_11;
  gis.Mode = GPIO_MODE_OUTPUT_OD;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &gis);
  ADC_DividerEnable(0);

  hadc.Instance = ADC1;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.NbrOfConversion = 1;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  if(HAL_ADC_Init(&hadc) != HAL_OK)
    Error_Handler();

  ADC_ChannelConfTypeDef accis = {0};
  accis.Channel = ADC_CHANNEL_2;
  accis.Rank = ADC_REGULAR_RANK_1;
  accis.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if(HAL_ADC_ConfigChannel(&hadc, &accis) != HAL_OK)
    Error_Handler();
  if(HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)
    Error_Handler();

  ADC_DividerEnable(1);
  print_log();
}
