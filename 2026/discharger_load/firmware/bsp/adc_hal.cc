#include "adc_hal.h"
#include "misc.h"

enum {
  ADC_SLOT_I = 2, ADC_SLOT_V = 0, /* ADC_SLOT_TEMP = 2, */ADC_SLOT_VREF = 1,
  ADC_SLOTS = 4
};

static ADC_HandleTypeDef hadc;
static DMA_HandleTypeDef hdma;
static uint32_t adc_buf[64]; /* multiple of ADC_SLOTS */

void ADC_Get(float* pvolt, float* pcurr)
{
  const float vref = 2.50f;
  float sum_v = 0, sum_i = 0, sum_vref = 0;
//  static float prev_curr = 0;
  const int sets = (int)(sizeof(adc_buf) / sizeof(adc_buf[0])) / ADC_SLOTS;

  for(int i = 0; i < sets; i++) {
    sum_i += adc_buf[i * ADC_SLOTS + ADC_SLOT_I];
    sum_v += adc_buf[i * ADC_SLOTS + ADC_SLOT_V];
    sum_vref += adc_buf[i * ADC_SLOTS + ADC_SLOT_VREF];
  }
  if(sum_vref < 1.0f) {
    *pvolt = 0;
    *pcurr = 0;
    return;
  }

  const float vadc_v = sum_v / sum_vref * vref;
  const float vadc_i = sum_i / sum_vref * vref;

  const float r_hi = 100.0e3f;
  const float r_lo = 10.0e3f;
  *pvolt = vadc_v * (r_hi / r_lo + 1.0f);

  const float r_shunt = 0.1f;
  const float i_gain = 11.0f;
  float curr = vadc_i / i_gain / r_shunt * 1000.0f;    // in mA
//  curr = prev_curr * 0.7f + curr * 0.3f;
  *pcurr = curr;
//  prev_curr = curr;

//printf("%8.3f %8.3f %8.3f %8.3f\n", vadc_v, vadc_i, *pvolt, *pcurr);
}

void ADC_Config(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_DMA_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_ADC_FORCE_RESET();
  __HAL_RCC_ADC_RELEASE_RESET();
  __HAL_RCC_ADC_CLK_ENABLE();

  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
  gis.Mode = GPIO_MODE_ANALOG;
  gis.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &gis);

  hdma.Instance = DMA1_Channel2;
  hdma.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma.Init.MemInc = DMA_MINC_ENABLE;
  hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma.Init.Mode = DMA_CIRCULAR;
  hdma.Init.Priority = DMA_PRIORITY_VERY_HIGH;
  HAL_DMA_DeInit(&hdma);
  HAL_DMA_Init(&hdma);
  HAL_DMA_ChannelMap(&hdma, DMA_CHANNEL_MAP_ADC);
  __HAL_LINKDMA(&hadc, DMA_Handle, hdma);

  hadc.Instance = ADC1;
  if(HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)
    Error_Handler();

  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = ENABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc.Init.SamplingTimeCommon = ADC_SAMPLETIME_239CYCLES_5;

  if(HAL_ADC_Init(&hadc) != HAL_OK)
    Error_Handler();

  ADC_ChannelConfTypeDef accis = {0};
  accis.Rank = ADC_RANK_CHANNEL_NUMBER;

  accis.Channel = ADC_CHANNEL_2; /* PA2 ADC_V */
  HAL_ADC_ConfigChannel(&hadc, &accis);
  accis.Channel = ADC_CHANNEL_3; /* PA3 VREF 2.5V */
  HAL_ADC_ConfigChannel(&hadc, &accis);
  /* CH4 + VREFINT pad sequencer to 4; buf len % 4 == 0 avoids DMA phase walk */
  accis.Channel = ADC_CHANNEL_4; /* PA4 ADC_I */
  HAL_ADC_ConfigChannel(&hadc, &accis);
  accis.Channel = ADC_CHANNEL_VREFINT;
  HAL_ADC_ConfigChannel(&hadc, &accis);

  if(HAL_ADC_Start_DMA(&hadc, adc_buf, sizeof(adc_buf) / sizeof(adc_buf[0])) != HAL_OK)
    Error_Handler();

  print_log();
}
