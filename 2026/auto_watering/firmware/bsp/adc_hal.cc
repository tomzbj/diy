#include "misc.h"
#include "platform.h"
#include "adc_hal.h"

ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma;

#define ADC_BUF_SIZE 1024
uint16_t adc_buf[ADC_BUF_SIZE];
uint16_t vrefint = 0;

/*extern "C" void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
 {
 _dbg();
 vrefint = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);
 if(HAL_ADCEx_InjectedStart_IT(hadc) != HAL_OK)
 Error_Handler();
 }

 extern "C" void ADC1_IRQHandler(void)
 {
 HAL_ADC_IRQHandler(&hadc);
 }*/

extern "C" void DMA1_Channel1_IRQHandler(void)
{
//  if(0){ static int count = 0; ++count %= 100; if(count == 0) printf("$\n"); }
  HAL_DMA_IRQHandler(hadc.DMA_Handle);
}

void ADC_Get(uint16_t* pdata, int size)
{
  memcpy(pdata, adc_buf, size * 2);
}

uint16_t ADC_GetVbat(void)
{
  HAL_ADCEx_InjectedStart(&hadc);
  if(HAL_ADCEx_InjectedPollForConversion(&hadc, 100) != HAL_OK)
    return 0;
  auto val = HAL_ADCEx_InjectedGetValue(&hadc, ADC_INJECTED_RANK_1);
  return (val != 0) ? 1200UL * 4095 / val : 0;
}

void ADC_Get(void)
{
  float sum_v = 0, sum_i = 0;
  for(int i = 0; i < ADC_BUF_SIZE / 2; i++) {
    sum_v += adc_buf[i * 2];
    sum_i += adc_buf[i * 2 + 1];
  }
  sum_v /= (ADC_BUF_SIZE / 2);
  sum_i /= (ADC_BUF_SIZE / 2);
  sum_v = sum_v / 4095.0 * 3.3 * 10;
  sum_i = sum_i / 4095.0 * 3.3 / .031 / 20.0;
  printf("%8.3f %8.3f\n", sum_v, sum_i);
}

void ADC_View(void)
{
  for(int i = 0; i < ADC_BUF_SIZE; i++) {
    printf("%8d", adc_buf[i]);
    if(i % 8 == 7)
      printf("\n");
  }
}

void ADC_Config(void)
{
  GPIO_InitTypeDef gis;
  RCC_PeriphCLKInitTypeDef rpcis = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_ADC1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  rpcis.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  rpcis.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  HAL_RCCEx_PeriphCLKConfig(&rpcis);

  gis.Pin = GPIO_PIN_6;
  gis.Mode = GPIO_MODE_ANALOG;
  gis.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &gis);
//  gis.Pin = GPIO_PIN_1;
//  HAL_GPIO_Init(GPIOB, &gis);

  hdma.Instance = DMA1_Channel1;
  hdma.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma.Init.MemInc = DMA_MINC_ENABLE;
  hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma.Init.Mode = DMA_CIRCULAR;
  hdma.Init.Priority = DMA_PRIORITY_HIGH;

  HAL_DMA_DeInit(&hdma);
  HAL_DMA_Init(&hdma);
  __HAL_LINKDMA(&hadc, DMA_Handle, hdma);

  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ (DMA1_Channel1_IRQn);

  hadc.Instance = ADC1;

  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc.Init.ContinuousConvMode = ENABLE;    // trigger from timer
//  hadc.Init.ContinuousConvMode = DISABLE;    // trigger from timer
  hadc.Init.NbrOfConversion = 1;
  hadc.Init.NbrOfDiscConversion = 1;
  hadc.Init.DiscontinuousConvMode = DISABLE;
//  hadc.Init.DiscontinuousConvMode = ENABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;    // ADC_EXTERNALTRIGCONV_T3_TRGO;    // trigger from timer
  if(HAL_ADC_Init(&hadc) != HAL_OK)
    Error_Handler();

  ADC_ChannelConfTypeDef accis = {0};
  accis.Channel = ADC_CHANNEL_6;
//  accis.Channel = ADC_CHANNEL_VREFINT;
  accis.Rank = ADC_REGULAR_RANK_1;
  accis.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if(HAL_ADC_ConfigChannel(&hadc, &accis) != HAL_OK)
    Error_Handler();
  {
    ADC_InjectionConfTypeDef ajcis = {0};
    ajcis.InjectedChannel = ADC_CHANNEL_VREFINT;
    ajcis.InjectedRank = ADC_INJECTED_RANK_1;
    ajcis.InjectedSamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    ajcis.InjectedOffset = 0;
    ajcis.InjectedNbrOfConversion = 1;
    ajcis.InjectedDiscontinuousConvMode = DISABLE;
    ajcis.AutoInjectedConv = DISABLE;
    ajcis.ExternalTrigInjecConv = ADC_INJECTED_SOFTWARE_START;
    if(HAL_ADCEx_InjectedConfigChannel(&hadc, &ajcis) != HAL_OK)
      Error_Handler();
    if(HAL_ADCEx_InjectedStart(&hadc) != HAL_OK)
      Error_Handler();
  }
//  HAL_NVIC_SetPriority(ADC1_IRQn, 0, 1);
//  HAL_NVIC_EnableIRQ (ADC1_IRQn);

  if(HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)
    Error_Handler();
  if(HAL_ADC_Start_DMA(&hadc, (uint32_t*)adc_buf, ADC_BUF_SIZE) != HAL_OK)
    Error_Handler();

  print_log();
}
