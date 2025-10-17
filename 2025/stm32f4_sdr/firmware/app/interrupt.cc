#include "misc.h"
#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

  void SysTick_Handler(void)
  {
    _delay_nms_mm();            // executes every 1ms
    u0.tick();
//    os_tick();
    zk.poll();
    {
      ze.post(ZEVENT::evt_t {.id = EVENTS::MILLISECOND, .data = 0});
    }
  }

  void HardFault_Handler(void)
  {
    while(1);
  }

#ifdef __cplusplus
}
#endif
