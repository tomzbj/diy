#ifndef __STM32F1xx_HAL_CONF_H
#define __STM32F1xx_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#define HAL_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_DAC_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED

#if !defined  (HSE_VALUE) 
#if defined(USE_STM3210C_EVAL)
#define HSE_VALUE    25000000U 
#else
#define HSE_VALUE    8000000U 
#endif
#endif 
#if !defined  (HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT    100U      
#endif 
#if !defined  (HSI_VALUE)
#define HSI_VALUE              8000000U  
#endif 
#if !defined  (LSI_VALUE) 
#define LSI_VALUE               40000U    
#endif 
    
#if !defined  (LSE_VALUE)
#define LSE_VALUE               32768U    
#endif 
#if !defined  (LSE_STARTUP_TIMEOUT)
#define LSE_STARTUP_TIMEOUT    5000U     
#endif 

#define  VDD_VALUE                    3300U 
#define  TICK_INT_PRIORITY            0x0FU 
#define  USE_RTOS                     0U
#define  PREFETCH_ENABLE              1U
#define  USE_HAL_ADC_REGISTER_CALLBACKS         1U
#define  USE_HAL_CAN_REGISTER_CALLBACKS         0U 
#define  USE_HAL_CEC_REGISTER_CALLBACKS         0U 
#define  USE_HAL_DAC_REGISTER_CALLBACKS         0U 
#define  USE_HAL_ETH_REGISTER_CALLBACKS         0U 
#define  USE_HAL_HCD_REGISTER_CALLBACKS         0U 
#define  USE_HAL_I2C_REGISTER_CALLBACKS         0U 
#define  USE_HAL_I2S_REGISTER_CALLBACKS         0U 
#define  USE_HAL_MMC_REGISTER_CALLBACKS         0U 
#define  USE_HAL_NAND_REGISTER_CALLBACKS        0U 
#define  USE_HAL_NOR_REGISTER_CALLBACKS         0U 
#define  USE_HAL_PCCARD_REGISTER_CALLBACKS      0U 
#define  USE_HAL_PCD_REGISTER_CALLBACKS         0U 
#define  USE_HAL_RTC_REGISTER_CALLBACKS         0U 
#define  USE_HAL_SD_REGISTER_CALLBACKS          0U 
#define  USE_HAL_SMARTCARD_REGISTER_CALLBACKS   0U 
#define  USE_HAL_IRDA_REGISTER_CALLBACKS        0U 
#define  USE_HAL_SRAM_REGISTER_CALLBACKS        0U 
#define  USE_HAL_SPI_REGISTER_CALLBACKS         0U 
#define  USE_HAL_TIM_REGISTER_CALLBACKS         0U 
#define  USE_HAL_UART_REGISTER_CALLBACKS        0U 
#define  USE_HAL_USART_REGISTER_CALLBACKS       0U 
#define  USE_HAL_WWDG_REGISTER_CALLBACKS        0U 


#define MAC_ADDR0   2U
#define MAC_ADDR1   0U
#define MAC_ADDR2   0U
#define MAC_ADDR3   0U
#define MAC_ADDR4   0U
#define MAC_ADDR5   0U
#define ETH_RX_BUF_SIZE                ETH_MAX_PACKET_SIZE 
#define ETH_TX_BUF_SIZE                ETH_MAX_PACKET_SIZE 
#define ETH_RXBUFNB                    8U                  
#define ETH_TXBUFNB                    4U                  
#define DP83848_PHY_ADDRESS             0x01U
#define PHY_RESET_DELAY                 0x000000FFU
#define PHY_CONFIG_DELAY                0x00000FFFU
#define PHY_READ_TO                     0x0000FFFFU
#define PHY_WRITE_TO                    0x0000FFFFU
#define PHY_BCR                         ((uint16_t)0x0000)  
#define PHY_BSR                         ((uint16_t)0x0001)  
#define PHY_RESET                       ((uint16_t)0x8000)  
#define PHY_LOOPBACK                    ((uint16_t)0x4000)  
#define PHY_FULLDUPLEX_100M             ((uint16_t)0x2100)  
#define PHY_HALFDUPLEX_100M             ((uint16_t)0x2000)  
#define PHY_FULLDUPLEX_10M              ((uint16_t)0x0100)  
#define PHY_HALFDUPLEX_10M              ((uint16_t)0x0000)  
#define PHY_AUTONEGOTIATION             ((uint16_t)0x1000)  
#define PHY_RESTART_AUTONEGOTIATION     ((uint16_t)0x0200)  
#define PHY_POWERDOWN                   ((uint16_t)0x0800)  
#define PHY_ISOLATE                     ((uint16_t)0x0400)  
#define PHY_AUTONEGO_COMPLETE           ((uint16_t)0x0020)  
#define PHY_LINKED_STATUS               ((uint16_t)0x0004)  
#define PHY_JABBER_DETECTION            ((uint16_t)0x0002)  
#define PHY_SR                          ((uint16_t)0x0010)  
#define PHY_MICR                        ((uint16_t)0x0011)  
#define PHY_MISR                        ((uint16_t)0x0012)  
#define PHY_LINK_STATUS                 ((uint16_t)0x0001)  
#define PHY_SPEED_STATUS                ((uint16_t)0x0002)  
#define PHY_DUPLEX_STATUS               ((uint16_t)0x0004)  
#define PHY_MICR_INT_EN                 ((uint16_t)0x0002)  
#define PHY_MICR_INT_OE                 ((uint16_t)0x0001)  
#define PHY_MISR_LINK_INT_EN            ((uint16_t)0x0020)  
#define PHY_LINK_INTERRUPT              ((uint16_t)0x2000)  

#define USE_SPI_CRC                     1U

#ifdef HAL_RCC_MODULE_ENABLED
#include "stm32f1xx_hal_rcc.h"
#endif 
#ifdef HAL_GPIO_MODULE_ENABLED
#include "stm32f1xx_hal_gpio.h"
#endif 
#ifdef HAL_EXTI_MODULE_ENABLED
#include "stm32f1xx_hal_exti.h"
#endif 
#ifdef HAL_DMA_MODULE_ENABLED
#include "stm32f1xx_hal_dma.h"
#endif 
#ifdef HAL_ETH_MODULE_ENABLED
#include "stm32f1xx_hal_eth.h"
#endif 
#ifdef HAL_CAN_MODULE_ENABLED
#include "stm32f1xx_hal_can.h"
#endif 
#ifdef HAL_CAN_LEGACY_MODULE_ENABLED
#include "Legacy/stm32f1xx_hal_can_legacy.h"
#endif 
#ifdef HAL_CEC_MODULE_ENABLED
#include "stm32f1xx_hal_cec.h"
#endif 
#ifdef HAL_CORTEX_MODULE_ENABLED
#include "stm32f1xx_hal_cortex.h"
#endif 
#ifdef HAL_ADC_MODULE_ENABLED
#include "stm32f1xx_hal_adc.h"
#endif 
#ifdef HAL_CRC_MODULE_ENABLED
#include "stm32f1xx_hal_crc.h"
#endif 
#ifdef HAL_DAC_MODULE_ENABLED
#include "stm32f1xx_hal_dac.h"
#endif 
#ifdef HAL_FLASH_MODULE_ENABLED
#include "stm32f1xx_hal_flash.h"
#endif 
#ifdef HAL_SRAM_MODULE_ENABLED
#include "stm32f1xx_hal_sram.h"
#endif 
#ifdef HAL_NOR_MODULE_ENABLED
#include "stm32f1xx_hal_nor.h"
#endif 
#ifdef HAL_I2C_MODULE_ENABLED
#include "stm32f1xx_hal_i2c.h"
#endif 
#ifdef HAL_I2S_MODULE_ENABLED
#include "stm32f1xx_hal_i2s.h"
#endif 
#ifdef HAL_IWDG_MODULE_ENABLED
#include "stm32f1xx_hal_iwdg.h"
#endif 
#ifdef HAL_PWR_MODULE_ENABLED
#include "stm32f1xx_hal_pwr.h"
#endif 
#ifdef HAL_RTC_MODULE_ENABLED
#include "stm32f1xx_hal_rtc.h"
#endif 
#ifdef HAL_PCCARD_MODULE_ENABLED
#include "stm32f1xx_hal_pccard.h"
#endif 
#ifdef HAL_SD_MODULE_ENABLED
#include "stm32f1xx_hal_sd.h"
#endif 
#ifdef HAL_NAND_MODULE_ENABLED
#include "stm32f1xx_hal_nand.h"
#endif 
#ifdef HAL_SPI_MODULE_ENABLED
#include "stm32f1xx_hal_spi.h"
#endif 
#ifdef HAL_TIM_MODULE_ENABLED
#include "stm32f1xx_hal_tim.h"
#endif 
#ifdef HAL_UART_MODULE_ENABLED
#include "stm32f1xx_hal_uart.h"
#endif 
#ifdef HAL_USART_MODULE_ENABLED
#include "stm32f1xx_hal_usart.h"
#endif 
#ifdef HAL_IRDA_MODULE_ENABLED
#include "stm32f1xx_hal_irda.h"
#endif 
#ifdef HAL_SMARTCARD_MODULE_ENABLED
#include "stm32f1xx_hal_smartcard.h"
#endif 
#ifdef HAL_WWDG_MODULE_ENABLED
#include "stm32f1xx_hal_wwdg.h"
#endif 
#ifdef HAL_PCD_MODULE_ENABLED
#include "stm32f1xx_hal_pcd.h"
#endif 
#ifdef HAL_HCD_MODULE_ENABLED
#include "stm32f1xx_hal_hcd.h"
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

