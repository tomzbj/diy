#include "misc.h"
#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

void SysTick_Handler(void)
{
  HAL_IncTick();
#if APP_USART_ENABLE
  u1.tick();
#endif
  zt.tick();
  millis_tick();
  ALARM_Systick();
}

void HardFault_Handler(void)
{
  while(1);
}

#ifdef __cplusplus
}
#endif
