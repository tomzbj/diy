#ifndef _ROM_H
#define _ROM_H

#include <cstdint>

class ROM {
  public:
    struct CBS {
        void (*init_f)(void);
        uint8_t (*addr_in_f)(void);
        void (*data_out_f)(uint8_t _data);
        int (*printf_f)(const char* fmt, ...);
        void (*load_f)(uint8_t* data, int size);
        void (*save_f)(const uint8_t* data, int size);
    };

    ROM(const CBS& cbs);
    void init(void);
    void update(void);
    void save(void);
    void load(void);
    void read_instructions(uint8_t* ins, int count);
    void write_instructions(const uint8_t* ins, int count);

  private:

    const CBS& _cbs;
    uint8_t _data[16];
};
#endif
