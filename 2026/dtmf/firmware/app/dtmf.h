#ifndef _DTMF_MOD_H
#define _DTMF_MOD_H

#include "zdds.h"
#include <cstdio>
#include <memory>
#include <cmath>

class DTMF_DEM {
  public:
    DTMF_DEM(int sample_rate, int batch_size)    //
    : _sample_rate(sample_rate), _batch_size(batch_size)
    {
      for(int i = 0; i < 8; i++) {
        const int K = roundf((float)_batch_size * _freq[i] / _sample_rate);
        float omega = 2 * 3.141593 * K / _batch_size;
        _coeffs[i] = 2 * cosf(omega);
      }
    }
    void feed(void* ptr)
    {
      _cur_data = ptr;
      _adc_updated = true;
    }
    void view_data(void)
    {
      flag_view_data = true;
    }
    void view_power(void)
    {
      flag_view_power = true;
    }
    void view_time(void)
    {
      flag_view_time = true;
    }
    void poll(void);
  private:
    int _sample_rate, _batch_size;
    volatile void* _cur_data;
    bool _adc_updated = false;
    bool flag_view_data = false;
    bool flag_view_power = false;
    bool flag_view_time = false;
    const int _freq[8] = {697, 770, 852, 941, 1209, 1336, 1477, 1633};
    float _coeffs[8];
};

class DTMF_MOD {
  public:
    DTMF_MOD(ZDDS& dds, int duration, int inteval)    //
    : _dds(dds), _duration(duration), _inteval(inteval), _state(STATE::IDLE)
    {
    }
    void tx(const char* msg, int len);
    void poll(void);

  private:
    enum class STATE {
      IDLE, SENDING, INTEVAL
    };

    ZDDS& _dds;
    int _duration, _inteval;
    STATE _state;
    char _msg[128];
    int _pos, _len;
    int _counter;
};
#endif

