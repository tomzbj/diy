#include "misc.h"
#include "platform.h"
#include "adc_hal.h"

ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma;

#define ADC_BUF_SIZE 205
uint16_t adc_buf[ADC_BUF_SIZE];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
//  printf("ADC#\n");
  dem.feed(adc_buf);
}

extern "C" void DMA1_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hadc.DMA_Handle);
}

void ADC_Config(void)
{
  GPIO_InitTypeDef gis;
  RCC_PeriphCLKInitTypeDef rpcis = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_ADC1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  rpcis.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  rpcis.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  HAL_RCCEx_PeriphCLKConfig(&rpcis);

  gis.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_3;
  gis.Mode = GPIO_MODE_ANALOG;
  gis.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &gis);

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
//  hadc.Init.ContinuousConvMode = ENABLE;    // trigger from timer
  hadc.Init.ContinuousConvMode = DISABLE;    // trigger from timer
  hadc.Init.NbrOfConversion = 1;
  hadc.Init.DiscontinuousConvMode = DISABLE;
//  hadc.Init.DiscontinuousConvMode = ENABLE;
  hadc.Init.NbrOfDiscConversion = 1;
//  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;    // ADC_EXTERNALTRIGCONV_T3_TRGO;    // trigger from timer
  hadc.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;    // trigger from timer
  if(HAL_ADC_Init(&hadc) != HAL_OK)
    Error_Handler();

  ADC_ChannelConfTypeDef accis = {0};
  accis.Channel = ADC_CHANNEL_3;
  accis.Rank = ADC_REGULAR_RANK_1;
  accis.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if(HAL_ADC_ConfigChannel(&hadc, &accis) != HAL_OK)
    Error_Handler();
  if(HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)
    Error_Handler();
  if(HAL_ADC_Start_DMA(&hadc, (uint32_t*)adc_buf, ADC_BUF_SIZE) != HAL_OK)
    Error_Handler();
  print_log();
}
