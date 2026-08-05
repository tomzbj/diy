#include "adc_hal.h"
#include "charger.h"

void CHARGER::adjust(void)
{
  float volt = 0, curr = 0;
//  ADC_Get(&volt, &curr);
  _cbs.adc_get_f(&volt, &curr);

  if(curr < 3.0f) {
    _stat_str = "NO BATTERY";
    _cbs.set_voltcurr_f(1.0f, 1.0f);
  }
  else if(volt < 0.5f) {
    _stat_str = "BAD BATTERY";
    _cbs.set_voltcurr_f(3.0f, 10.0f);
  }
  else if(volt < 1.3f) {
    _stat_str = "NIMH";
    _cbs.set_voltcurr_f(1.4f, 300.0f);
  }
  else if(volt < 1.5f) {
    _stat_str = "NIMH";
    _cbs.set_voltcurr_f(1.6f, 25.0f);
  }
  else if(volt < 3.0f) {
    _stat_str = "LITHIUM";
    _cbs.set_voltcurr_f(3.2f, 50.0f);
  }
  else if(volt < 4.2f) {
    _stat_str = "LITHIUM";
    _cbs.set_voltcurr_f(4.2f, 250.0f);
  }
  else if(volt < 5.5f) {
    _stat_str = "LITHIUM";
    _cbs.set_voltcurr_f(4.2f, 50.0f);
  }
  else if(volt < 9.5f) {
    _stat_str = "6F22";
    _cbs.set_voltcurr_f(9.6f, 50.0f);
  }
}

void CHARGER::poll(void)
{
  _count++;
  _count %= 30;
  switch(_count) {
    case 0:    // detecting
      _cbs.set_voltcurr_f(9.5f, 10.0f);
      break;
    case 1:    // set state
      adjust();
      break;
    default:
      break;
  }
}
