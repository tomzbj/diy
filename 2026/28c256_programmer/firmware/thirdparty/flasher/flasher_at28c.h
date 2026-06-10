#ifndef _FLASHER_H
#define _FLASHER_H

#include <cstdint>
#include <cstdlib>

class FLASHER_AT28C {
  public:
    static constexpr uint8_t ID = 0xa0;
    enum class CMD {
      READ_CRC32 = 0xa2, READ = 0xac, WRITE = 0xae, ERASE_CHIP = 0xaf,
      GET_ROM_SIZE = 0xa6, GET_MAX_PACKET_SIZE = 0xa9
    };

    struct CBS {
        int (*read_f)(uint32_t addr, int size, void* buf);
        int (*write_f)(uint32_t addr, int size, const void* buf);
        void (*usart_write_f)(const void* data, int size);
//        uint32_t (*crc32_f)(uint32_t initial, const void* data, int size);
    };

    FLASHER_AT28C(CBS& cbs, uint16_t rom_size, uint16_t max_packet_size) :
      _cbs(cbs), _rom_size(rom_size), _max_packet_size(max_packet_size)
    {
    }
    void parse_binary(const uint8_t* msg, int size);

  private:
    enum RESULT {
      OK = 0, ERROR = -1
    };

    CBS& _cbs;
    const uint16_t _rom_size, _max_packet_size;
    static uint32_t crc32_soft(uint32_t crc, const void* msg, int size);
    void read(uint16_t pos, int size);
    void read_crc32(uint16_t pos, int total_size);
    void erase_chip(void);
    void uputc(uint8_t c);
    void write(uint16_t pos, uint16_t size, const void* content);
    RESULT verify(const uint8_t* msg, int size);
    uint32_t crc32(uint32_t crc, const uint8_t* msg, int size);

};

#endif
