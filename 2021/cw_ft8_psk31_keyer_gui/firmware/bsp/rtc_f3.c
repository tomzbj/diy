#include "misc.h"

void RTC_GetTimeString(char* string)
{
    RTC_TimeTypeDef rts;
    RTC_GetTime(RTC_Format_BIN, &rts);
    sprintf(string, "%02d:%02d:%02d", rts.RTC_Hours, rts.RTC_Minutes,
        rts.RTC_Seconds);
}

void RTC_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    if(RTC_ReadBackupRegister(RTC_BKP_DR1) != 0x26992699) {
         RTC_InitTypeDef rit;
         RCC_BackupResetCmd(ENABLE);
         RCC_BackupResetCmd(DISABLE);

         RCC_LSEConfig(RCC_LSE_ON);
         while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
         RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

         rit.RTC_AsynchPrediv = 0x7f;
         rit.RTC_SynchPrediv = 0xff;
         rit.RTC_HourFormat = RTC_HourFormat_24;
         RTC_Init(&rit);

         RCC_RTCCLKCmd(ENABLE);
         RTC_WaitForSynchro();

         RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
         PWR_BackupAccessCmd(ENABLE);
         RTC_WaitForSynchro();
         RTC_WriteBackupRegister(RTC_BKP_DR1, 0x26992699);
    }
}

/*
 #define RTC_CLOCK_SOURCE_LSE
 RTC_InitTypeDef RTC_InitStructure;
 RTC_AlarmTypeDef RTC_AlarmStructure;
 __IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

 #if defined (RTC_CLOCK_SOURCE_LSI)
 RCC_LSICmd(ENABLE);
 #endif
 if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2) {
 RTC_Config();
 RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
 RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
 RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
 if(RTC_Init(&RTC_InitStructure) == ERROR) {
 }
 RTC_TimeRegulate();
 }
 else {
 if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) {
 printf("\r\n Power On Reset occurred....\n\r");
 }
 else if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) {
 printf("\r\n External Reset occurred....\n\r");
 }
 printf("\n\r No need to configure RTC....\n\r");
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
 PWR_BackupAccessCmd(ENABLE);
 RTC_WaitForSynchro();
 RTC_ClearFlag(RTC_FLAG_ALRAF);
 EXTI_ClearITPendingBit(EXTI_Line17);
 RTC_TimeShow();
 RTC_AlarmShow();
 }
 }

 void RTC_TimeRegulate(void)
 {
 uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;
 printf(
 "\n\r==============Time Settings=====================================\n\r");
 RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
 printf("  Please Set Hours:\n\r");
 while(tmp_hh == 0xFF) {
 tmp_hh = USART_Scanf(23);
 RTC_TimeStructure.RTC_Hours = tmp_hh;
 }
 printf("  %0.2d\n\r", tmp_hh);
 printf("  Please Set Minutes:\n\r");
 while(tmp_mm == 0xFF) {
 tmp_mm = USART_Scanf(59);
 RTC_TimeStructure.RTC_Minutes = tmp_mm;
 }
 printf("  %0.2d\n\r", tmp_mm);
 printf("  Please Set Seconds:\n\r");
 while(tmp_ss == 0xFF) {
 tmp_ss = USART_Scanf(59);
 RTC_TimeStructure.RTC_Seconds = tmp_ss;
 }
 printf("  %0.2d\n\r", tmp_ss);
 if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR) {
 printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
 }
 else {
 printf("\n\r>> !! RTC Set Time success. !! <<\n\r");
 RTC_TimeShow();
 RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
 }
 tmp_hh = 0xFF;
 tmp_mm = 0xFF;
 tmp_ss = 0xFF;
 RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
 printf(
 "\n\r==============Alarm A Settings=====================================\n\r");
 RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
 printf("  Please Set Alarm Hours:\n\r");
 while(tmp_hh == 0xFF) {
 tmp_hh = USART_Scanf(23);
 RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = tmp_hh;
 }
 printf("  %0.2d\n\r", tmp_hh);
 printf("  Please Set Alarm Minutes:\n\r");
 while(tmp_mm == 0xFF) {
 tmp_mm = USART_Scanf(59);
 RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = tmp_mm;
 }
 printf("  %0.2d\n\r", tmp_mm);
 printf("  Please Set Alarm Seconds:\n\r");
 while(tmp_ss == 0xFF) {
 tmp_ss = USART_Scanf(59);
 RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = tmp_ss;
 }
 printf("  %0.2d", tmp_ss);
 RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_Weekday_Monday;
 RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
 RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
 RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
 printf("\n\r>> !! RTC Set Alarm success. !! <<\n\r");
 RTC_AlarmShow();
 RTC_ITConfig(RTC_IT_ALRA, ENABLE);
 RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
 }
 void RTC_TimeShow(void)
 {
 RTC_TimeStructInit(&RTC_TimeStructure);
 RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
 printf("\n\r  The current time is :  %0.2d:%0.2d:%0.2d \n\r",
 RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes,
 RTC_TimeStructure.RTC_Seconds);
 }
 void RTC_AlarmShow(void)
 {
 RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
 printf("\n\r  The current alarm is :  %0.2d:%0.2d:%0.2d \n\r",
 RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours,
 RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes,
 RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
 }
 uint8_t USART_Scanf(uint32_t value)
 {
 uint32_t index = 0;
 uint32_t tmp[2] = {0, 0};
 while(index < 2) {
 while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET) {
 }
 tmp[index++] = (USART_ReceiveData(EVAL_COM1));
 if((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39)) {
 printf("\n\r Please enter valid number between 0 and 9 \n\r");
 index--;
 }
 }
 index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
 if(index > value) {
 printf("\n\r Please enter valid number between 0 and %d \n\r", value);
 return 0xFF;
 }
 return index;
 }
 PUTCHAR_PROTOTYPE
 {
 USART_SendData(EVAL_COM1, (uint8_t) ch);
 while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
 {}
 return ch;
 }
 #ifdef  USE_FULL_ASSERT
 void assert_failed(uint8_t* file, uint32_t line)
 {
 while (1)
 {
 }
 }
 #endif

 */
