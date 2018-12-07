#ifndef __SYSTEM_STM32F10X_H
#define __SYSTEM_STM32F10X_H

extern const uint32_t SystemFrequency;                   /*!< System Clock Frequency (Core Clock) */
extern const uint32_t SystemFrequency_SysClk;            /*!< System clock                        */
extern const uint32_t SystemFrequency_AHBClk;            /*!< AHB System bus speed                */
extern const uint32_t SystemFrequency_APB1Clk;           /*!< APB Peripheral Bus 1 (low)  speed   */
extern const uint32_t SystemFrequency_APB2Clk;           /*!< APB Peripheral Bus 2 (high) speed   */

extern void SystemInit(void);
#endif /*__SYSTEM_STM32F10X_H */
