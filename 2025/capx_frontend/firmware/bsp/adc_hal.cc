#include "adc_hal.h"

static ADC_HandleTypeDef hadc;

extern "C" void ADC_COMP_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&hadc);
}

uint16_t ADC_Get(void)
{
//  HAL_ADC_Start(&hadc);
  HAL_ADC_PollForConversion(&hadc, 100);
//  HAL_ADC_Stop(&hadc);
//  return ADC1->DR;
  return HAL_ADC_GetValue(&hadc);
//  if(ADC1->DR > 0) return 1200 * 4095 / ADC1->DR; else return 0;
}

void ADC_Config(void)
{
  __HAL_RCC_ADC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_4;
  gis.Mode = GPIO_MODE_ANALOG;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gis);

  ADC_ChannelConfTypeDef cfg;
//    _hadc.Instance = ADC1;
//    if(HAL_ADCEx_Calibration_Start(&_hadc) != HAL_OK) Error_Handler();
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = ENABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc.Init.SamplingTimeCommon = ADC_SAMPLETIME_239CYCLES_5;

  HAL_ADC_DeInit(&hadc);
  if(HAL_ADC_Init(&hadc) != HAL_OK)
    Error_Handler();
  cfg.Rank = ADC_RANK_CHANNEL_NUMBER;    //ADC_RANK_NONE;
  cfg.Channel = ADC_CHANNEL_4;
  if(HAL_ADC_ConfigChannel(&hadc, &cfg) != HAL_OK)
    Error_Handler();
  HAL_ADC_Start(&hadc);
  print_log();
}
