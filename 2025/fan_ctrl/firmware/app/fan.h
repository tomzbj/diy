#ifndef _FAN_H
#define _FAN_H

#include <cstdint>

class FAN {
  public:
    void init(void);
    void off(void)
    {
      state_ = 0;
      update();
    }

    void toggle_state(void)
    {
      state_++;
      state_ %= 4;
      update();
    }

  private:
    int state_;
    const int pwm_val_[4] = {0, 200, 600, 980};
    const uint8_t led_[4] = {0x0, 0x4, 0xc, 0x4c};

    void update(void);
};

#endif
