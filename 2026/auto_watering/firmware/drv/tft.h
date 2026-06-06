/* Author: tomzbj  Updated: 2024-04-13 Rev01 */

#ifndef _TFT_H
#define _TFT_H

#include <cstdint>

class TFT {
  public:

    typedef struct {
        void (*reset_f)(int state);
        void (*cs_f)(int state);
        void (*rs_f)(int state);
        void* fastwrite_f;
        void (*delay_us_f)(volatile int nus);
    } cbs_t;

    enum {
      DATAWIDTH_8, DATAWIDTH_16
    };

    TFT(int width, int height, int data_width, cbs_t cbs)
    {
      _width = width;
      _height = height;
      _cbs = cbs;
      _data_width = data_width;
      _fastwrite8_f = (typeof(_fastwrite8_f))_cbs.fastwrite_f;
      _fastwrite16_f = (typeof(_fastwrite16_f))_cbs.fastwrite_f;
    }

    virtual void init(void)=0;
    virtual void set_window(int x1, int y1, int x2, int y2)=0;

    virtual void clear(uint16_t color)
    {
      uint16_t buf[1024];
      for(int i = 0; i < 1024; i++)
        buf[i] = color;
      set_window(0, 0, _width - 1, _height - 1);
      _cbs.cs_f(0);
      _cbs.rs_f(1);
      int n = _width * _height;
      int batch_size = sizeof(buf) / sizeof(buf[0]);
      while(n > 0) {
        fastwrite(buf, batch_size);
        n -= batch_size;
        if(n < batch_size)
          batch_size = n;
      }
      _cbs.cs_f(1);
    }
    virtual void write(uint16_t* buf, int count)
    {
      _cbs.cs_f(0);
      _cbs.rs_f(1);
      while(count > 0) {
        int batch_size = 65500;
        if(count < batch_size)
          batch_size = count;
        fastwrite(buf, batch_size);
        count -= batch_size;
      }
      _cbs.cs_f(1);
    }

  protected:
    cbs_t _cbs;
    int _width, _height, _data_width;

    void (*_fastwrite8_f)(uint8_t* buf, int count);
    void (*_fastwrite16_f)(uint16_t* buf, int count);

    void fastwrite(uint16_t* buf, int count)
    {
      if(_data_width == DATAWIDTH_8)
        _fastwrite8_f((uint8_t*)buf, count * 2);
      else
        _fastwrite16_f(buf, count);
    }
    void write_cmd(uint16_t cmd)
    {
      _cbs.cs_f(0);
      _cbs.rs_f(0);

      if(_data_width == DATAWIDTH_8)
        _fastwrite8_f((uint8_t*)&cmd, 1);
      else
        _fastwrite16_f(&cmd, 1);
      _cbs.cs_f(1);
    }
    virtual void write_data8(uint8_t data)
    {
      _cbs.cs_f(0);
      _cbs.rs_f(1);
      _fastwrite8_f(&data, 1);
      _cbs.cs_f(1);
    }
    virtual void write_data16(uint16_t data)
    {
      _cbs.cs_f(0);
      _cbs.rs_f(1);
      _fastwrite16_f(&data, 1);
      _cbs.cs_f(1);
    }
    virtual void write_reg(uint8_t reg, uint16_t data)
    {
      write_cmd(reg);
      if(_data_width == DATAWIDTH_8)
        write_data8(data);
      else
        write_data16(data);
    }

};

#endif
