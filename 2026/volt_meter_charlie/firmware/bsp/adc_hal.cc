#include "adc_hal.h"
#include <algorithm>

static ADC_HandleTypeDef hadc;

extern "C" void ADC_COMP_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&hadc);
}

uint16_t ADC_Get(void)
{
  if(1) {
    constexpr int N_SAMPLES = 16;
    uint16_t tmp[N_SAMPLES];
    for(int i = 0; i < N_SAMPLES; i++) {
      HAL_ADC_Start(&hadc);
      HAL_ADC_PollForConversion(&hadc, 1000);
      tmp[i] = HAL_ADC_GetValue(&hadc);
    }
    std::sort(tmp, tmp + N_SAMPLES);
    uint32_t sum = 0;
    for(int i = N_SAMPLES / 4; i < N_SAMPLES * 3 / 4; i++) {
      sum += tmp[i];
    }
    sum /= (N_SAMPLES / 2);
    return sum;
  }
}

void ADC_Config(void)
{
  __HAL_RCC_ADC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_6;
  gis.Mode = GPIO_MODE_ANALOG;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gis);

  ADC_ChannelConfTypeDef cfg;
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = ENABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc.Init.SamplingTimeCommon = ADC_SAMPLETIME_239CYCLES_5;

  HAL_ADC_DeInit(&hadc);
  if(HAL_ADC_Init(&hadc) != HAL_OK)
    Error_Handler();
  cfg.Rank = ADC_RANK_CHANNEL_NUMBER;    //ADC_RANK_NONE;
  cfg.Channel = ADC_CHANNEL_6;
//  cfg.Channel = ADC_CHANNEL_VREFINT;
  if(HAL_ADC_ConfigChannel(&hadc, &cfg) != HAL_OK)
    Error_Handler();
  HAL_ADC_Start(&hadc);
  print_log();
}
