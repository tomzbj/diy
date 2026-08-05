#include "misc.h"
#include "platform.h"
#include "adc_hal.h"

static ADC_HandleTypeDef hadc;
static DMA_HandleTypeDef hdma;

#define ADC_BUF_SIZE 288
uint16_t adc_buf[ADC_BUF_SIZE];

extern "C" void DMA1_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hadc.DMA_Handle);
}

void ADC_Get(float* volt, float* curr)
{
  // STM32F0 sequencer rank == channel number (forward: CH3 -> CH8 -> CH9)
  // adc_buf: [VREF, V, I, VREF, V, I, ...]
  float sum_v = 0, sum_i = 0, sum_vref = 0;
  for(int i = 0; i < ADC_BUF_SIZE / 3; i++) {
    sum_vref += adc_buf[i * 3];
    sum_v += adc_buf[i * 3 + 1];
    sum_i += adc_buf[i * 3 + 2];
  }
  sum_v = sum_v / sum_vref * 2.5f * (27.0f / 10.0f + 1);
  sum_i = sum_i / sum_vref * 2.5f / 20.0f / 0.33f * 1000.0f;
  if(volt)
    *volt = sum_v;
  if(curr)
    *curr = sum_i;
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
  GPIO_InitTypeDef gis = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_ADC1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  gis.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  gis.Mode = GPIO_MODE_ANALOG;
  gis.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &gis);
  gis.Pin = GPIO_PIN_3;
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
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

  hadc.Instance = ADC1;

  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  // F0: forward scan converts enabled channels in ascending channel-number order
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = ENABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc.Init.SamplingTimeCommon = ADC_SAMPLETIME_239CYCLES_5;
  // PCLK=48MHz: DIV2->24MHz exceeds 14MHz max; DIV4->12MHz is in-spec
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;

  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;

  if(HAL_ADC_Init(&hadc) != HAL_OK)
    Error_Handler();

  // Enable order does not set DMA order; ranks follow channel numbers:
  // CH3(PA3)=VREF, CH8(PB0)=V, CH9(PB1)=I
  ADC_ChannelConfTypeDef accis = {0};
  accis.Rank = ADC_RANK_CHANNEL_NUMBER;
  accis.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  accis.Channel = ADC_CHANNEL_3;
  if(HAL_ADC_ConfigChannel(&hadc, &accis) != HAL_OK)
    Error_Handler();
  accis.Channel = ADC_CHANNEL_8;
  if(HAL_ADC_ConfigChannel(&hadc, &accis) != HAL_OK)
    Error_Handler();
  accis.Channel = ADC_CHANNEL_9;
  if(HAL_ADC_ConfigChannel(&hadc, &accis) != HAL_OK)
    Error_Handler();

  if(HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)
    Error_Handler();
  if(HAL_ADC_Start_DMA(&hadc, (uint32_t*)adc_buf, ADC_BUF_SIZE) != HAL_OK)
    Error_Handler();
  print_log();
}
