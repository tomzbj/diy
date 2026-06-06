#include "misc.h"
#include "zuart4.h"

#ifdef __cplusplus
extern "C" {
#endif

  void SysTick_Handler(void)
  {
    extern class ZUART4 u1;
    HAL_IncTick();
    u1.tick();
    millis_tick();
  }

  void MemManage_Handler(void)
  {
    while(1);
  }
  void BusFault_Handler(void)
  {
    while(1);
  }
  void UsageFault_Handler(void)
  {
    while(1);
  }
  void HardFault_Handler(void)
  {
    while(1);
  }

#ifdef __cplusplus
}
#endif
