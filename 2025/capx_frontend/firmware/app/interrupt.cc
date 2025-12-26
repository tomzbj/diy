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
    _delay_nms_mm();
    u1.tick();
//    u2.tick();
  }

#ifdef __cplusplus
}
#endif
