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
  zt.tick();
  millis_tick();
  charlie.poll_led();
}

#ifdef __cplusplus
}
#endif
