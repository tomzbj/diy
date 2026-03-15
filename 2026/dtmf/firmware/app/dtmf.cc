#include "misc.h"
#include <string.h>
#include "dtmf.h"
#include <algorithm>

typedef struct {
    char ch;
    int low, hi;
} tone_t;

tone_t tone[] = {
//
  {'1', 697, 1209}, {'2', 697, 1336}, {'3', 697, 1477}, {'4', 770, 1209},    //
  {'5', 770, 1336}, {'6', 770, 1477}, {'7', 852, 1209}, {'8', 852, 1336},    //
  {'9', 852, 1477}, {'A', 697, 1633}, {'B', 770, 1633}, {'C', 852, 1633},    //
  {'D', 941, 1633}, {'#', 941, 1477}, {'0', 941, 1336}, {'*', 941, 1209},    //
  };

static tone_t get_tone(char ch)
{
  for(const auto v : tone) {
    if(v.ch == ch)
      return v;
  }
  return tone_t {'$', 0, 0};
}

void DTMF_MOD::tx(const char* msg, int len)
{
  if(_state != STATE::IDLE)
    return;
  memset(_msg, '\0', sizeof(_msg));
  strncpy(_msg, msg, len);
  _pos = 0;
  _len = len;
  _state = STATE::SENDING;
  auto [c, x, y] = get_tone(_msg[0]);
  _dds.set_freq(x, y);
  ++_pos;
  _counter = 0;
}

void DTMF_MOD::poll(void)    // will be called every 1ms
{
  if(_state == STATE::IDLE)
    return;
  ++_counter;
  if(_state == STATE::SENDING && _counter == _duration) {
    _dds.set_freq(0, 0);
    if(_pos >= _len)
      _state = STATE::IDLE;
    else
      _state = STATE::INTEVAL;
    _counter = 0;
  }
  else if(_state == STATE::INTEVAL && _counter == _inteval) {
//    auto [x, y] = tone[_msg[_pos]];
    auto [c, x, y] = get_tone(_msg[_pos]);
    ++_pos;
    _dds.set_freq(x, y);
    _state = STATE::SENDING;
    _counter = 0;
  }
}

void DTMF_DEM::poll(void)
{

  if(_adc_updated == false)
    return;

  volatile uint32_t t1 = DWT->CYCCNT;

  if(flag_view_data) {
    printf("%d\n", _batch_size);
    uint16_t* p = (uint16_t*)_cur_data;
    for(int i = 0; i < _batch_size; i++) {
      printf("%d\n", p[i]);
    }
    flag_view_data = false;
  }

  float P[8];

  uint16_t* p = (uint16_t*)_cur_data;
  for(int j = 0; j < 8; j++) {
    const auto& C = _coeffs[j];

    float Q0 = 0, Q1 = 0, Q2 = 0;
    for(int i = 0; i < _batch_size; i++) {
//      Q0 = C * Q1 - Q2 + _workbuf[i];
      Q0 = C * Q1 - Q2 + float(p[i] - 2048);
      Q2 = Q1;
      Q1 = Q0;
    }
    P[j] = Q1 * Q1 + Q2 * Q2 - C * Q1 * Q2;
  }
  if(flag_view_power) {
    for(int i = 0; i < 8; i++) {
      printf("%4d:%-8.3f", _freq[i], P[i]);
    }
    printf("\n");
    printf("\n");
    flag_view_power = false;
  }
  int max_low = 0, max_hi = 4;
  for(int i = 1; i <= 3; i++) {
    if(P[i] > P[max_low])
      max_low = i;
    if(P[i + 4] > P[max_hi])
      max_hi = i + 4;
  }

  static char prev_c = 'X';
  if(P[max_low] < 3e9 && P[max_hi] < 3e9) {
    prev_c = 'X';
    return;
  }
  const char C[4][4] = { {'1', '2', '3', 'A'}, {'4', '5', '6', 'B'},    //
    {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};
  char curr_c = C[max_low][max_hi - 4];
  if(curr_c != prev_c) {
    printf("%c%c\n", curr_c, prev_c);
//    printf("%c\n", curr_c);
    fflush(stdout);
  }
  prev_c = curr_c;
  volatile uint32_t t2 = DWT->CYCCNT;
  if(flag_view_time) {
    printf("cycles: %lu $\n", t2 - t1);
    flag_view_time = false;
  }

}
