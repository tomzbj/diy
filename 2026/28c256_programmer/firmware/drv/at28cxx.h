#ifndef _AT28CXX_H
#define _AT28CXX_H

#include <cstdint>

class AT28CXX {
  public:
    struct CBS {
        void (*setoe_f)(int s);
        void (*setwe_f)(int s);
        void (*setce_f)(int s);
        uint8_t (*read_data_f)(void);
        void (*write_data_f)(uint8_t data);
        void (*write_addr_f)(uint32_t addr);
        void (*delay_us_f)(volatile int nus);
    };

    AT28CXX(CBS& cbs) :
      _cbs(cbs)
    {
    }
    void init(void)
    {
      _cbs.setce_f(1);
      _cbs.setoe_f(1);
      _cbs.setwe_f(1);
    }

    void write(uint32_t addr, const uint8_t* data, int count);
    void read(uint32_t addr, uint8_t* data, int count);

  private:
    CBS& _cbs;

    uint8_t read_byte(uint32_t addr);
    void write_byte_raw(uint32_t addr, uint8_t data);
    void unlock_sdp(void);
    void wait_ready(uint32_t addr, uint8_t expect_data);
};

#endif
