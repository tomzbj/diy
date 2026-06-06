#include "misc.h"
#include "platform.h"

static RTC_HandleTypeDef hrtc;

void RTC_Show(void)
{
  RTC_TimeTypeDef rtis = {0};
  HAL_RTC_GetTime(&hrtc, &rtis, RTC_FORMAT_BIN);
  printf("%02d:%02d:%02d\n", rtis.Hours, rtis.Minutes, rtis.Seconds);
}

void RTC_SetTime(uint32_t bcd_time)
{
  RTC_DateTypeDef rdis;
  RTC_TimeTypeDef rtis;
  rdis.Year = 0x14;
  rdis.Month = RTC_MONTH_FEBRUARY;
  rdis.Date = 0x18;
  rdis.WeekDay = RTC_WEEKDAY_TUESDAY;
  test_if(HAL_RTC_SetDate(&hrtc, &rdis, RTC_FORMAT_BIN));
  rtis.Hours = (bcd_time / 10000) % 100;
  rtis.Minutes = (bcd_time / 100) % 100;
  rtis.Seconds = bcd_time % 100;
  test_if(HAL_RTC_SetTime(&hrtc, &rtis, RTC_FORMAT_BIN));
}

void RTC_Config(void)
{
  {
    RCC_OscInitTypeDef rois;
    RCC_PeriphCLKInitTypeDef pckis;
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_BKP_CLK_ENABLE();
    rois.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    rois.PLL.PLLState = RCC_PLL_NONE;
    rois.LSEState = RCC_LSE_ON;
    test_if(HAL_RCC_OscConfig(&rois));
    pckis.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    pckis.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    test_if(HAL_RCCEx_PeriphCLKConfig(&pckis));
    __HAL_RCC_RTC_ENABLE();

    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    test_if(HAL_RTC_Init(&hrtc));
  }

  print_log();
}
