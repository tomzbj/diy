#include "misc.h"
#include "counter.h"

COUNTER counter;

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
  if(htim->Instance == TIM1)    // carry
    counter.carry();
  else if(htim->Instance == TIM16)    // gate
    counter.update();
}
