#ifndef __STM32F0xx_HAL_CONF_H
#define __STM32F0xx_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal_def.h"

#define HAL_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
//#define HAL_EXTI_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_USART_MODULE_ENABLED
#define HAL_RTC_MODULE_ENABLED
//#define HAL_SPI_MODULE_ENABLED
//#define HAL_TIM_MODULE_ENABLED

#if !defined  (HSE_VALUE) 
#define HSE_VALUE            8000000U  
#endif 
#if !defined  (HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT  100U      
#endif 
#if !defined  (HSI_VALUE)
#define HSI_VALUE            8000000U  
#endif 
#if !defined  (HSI_STARTUP_TIMEOUT) 
#define HSI_STARTUP_TIMEOUT  5000U     
#endif 
#if !defined  (HSI14_VALUE) 
#define HSI14_VALUE          14000000U 
#endif 
#if !defined  (HSI48_VALUE) 
#define HSI48_VALUE          48000000U 
#endif 
#if !defined  (LSI_VALUE) 
#define LSI_VALUE            32000U    
#endif 

#if !defined  (LSE_VALUE)
#define LSE_VALUE            32768U    
#endif 
#if !defined  (LSE_STARTUP_TIMEOUT)
#define LSE_STARTUP_TIMEOUT  5000U     
#endif 
#define  VDD_VALUE                    3300U  
#define  TICK_INT_PRIORITY            ((uint32_t)(1U<<__NVIC_PRIO_BITS) - 1U) 
#define  USE_RTOS                     0U
#define  PREFETCH_ENABLE              1U
#define  INSTRUCTION_CACHE_ENABLE     0U
#define  DATA_CACHE_ENABLE            0U
#define  USE_SPI_CRC                  1U
#define  USE_HAL_ADC_REGISTER_CALLBACKS         0U 
#define  USE_HAL_CAN_REGISTER_CALLBACKS         0U 
#define  USE_HAL_COMP_REGISTER_CALLBACKS        0U 
#define  USE_HAL_CEC_REGISTER_CALLBACKS         0U 
#define  USE_HAL_DAC_REGISTER_CALLBACKS         0U 
#define  USE_HAL_I2C_REGISTER_CALLBACKS         0U 
#define  USE_HAL_SMBUS_REGISTER_CALLBACKS       0U 
#define  USE_HAL_UART_REGISTER_CALLBACKS        0U
#define  USE_HAL_USART_REGISTER_CALLBACKS       0U 
#define  USE_HAL_IRDA_REGISTER_CALLBACKS        0U 
#define  USE_HAL_SMARTCARD_REGISTER_CALLBACKS   0U 
#define  USE_HAL_WWDG_REGISTER_CALLBACKS        0U 
#define  USE_HAL_RTC_REGISTER_CALLBACKS         0U 
#define  USE_HAL_SPI_REGISTER_CALLBACKS         0U 
#define  USE_HAL_I2S_REGISTER_CALLBACKS         0U 
#define  USE_HAL_TIM_REGISTER_CALLBACKS         0U 
#define  USE_HAL_TSC_REGISTER_CALLBACKS         0U 
#define  USE_HAL_PCD_REGISTER_CALLBACKS         0U 
#ifdef HAL_RCC_MODULE_ENABLED
#include "stm32f0xx_hal_rcc.h"
#endif 
#ifdef HAL_GPIO_MODULE_ENABLED
#include "stm32f0xx_hal_gpio.h"
#endif 
#ifdef HAL_EXTI_MODULE_ENABLED
#include "stm32f0xx_hal_exti.h"
#endif 
#ifdef HAL_DMA_MODULE_ENABLED
#include "stm32f0xx_hal_dma.h"
#endif 
#ifdef HAL_CORTEX_MODULE_ENABLED
#include "stm32f0xx_hal_cortex.h"
#endif 
#ifdef HAL_ADC_MODULE_ENABLED
#include "stm32f0xx_hal_adc.h"
#endif 
#ifdef HAL_CAN_MODULE_ENABLED
#include "stm32f0xx_hal_can.h"
#endif 
#ifdef HAL_CEC_MODULE_ENABLED
#include "stm32f0xx_hal_cec.h"
#endif 
#ifdef HAL_COMP_MODULE_ENABLED
#include "stm32f0xx_hal_comp.h"
#endif 
#ifdef HAL_CRC_MODULE_ENABLED
#include "stm32f0xx_hal_crc.h"
#endif 
#ifdef HAL_DAC_MODULE_ENABLED
#include "stm32f0xx_hal_dac.h"
#endif 
#ifdef HAL_FLASH_MODULE_ENABLED
#include "stm32f0xx_hal_flash.h"
#endif 
#ifdef HAL_I2C_MODULE_ENABLED
#include "stm32f0xx_hal_i2c.h"
#endif 
#ifdef HAL_I2S_MODULE_ENABLED
#include "stm32f0xx_hal_i2s.h"
#endif 
#ifdef HAL_IRDA_MODULE_ENABLED
#include "stm32f0xx_hal_irda.h"
#endif 
#ifdef HAL_IWDG_MODULE_ENABLED
#include "stm32f0xx_hal_iwdg.h"
#endif 
#ifdef HAL_PCD_MODULE_ENABLED
#include "stm32f0xx_hal_pcd.h"
#endif 
#ifdef HAL_PWR_MODULE_ENABLED
#include "stm32f0xx_hal_pwr.h"
#endif 
#ifdef HAL_RTC_MODULE_ENABLED
#include "stm32f0xx_hal_rtc.h"
#endif 
#ifdef HAL_SMARTCARD_MODULE_ENABLED
#include "stm32f0xx_hal_smartcard.h"
#endif 
#ifdef HAL_SMBUS_MODULE_ENABLED
#include "stm32f0xx_hal_smbus.h"
#endif 
#ifdef HAL_SPI_MODULE_ENABLED
#include "stm32f0xx_hal_spi.h"
#endif 
#ifdef HAL_TIM_MODULE_ENABLED
#include "stm32f0xx_hal_tim.h"
#endif 
#ifdef HAL_TSC_MODULE_ENABLED
#include "stm32f0xx_hal_tsc.h"
#endif 
#ifdef HAL_UART_MODULE_ENABLED
#include "stm32f0xx_hal_uart.h"
#endif 
#ifdef HAL_USART_MODULE_ENABLED
#include "stm32f0xx_hal_usart.h"
#endif 
#ifdef HAL_WWDG_MODULE_ENABLED
#include "stm32f0xx_hal_wwdg.h"
#endif 
#ifdef  USE_FULL_ASSERT
#define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
    void assert_failed(uint8_t* file, uint32_t line);
#else
#define assert_param(expr) ((void)0U)
#endif 
#ifdef __cplusplus
}
#endif
#endif 
