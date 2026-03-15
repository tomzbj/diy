#include "misc.h"
#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

void SysTick_Handler(void)
{
  HAL_IncTick();
  u1.tick();
  zt.tick();
  millis_tick();
  if(0) {
    static int count = 0;
    ++count %= 1000;
    if(count == 0)
      printf("%08lX\n", USART1->CR1);
  }
}

void HardFault_Handler(void)
{
  while(1);
}

#ifdef __cplusplus
}
#endif
