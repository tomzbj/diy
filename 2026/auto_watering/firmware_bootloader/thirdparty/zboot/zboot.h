#ifndef _ZBOOT_H
#define _ZBOOT_H

#include <cstdint>
#include <memory>

class ZBOOT {
  public:
    constexpr static uint8_t ID = 0x80;

    enum class CMD {
      GET_MAX_APPSIZE = 0x80, READ_FLASH = 0x81, GET_APPSIZE = 0x82,
      READ_APP_CRC32 = 0x83, ERASE_APP = 0x84, WRITE_FLASH = 0x85, CHECK_EMPTY = 0x86,
      JUMP_TO_APP = 0x87, GET_MAX_PACKET_SIZE = 0x88
    };
    struct CBS {
        int (*printf_f)(const char* fmt, ...);
        void (*usart_write_f)(uint8_t* data, int size);
        uint32_t (*crc32_f)(uint32_t crc, const uint8_t* msg, int size);
        void (*reboot_f)(void);
        void (*page_erase_f)(uint32_t addr);
        void (*flash_program_f)(uint32_t addr, const void* data, int size);
    };

    ZBOOT(const CBS& cbs, uint32_t flash_base, uint32_t flash_size, uint32_t page_size,
      uint32_t packet_size);
    void parse_cmd(const char* cmd, int len);
    void parse_binary(const uint8_t* msg, int size);
    void jump_to_app(void);
    bool app_is_valid(void);

  private:

    enum class RESULT {
      OK = 0, ERROR = -1
    };

    const CBS& _cbs;
    struct {
        uint32_t flash_base, app_base;
        uint32_t bootloader_size, flash_size, page_size, max_packet_size, max_app_size;
    } _inf;

    bool check_empty(void* addr, int size);
    void erase(bool echo);
    void uputc(uint8_t c);

    uint32_t crc32(uint32_t crc, const uint8_t* msg, int size);
    RESULT verify(const uint8_t* msg, int size);
    uint32_t get_app_size(void);

};
#endif
