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
    u1.tick();
    ucdc.tick();
    millis_tick();
    zt.tick();
  }

#ifdef __cplusplus
}
#endif
