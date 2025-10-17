#include <cstdio>
#include <cstdint>
#include <cmath>

class DC_BLOCKER {
  public:
    DC_BLOCKER(int rn, int rd) : _rn(rn), _rd(rd), _oldx(0), _oldy(0)
    {
    }
    int32_t block(int32_t x, int rn, int rd)
    {
      int32_t y = x - _oldx + SDIVP2(_oldy * rn, rd);
      _oldx = x;
      _oldy = y;
      return y;
    }

    void block(int16_t* in_data, int16_t* out_data, int size)
    {
      for(int i = 0; i < size; i++) {
        out_data[i] = block(in_data[i], _rn, _rd);
      }
    }

    void dc_remove(int16_t* in_data, int16_t* out_data, int size)
    {
      static int32_t offset;
      int32_t sum = 0;
      for(int i = 0; i < size / 16; i++) {
        for(int j = 0; j < 16; j++) {
          auto& t = in_data[i * 16 + j];
          sum += t;
          out_data[i * 16 + j] = t - offset;
        }
      }
      offset = sum / size;
    }

  private:
    static inline int32_t SDIVP2(int32_t a, int32_t p)
    {
      return (a >= 0) ? (a >> p) : -((-a) >> (p));
    }
    int _rn, _rd;
    int32_t _oldx, _oldy;
};

