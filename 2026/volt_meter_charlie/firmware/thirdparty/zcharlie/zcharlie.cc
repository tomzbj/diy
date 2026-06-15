#include "zcharlie.h"
#include <cstdint>
#include <cstring>

void ZCHARLIE::poll_pin(void)
{
  if(_poll_pin_count == 0)
    memcpy(_vram.get(), _vram_shadow.get(), _num_digits);
  ++_poll_pin_count %= _num_pins;
  const auto& count = _poll_pin_count;

  for(int y = 0; y < _num_pins; y++) {
    _set_dir_f(y, IN);
  }

//  for(int y = 0; y < _num_pins; y++) { _set_dir_f(y, (count == y) ? L : IN); }
  for(int i = 0; i < _num_leds; i++) {
    int x = _leds[i].x - 1;
    int y = _leds[i].y - 1;
    if(y != count)
      continue;
    if(_vram[i / 8] & (1 << (i % 8))) {
      _set_dir_f(x, H);
    }
  }
  _set_dir_f(count, L);
}

void ZCHARLIE::poll_led(void)
{
  if(_poll_led_count == 0)
    memcpy(_vram.get(), _vram_shadow.get(), _num_digits);
  ++_poll_led_count %= _num_leds;
  const auto& count = _poll_led_count;
  _set_dir_f(_prev_x, IN);
  _set_dir_f(_prev_y, IN);
  int x = _leds[count].x - 1;
  int y = _leds[count].y - 1;
  if(_vram[count / 8] & (1 << (count % 8))) {
    _set_dir_f(x, H);
    _set_dir_f(y, L);
  }
  _prev_x = x;
  _prev_y = y;
}

void ZCHARLIE::write(float n)
{
  if(n >= 1000.0) {
    _vram[2] = 0x79;
    _vram[1] = 0x50;
    _vram[0] = 0x50;
  }
  else if(n >= 100.0) {
    write(int(n));
  }
  else if(n >= 10.0) {
    write(int(n * 10), 1);
  }
  else {
    write(int(n * 100), 2);
  }
}

void ZCHARLIE::write(int n, int dot)
{
  for(int i = 0; i < _num_digits; i++) {
    _vram_shadow[i] = _mask[n % 10];
    if(dot == i)
      _vram_shadow[i] |= 0x80;
    n /= 10;
  }
}

void ZCHARLIE::write(int n)
{
  for(int i = 0; i < _num_digits; i++) {
    _vram_shadow[i] = _mask[n % 10];
    n /= 10;
  }
}

void ZCHARLIE::write(uint8_t* data, int count)
{
  if(count > _num_digits)
    count = _num_digits;
  for(int i = 0; i < count; i++) {
    _vram_shadow[i] = data[i];
  }
}
