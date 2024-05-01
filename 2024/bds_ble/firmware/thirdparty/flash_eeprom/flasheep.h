/* Author: tomzbj  Updated: 2024-04-18 Rev: 0.02 */

#ifndef _FLASH_EEPROM_H
#define _FLASH_EEPROM_H

#include <cstdint>
#include <cstddef>

class FlashEEP {
public:
    typedef struct {
        void (*unlock_f)(void);
        void (*lock_f)(void);
        void (*page_erase_f)(uint32_t addr);
        void (*word_program_f)(uint32_t addr, uint32_t data);
    } cb_t;
    FlashEEP(size_t base_addr, size_t page_size, cb_t cbs)     //
    : _base(base_addr), _page_size(page_size), _cbs(cbs)
    {
    }
    void write_word(uint16_t addr, uint16_t data);
    uint16_t read_word(uint16_t addr);
    void write(uint16_t addr, void* data, int nbytes);
    void read(uint16_t addr, void* data, int nbytes);
    uint16_t get_size(void);
    void erase_all(void);

private:
    // private funcs
    int get_first_empty_addr(void);
    void rotate(void);

    // data members
    size_t _base, _page_size;
    const cb_t& _cbs;
};

#endif
