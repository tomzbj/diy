#include "misc.h"
#include "adc_hal.h"

static auto Error_Handler = +[] {};

void CADC::init(void)
{
    RCC_OscInitTypeDef rois;
    __HAL_RCC_ADC1_CLK_ENABLE();
    HAL_RCC_GetOscConfig(&rois);
    rois.OscillatorType = RCC_OSCILLATORTYPE_HSI14;
    rois.HSI14CalibrationValue = RCC_HSI14CALIBRATION_DEFAULT;
    rois.HSI14State = RCC_HSI14_ADC_CONTROL;
    HAL_RCC_OscConfig(&rois);

//    HAL_NVIC_SetPriority(ADCx_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ (ADCx_IRQn);

    ADC_ChannelConfTypeDef adc_ccfg;

    _hadc.Instance = ADC1;
    _hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    _hadc.Init.Resolution = ADC_RESOLUTION_12B;
    _hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    _hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
    _hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    _hadc.Init.LowPowerAutoWait = DISABLE;
    _hadc.Init.LowPowerAutoPowerOff = DISABLE;
    _hadc.Init.ContinuousConvMode = ENABLE;
    _hadc.Init.DiscontinuousConvMode = DISABLE;
    _hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    _hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    _hadc.Init.DMAContinuousRequests = ENABLE;
    _hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;

    if(HAL_ADC_Init(&_hadc) != HAL_OK) {
        Error_Handler();
    }
    adc_ccfg.Channel = ADC_CHANNEL_VBAT;
    adc_ccfg.Rank = 1;
    adc_ccfg.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    if(HAL_ADC_ConfigChannel(&_hadc, &adc_ccfg) != HAL_OK) {
        Error_Handler();
    }
    if(HAL_ADCEx_Calibration_Start(&_hadc) != HAL_OK) {
        Error_Handler();
    }
//    __HAL_ADC_ENABLE_IT(&hadc, (ADC_IT_OVR));
    HAL_ADC_Start(&_hadc);
}
