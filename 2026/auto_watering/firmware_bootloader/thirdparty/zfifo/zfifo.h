/* Author: tomzbj  Updated: 2026-04-03  Rev: 0.02 */

#ifndef _FIFO_H
#define _FIFO_H

#include <cstdint>
#include <cstring>
#include <memory>

class ZFIFO {
  public:
    ZFIFO(uint32_t size)
    {
      if((size & (size - 1)) != 0)
        size = 0x80000000 >> (__builtin_clz(size) - 1);
      _buf = std::make_unique<uint8_t[]>(size);
      _size = size;
      _in = _out = 0;
    }

    void clear(void)
    {
      _out = _in;
    }
    uint8_t peek(void)
    {
      return _buf[_out & (_size - 1)];
    }

    void peek(void* data, int ofs, int size)
    {
      uint8_t* pdata = (uint8_t*)data;
      for(int i = 0; i < size; i++) {
        pdata[i] = _buf[(_out + ofs + i) & (_size - 1)];
      }
    }

    void peek(void* data, int size)
    {
      peek(data, 0, size);
    }

    void traverse(int (*printf_f)(const char* args, ...))
    {
      uint32_t size = _in - _out;
      for(uint32_t i = 0; i < size; i++) {
        printf_f("%c", _buf[((_out + i) & (_size - 1))]);
      }
      printf_f("\n");
      printf_f("in: %d out: %d free: %d\n", _in, _out, _size - _in + _out);
    }

    int put(const void* pdata, uint32_t len)
    {
      uint32_t l;
      len = min(len, _size - _in + _out);
      l = min(len, _size - (_in & (_size - 1)));
      memcpy(_buf.get() + (_in & (_size - 1)), pdata, l);
      memcpy(_buf.get(), (uint8_t*)pdata + l, len - l);
      _in += len;

      return len;
    }
    int get(void* pdata, uint32_t len)
    {
      uint32_t l;
      len = min(len, _in - _out);
      l = min(len, _size - (_out & (_size - 1)));
      memcpy(pdata, _buf.get() + (_out & (_size - 1)), l);
      memcpy((uint8_t*)pdata + l, _buf.get(), len - l);
      _out += len;
      return len;
    }
    uint32_t len(void)
    {
      return _in - _out;
    }

//  private:
  public:
    std::unique_ptr<uint8_t[]> _buf;
    uint32_t _size, _in, _out;

    template<typename T>
    inline T min(T x, T y)
    {
      return (x <= y) ? x : y;
    }
};

#endif
