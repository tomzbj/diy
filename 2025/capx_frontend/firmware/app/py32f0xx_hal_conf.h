#ifndef __PY32F0xx_HAL_CONF_H
#define __PY32F0xx_HAL_CONF_H
#ifdef __cplusplus
extern "C" {
#endif

#include "py32f0xx.h"

#define HAL_MODULE_ENABLED  
#define HAL_RCC_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
// #define HAL_CRC_MODULE_ENABLED
// #define HAL_COMP_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED   
#define HAL_GPIO_MODULE_ENABLED    
// #define HAL_IWDG_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
// #define HAL_LPTIM_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
// #define HAL_I2C_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
// #define HAL_EXTI_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED  
#define HAL_DMA_MODULE_ENABLED

#if !defined  (HSI_VALUE) 
#define HSI_VALUE              ((uint32_t)8000000)     
#endif 

#if !defined  (HSE_VALUE) 
#define HSE_VALUE              ((uint32_t)24000000) 
#endif 
#if !defined  (HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT    ((uint32_t)200)   
#endif 

#if !defined  (LSI_VALUE) 
#define LSI_VALUE               32768U    
#endif 
/*!< Value of the Internal Low Speed oscillator in Hz
 The real value may vary depending on the variations
 in voltage and temperature. */
#if defined (RCC_LSE_SUPPORT)
#if !defined  (LSE_VALUE)
#define LSE_VALUE              ((uint32_t)32768) 
#endif 
#if !defined  (LSE_STARTUP_TIMEOUT)
#define LSE_STARTUP_TIMEOUT    ((uint32_t)5000)   
#endif 
#endif

#define  VDD_VALUE               ((uint32_t)3300) 
#define  PRIORITY_HIGHEST        0
#define  PRIORITY_HIGH           1
#define  PRIORITY_LOW            2
#define  PRIORITY_LOWEST         3
#define  TICK_INT_PRIORITY       ((uint32_t)PRIORITY_LOWEST)    
#define  USE_RTOS                0
#define  PREFETCH_ENABLE         1

#ifdef HAL_MODULE_ENABLED
#include "py32f0xx_hal.h"
#endif 

#ifdef HAL_DMA_MODULE_ENABLED
#include "py32f0xx_hal_dma.h"
#endif 
#ifdef HAL_RCC_MODULE_ENABLED
#include "py32f0xx_hal_rcc.h"
#endif 
#ifdef HAL_EXTI_MODULE_ENABLED
#include "py32f0xx_hal_exti.h"
#endif 
#ifdef HAL_GPIO_MODULE_ENABLED
#include "py32f0xx_hal_gpio.h"
#endif 
#ifdef HAL_CORTEX_MODULE_ENABLED
#include "py32f0xx_hal_cortex.h"
#endif 
#ifdef HAL_ADC_MODULE_ENABLED
#include "py32f0xx_hal_adc.h"
#endif 
#ifdef HAL_CRC_MODULE_ENABLED
#include "py32f0xx_hal_crc.h"
#endif 
#ifdef HAL_COMP_MODULE_ENABLED
#include "py32f0xx_hal_comp.h"
#endif 
#ifdef HAL_FLASH_MODULE_ENABLED
#include "py32f0xx_hal_flash.h"
#endif 
#ifdef HAL_I2C_MODULE_ENABLED
#include "py32f0xx_hal_i2c.h"
#endif 
#ifdef HAL_IWDG_MODULE_ENABLED
#include "py32f0xx_hal_iwdg.h"
#endif 
#ifdef HAL_PWR_MODULE_ENABLED
#include "py32f0xx_hal_pwr.h"
#endif 
#ifdef HAL_SPI_MODULE_ENABLED
#include "py32f0xx_hal_spi.h"
#endif 
#ifdef HAL_TIM_MODULE_ENABLED
#include "py32f0xx_hal_tim.h"
#endif 
#ifdef HAL_LPTIM_MODULE_ENABLED
#include "py32f0xx_hal_lptim.h" 
#endif 
#ifdef HAL_UART_MODULE_ENABLED
#include "py32f0xx_hal_uart.h"
#endif 
#ifdef HAL_USART_MODULE_ENABLED
#include "py32f0xx_hal_usart.h"
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
