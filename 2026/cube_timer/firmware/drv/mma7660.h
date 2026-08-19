#ifndef _MMA7660_I2C_H
#define _MMA7660_I2C_H

#include <cstdint>
#include "zi2c.h"

// i2c addr: 0x4c

class MMA7660 {

  public:
    enum class MODE {
      STANDBY = 0, ACTIVE = 1
    };

    // TILT PoLa / BaFro decoded orientation (chip axes)
    enum class ORIENT {
      UNKNOWN = 0, UP, DOWN, LEFT, RIGHT, FRONT, BACK
    };

    // MODE (0x07) option bits (OR into set_mode flags)
    static constexpr uint8_t MODE_IPP = 0x40;    // INT push-pull
    static constexpr uint8_t MODE_IAH = 0x80;    // INT active-high

    // INTSU (0x06) bits
    static constexpr uint8_t INT_FBINT = 0x01;    // Front/Back change
    static constexpr uint8_t INT_PLINT = 0x02;    // Up/Down/Left/Right change
    static constexpr uint8_t INT_PDINT = 0x04;
    static constexpr uint8_t INT_GINT = 0x10;

    struct ACC {
        int16_t x, y, z;
    };

    MMA7660(const ZI2C& i2c) :
      _i2c(i2c)
    {
    }

    uint8_t read_mode(void)
    {
      return read_reg(REG::MODE);
    }

    // Returns true if MODE R/W looks alive (no WHO_AM_I on this part).
    bool probe(void)
    {
      set_mode(MODE::STANDBY);
      return read_mode() == 0x00;
    }

    // flags: MODE_IAH / MODE_IPP (must configure INTSU in Standby first).
    void set_mode(MODE mode, uint8_t flags = 0)
    {
      // Explicit value: clear TON (bit2). Avoid RMW of 0xFF on floating bus.
      uint8_t val = (mode == MODE::ACTIVE) ? 0x01 : 0x00;
      val |= (uint8_t)(flags & (MODE_IAH | MODE_IPP));
      _i2c.write_reg(static_cast<uint8_t>(REG::MODE), val);
    }

    // Must be called in Standby.
    void set_intsu(uint8_t mask)
    {
      _i2c.write_reg(static_cast<uint8_t>(REG::INTSU), mask);
    }

    // Must be called in Standby. filt: 0=no debounce .. 7=8 matching samples.
    // amsr: 0=120Hz (default), see datasheet SR AMSR[2:0].
    void set_sample_rate(uint8_t amsr, uint8_t filt)
    {
      uint8_t sr = (uint8_t)((filt & 0x7) << 5) | (amsr & 0x7);
      _i2c.write_reg(static_cast<uint8_t>(REG::SR), sr);
    }

    // Any I2C access clears INT; reading TILT also clears Tap/Shake status.
    void clear_int(void)
    {
      uint8_t tilt = 0;
      (void)read_tilt(tilt);
    }

    // Burst-read XYZ; retry whole set if any Alert (bit6) is set.
    bool read_acc(ACC& out)
    {
      uint8_t raw[3] = {0};

      for(int retry = 0; retry < ALERT_RETRIES; retry++) {
        _i2c.read_reg(static_cast<uint8_t>(REG::XOUT), raw, 3);
        if(((raw[0] | raw[1] | raw[2]) & 0x40) == 0) {
          out.x = convert_6bit_to_16bit(raw[0]);
          out.y = convert_6bit_to_16bit(raw[1]);
          out.z = convert_6bit_to_16bit(raw[2]);
          return true;
        }
      }
      return false;
    }

    bool read_tilt(uint8_t& tilt)
    {
      for(int retry = 0; retry < ALERT_RETRIES; retry++) {
        tilt = read_reg(REG::TILT);
        if((tilt & 0x40) == 0)
          return true;
      }
      return false;
    }

    ORIENT orient_from_tilt(uint8_t tilt) const
    {
      uint8_t pola = (tilt >> 2) & 0x7;
      uint8_t bafro = tilt & 0x3;

      switch(pola) {
        case 0x1:
          return ORIENT::LEFT;
        case 0x2:
          return ORIENT::RIGHT;
        case 0x5:
          return ORIENT::DOWN;
        case 0x6:
          return ORIENT::UP;
        default:
          break;
      }
      switch(bafro) {
        case 0x1:
          return ORIENT::FRONT;
        case 0x2:
          return ORIENT::BACK;
        default:
          return ORIENT::UNKNOWN;
      }
    }

    // Cone + hysteresis on gravity. Axis: +Z F, -Z B, -X U, +X D, -Y R, +Y L.
    ORIENT orient_update(const ACC& a, ORIENT prev, int cone_deg = 15) const
    {
      const int x = a.x, y = a.y, z = a.z;
      const int mag2 = x * x + y * y + z * z;
      if(mag2 < 100)
        return prev;

      int cos2_m;
      switch(cone_deg) {
        case 10:
          cos2_m = 970;
          break;
        case 20:
          cos2_m = 883;
          break;
        case 25:
          cos2_m = 821;
          break;
        case 30:
          cos2_m = 750;
          break;
        case 15:
        default:
          cos2_m = 933;
          break;
      }

      auto in_cone = [&](int comp) -> bool {
        return comp * comp * 1000 >= mag2 * cos2_m;
      };

      if(z > 0 && in_cone(z))
        return ORIENT::FRONT;
      if(z < 0 && in_cone(z))
        return ORIENT::BACK;
      if(x < 0 && in_cone(x))
        return ORIENT::UP;
      if(x > 0 && in_cone(x))
        return ORIENT::DOWN;
      if(y < 0 && in_cone(y))
        return ORIENT::RIGHT;
      if(y > 0 && in_cone(y))
        return ORIENT::LEFT;
      return prev;
    }

    void read_all_regs(int (*printf_f)(const char* fmt, ...))
    {
      for(int i = 0; i <= 0xa; i++)
        printf_f("%02x ", i);
      printf_f("\n");
      for(int i = 0; i <= 0xa; i++)
        printf_f("%02x ", _i2c.read_reg(i));
      printf_f("\n");
    }

  private:
    static constexpr int ALERT_RETRIES = 8;

    enum class REG {
      XOUT = 0x0, YOUT = 0x1, ZOUT = 0x2, TILT = 0x3, SRST = 0x4, SPCNT = 0x5,
      INTSU = 0x6, MODE = 0x7, SR = 0x8, PDET = 0x9, PD = 0xa
    };

    const ZI2C& _i2c;

    uint8_t read_reg(REG reg)
    {
      return _i2c.read_reg(static_cast<uint8_t>(reg));
    }

    static int16_t convert_6bit_to_16bit(uint8_t val)
    {
      val &= 0x3F;
      if(val & 0x20)
        return static_cast<int16_t>(val | 0xFFC0);
      return static_cast<int16_t>(val);
    }
};
#endif
