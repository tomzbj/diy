#ifndef _MEAS_H
#define _MEAS_H

#include "misc.h"

class MEAS {
  public:

    struct CBS {
        uint32_t (*millis_f)(void);
        void (*adc_get_f)(float* volt, float* curr);
        void (*set_curr_f)(float curr);
    };

    struct BATT_PARAMS {
        const char* name;
        float volt_terminal;
        float curr_limit;
    };

    enum {
      STOP, RUN
    };

    enum BATTERY_TYPE {
      AA, LIION, POWERBANK, BATT9V, DUMMYLOAD
    };

    MEAS(const CBS& cbs) :
      _cbs(cbs)
    {
    }

    void get_params(const char** name, float* volt, float* curr)
    {
      *name = _params[_battery_type].name;
      *volt = _params[_battery_type].volt_terminal;
      *curr = _params[_battery_type].curr_limit;
    }
    void get_capacity(float* pcapacity, float* penergy)
    {
      *pcapacity = _capacity;
      *penergy = _energy;
    }
    void poll(void)
    {
      float volt, curr;
      _cbs.adc_get_f(&volt, &curr);
      int inteval = _cbs.millis_f() - _last_millis;
      _last_millis = _cbs.millis_f();

      if(_state == STOP) {
        _cbs.set_curr_f(0.0f);
      }
      else if(_state == RUN) {
        if(_battery_type != DUMMYLOAD && volt < _params[_battery_type].volt_terminal) {
          _state = STOP;
          _cbs.set_curr_f(0.0f);
        }
        else {
          if(_battery_type != DUMMYLOAD) {
            _capacity += curr * inteval / 3.6e6f;
            _energy += curr * volt * inteval / 3.6e6f;
          }
          _cbs.set_curr_f(_params[_battery_type].curr_limit * _curr_ratio / 4.0f);
        }
      }
    }
    void key_up_press(void)
    {
      toggle_state();
    }
    void key_down_press(void)
    {
      toggle_curr_limit();
    }
    void key_up_longpress(void)
    {
      if(_state == STOP)
        toggle_battery_type();
    }
    void key_down_longpress(void)
    {
      if(_state == STOP)
        clear_capacity();
    }
    int get_state(void)
    {
      return _state;
    }
    bool is_dummy_load(void) const
    {
      return _battery_type == DUMMYLOAD;
    }

  private:
    const CBS& _cbs;

    float volt, curr, capacity;
    const BATT_PARAMS _params[5] = {
    //
      {"NIMH", 1.0f, 500.0f},    // for AA
      {"LI-ION", 3.0f, 990.0f},    // for Li-ion 18650
      {"POWERBANK", 4.0f, 990.0f},    // for powerbank
      {"6F22", 6.0f, 80.0f},    // 6F22
      {"DUMMYLOAD", 0.0f, 990.0f}    // dummy load
    };
    int _battery_type = 0;
    int _state;
    uint32_t _last_millis;
    int _curr_ratio = 1;
    float _capacity = 0.0f;
    float _energy = 0.0f;

    void toggle_battery_type(void)
    {
      ++_battery_type;
      _battery_type %= 5;
    }
    void toggle_state(void)
    {
      if(_state == STOP) {
        _state = RUN;
        _last_millis = _cbs.millis_f();
      }
      else if(_state == RUN) {
        _state = STOP;
      }
    }
    void toggle_curr_limit(void)
    {
      _curr_ratio++;
      if(_curr_ratio > 4)
        _curr_ratio = 1;
    }
    void clear_capacity(void)
    {
      _capacity = 0.0f;
      _energy = 0.0f;
    }

};

#endif
