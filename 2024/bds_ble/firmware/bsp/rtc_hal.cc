#include "misc.h"
#include "rtc_hal.h"

static auto Error_Handler = +[] {};

// HSE_DIV32 as RTC_CLKSource, 16M / 32 = 500k, 500k / 125 / 4000 = 1
#define RTC_ASYNCH_PREDIV  (125 - 1)
#define RTC_SYNCH_PREDIV   (4000 - 1)

void CRTC::set_time(int h, int m, int s)
{
    RTC_TimeTypeDef rts;
    rts.Hours = h;
    rts.Minutes = m;
    rts.Seconds = s;
    rts.TimeFormat = RTC_HOURFORMAT_24;
    rts.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    rts.StoreOperation = RTC_STOREOPERATION_RESET;
    if(HAL_RTC_SetTime(&_hrtc, &rts, RTC_FORMAT_BIN) != HAL_OK) {
        Error_Handler();
    }
}

void CRTC::get_time(int* h, int* m, int* s)
{
    RTC_TimeTypeDef rts;
    RTC_DateTypeDef rds;
    HAL_RTC_GetTime(&_hrtc, &rts, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&_hrtc, &rds, RTC_FORMAT_BIN);
    *h = rts.Hours;
    *m = rts.Minutes;
    *s = rts.Seconds;
}

void CRTC::show_calendar(void)
{
    RTC_DateTypeDef rds;
    RTC_TimeTypeDef rts;
    HAL_RTC_GetTime(&_hrtc, &rts, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&_hrtc, &rds, RTC_FORMAT_BIN);
    printf("%2d:%2d:%2d\t", rts.Hours, rts.Minutes, rts.Seconds);
    printf("%2d-%2d-%2d\n\n", rds.Month, rds.Date, 2000 + rds.Year);
}

void CRTC::init(void)
{
//    RCC_OscInitTypeDef rois;
    RCC_PeriphCLKInitTypeDef rpcis;
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    /* rois.OscillatorType = RCC_OSCILLATORTYPE_LSI;
     rois.PLL.PLLState = RCC_PLL_NONE;
     rois.LSEState = RCC_LSI_ON;
     if(HAL_RCC_OscConfig(&rois) != HAL_OK) {
     Error_Handler();
     } */
    rpcis.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    rpcis.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV32;    // 16M / 32 = 500k
    if(HAL_RCCEx_PeriphCLKConfig(&rpcis) != HAL_OK) {
        Error_Handler();
    }
    __HAL_RCC_RTC_ENABLE();

    _hrtc.Instance = RTC;
    _hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    _hrtc.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
    _hrtc.Init.SynchPrediv = RTC_SYNCH_PREDIV;
    _hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    _hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    _hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&_hrtc) != HAL_OK) {
        Error_Handler();
    }
    __HAL_RCC_CLEAR_RESET_FLAGS();
    set_time(0, 0, 0);
    HAL_RTCEx_BKUPWrite(&_hrtc, RTC_BKP_DR1, 0x32F2);
}
