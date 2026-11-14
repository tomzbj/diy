#include "adc_hal.h"
#include "misc.h"

enum {
  ADC_SLOT_VOLT_A = 2, ADC_SLOT_CURR_A = 3, ADC_SLOT_VOLT_B = 0, ADC_SLOT_CURR_B = 1,    //
  ADC_SLOT_VREFINT = 4, ADC_SLOT_TEMPSENSOR = 5, ADC_SLOTS = 6
};

//static ADC_HandleTypeDef hadc;
ADC_HandleTypeDef hadc;
static DMA_HandleTypeDef hdma;
static uint32_t adc_buf[240]; /* multiple of ADC_SLOTS */

ADC_RESULT g_result = {0};

ADC_RESULT* ADC_Get(void)
//void ADC_Get(float** curr)
{
//  constexpr float VREF = 1.20f;
  constexpr float VFULL = 3.10f;
  float sum_curr_a = 0, sum_volt_a = 0, sum_curr_b = 0, sum_volt_b = 0, sum_vref = 0;
//  static float prev_curr = 0;
  const int sets = (int)(sizeof(adc_buf) / sizeof(adc_buf[0])) / ADC_SLOTS;

  for(int i = 0; i < sets; i++) {
    sum_curr_a += adc_buf[i * ADC_SLOTS + ADC_SLOT_CURR_A];
    sum_curr_b += adc_buf[i * ADC_SLOTS + ADC_SLOT_CURR_B];
    sum_volt_a += adc_buf[i * ADC_SLOTS + ADC_SLOT_VOLT_A];
    sum_volt_b += adc_buf[i * ADC_SLOTS + ADC_SLOT_VOLT_B];
    sum_vref += adc_buf[i * ADC_SLOTS + ADC_SLOT_VREFINT];
  }
  g_result.curr_a = sum_curr_a / sets * VFULL / 4095.0 * 1000.0;
  g_result.curr_b = sum_curr_b / sets * VFULL / 4095.0 / 34.0 * 201.0 / 330.0 * 1000.0;
  g_result.volt_a = sum_volt_a / sets * VFULL / 4095.0 / 2.2 * 5.5;
  g_result.volt_b = sum_volt_b / sets * VFULL / 4095.0 * 200.0;
  return &g_result;
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
  gis.Pin = GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
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

  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
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

  if(HAL_ADC_DeInit(&hadc) != HAL_OK)
    Error_Handler();
  if(HAL_ADC_Init(&hadc) != HAL_OK)
    Error_Handler();

  ADC_ChannelConfTypeDef accis = {0};
  accis.Rank = ADC_RANK_CHANNEL_NUMBER;

  accis.Channel = ADC_CHANNEL_1;
  HAL_ADC_ConfigChannel(&hadc, &accis);
  accis.Channel = ADC_CHANNEL_3;
  HAL_ADC_ConfigChannel(&hadc, &accis);
  accis.Channel = ADC_CHANNEL_4;
  HAL_ADC_ConfigChannel(&hadc, &accis);
  accis.Channel = ADC_CHANNEL_5;
  HAL_ADC_ConfigChannel(&hadc, &accis);
  accis.Channel = ADC_CHANNEL_VREFINT;
  HAL_ADC_ConfigChannel(&hadc, &accis);
  accis.Channel = ADC_CHANNEL_TEMPSENSOR;
  HAL_ADC_ConfigChannel(&hadc, &accis);

  if(HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)
    Error_Handler();

  if(HAL_ADC_Start_DMA(&hadc, adc_buf, sizeof(adc_buf) / sizeof(adc_buf[0])) != HAL_OK)
    Error_Handler();

  print_log();
}
