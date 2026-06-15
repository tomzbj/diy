#ifndef _ZCHARLIE_H
#define _ZCHARLIE_H

#include <cstdint>
#include <memory>

class ZCHARLIE {
  public:
    enum {
      IN, L, H
    };
    typedef struct {
        int x, y;
    } leds_t;
    ZCHARLIE(const void (*set_dir_f)(int pin_id, int state), const leds_t* leds,
      const int num_leds, const int num_pins)
    : _set_dir_f(set_dir_f), _leds(leds), _num_leds(num_leds), _num_pins(num_pins),
      _num_digits(num_leds / 8)
    {
      _vram = std::make_unique<uint8_t[]>(_num_digits);
      _vram_shadow = std::make_unique<uint8_t[]>(_num_digits);
    }
    void poll_pin(void);
    void poll_led(void);
    void write(int n);
    void write(uint8_t* data, int count);
    void write(int n, int dot);
    void write(float n);

  private:
    std::unique_ptr<uint8_t[]> _vram, _vram_shadow;
    const void (*_set_dir_f)(int pin_id, int state);
    const leds_t* _leds;
    const int _num_leds, _num_pins, _num_digits;
    constexpr static uint8_t _mask[] = {    //
      0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f};
    int _poll_pin_count;
    int _poll_led_count;
    int _prev_x, _prev_y;
};
#endif
