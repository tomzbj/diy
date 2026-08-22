/* Author: tomzbj  Updated: 2024-12-03 Rev: 0.01 */

#ifndef _ST7305_H
#define _ST7305_H

#include <cstdint>

class ST7305 {
  public:
    typedef struct {
        void (*reset_f)(int state);
        void (*cs_f)(int state);
        void (*rs_f)(int state);
        void (*fastwrite_f)(const void* msg, int size);
        void (*delay_us_f)(volatile int nus);
    } cbs_t;

    ST7305(const cbs_t& cbs) : _cbs(cbs)
    {
    }
    void init(void);

    void clear(uint8_t pattern)
    {
      set_window();
      for(int i = 0; i < 100 * 50; i++) {    //192 * 14 * 3; i++) {
        write_data(pattern);
      }
    }
    void write(uint8_t* buf, int size)
    {
      set_window();
      _cbs.rs_f(1);
      _cbs.cs_f(0);
      _cbs.fastwrite_f(buf, size);
      _cbs.cs_f(1);
    }

  private:
    const cbs_t& _cbs;

    inline void write_cmd(uint8_t cmd)
    {
      _cbs.rs_f(0);
      _cbs.cs_f(0);
      _cbs.fastwrite_f(&cmd, 1);
      _cbs.cs_f(1);
    }

    inline void write_data(uint8_t data)
    {
      _cbs.rs_f(1);
      _cbs.cs_f(0);
      _cbs.fastwrite_f(&data, 1);
      _cbs.cs_f(1);
    }

    void set_window(void);
};

#endif
