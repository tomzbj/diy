/* Author: tomzbj  Updated: 2026-06-03 Rev: 0.04 */
#ifndef _FLASH_EEPROM_H
#define _FLASH_EEPROM_H

#include <cstdint>
#include <cstddef>

class ZEEP {
  public:
    struct CBS {    // all addrs are absolute
        void (*page_erase_f)(uint32_t addr);
        void (*word_program_f)(uint32_t addr, uint32_t data);
        uint32_t (*word_read_f)(uint32_t addr);
    };
    ZEEP(size_t base_addr, size_t page_size, CBS& cbs) :
      _base_addr(base_addr), _page_size(page_size), _cbs(cbs),    //
      EEPROM_SIZE(_page_size / 4), MAX_DUMMY_ADDR(EEPROM_SIZE - 2)    // 384/768bytes for 1k/2k page
    {
    }
    void write_word(uint16_t addr, uint16_t data) const;
    uint16_t read_word(uint16_t addr) const;
    void write(uint16_t addr, const void* data, int nbytes) const;
    void read(uint16_t addr, void* data, int nbytes) const;
    uint16_t get_size(void) const;
    void erase_all(void) const;

  private:

    const size_t _base_addr, _page_size;
    const CBS& _cbs;
    const uint16_t EEPROM_SIZE, MAX_DUMMY_ADDR;

    int get_first_empty_addr(void) const;
    void rotate(void) const;
};

#endif
