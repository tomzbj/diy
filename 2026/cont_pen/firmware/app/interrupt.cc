#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

  void HardFault_Handler(void)
  {
    while(1) {
    }
  }

  void SysTick_Handler(void)
  {
    HAL_IncTick();
//    _delay_nms_mm();
    millis_tick();
  }

  void EXTI0_1_IRQHandler(void)
  {
    if(EXTI->PR & EXTI_PR_PR0)
      EXTI->PR = EXTI_PR_PR0;
  }

#ifdef __cplusplus
}
#endif
