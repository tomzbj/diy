#ifndef _CHARLIE_H
#define _CHARLIE_H

#include <cstdint>

class CHARLIE {
  public:
    void init(void);
    void view(void);
    void poll(void);
    void write(uint8_t* data, int count)
    {
      if(count > 20)
        count = 20;
      for(int i = 0; i < count; i++) {
        _vram[i] = data[i];
      }
    }

  private:

    void set_dir(uint32_t port, uint16_t pin, int dir);
    enum {
      IN, L, H
    };
    uint8_t _vram[20];
    uint8_t _array[140];

    uint16_t pin[15];
    uint32_t port[15];
};
#endif
