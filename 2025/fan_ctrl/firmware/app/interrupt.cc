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
    u1.tick();
    zk.poll();
    ze.post(EVENTS::TIMER);
  }

#ifdef __cplusplus
}
#endif
