#ifndef _FLASH_EEPROM_H
#define _FLASH_EEPROM_H

#include <stddef.h>

class FlashEEP {
public:
    FlashEEP(size_t base_addr, size_t page_size);
    void write_word(unsigned short addr, unsigned short data);
    unsigned short read_word(unsigned short addr);
    void write(unsigned short addr, void* data, int nbytes);
    void read(unsigned short addr, void* data, int nbytes);
    unsigned short get_size(void);
    void erase_all(void);

private:
    // private funcs
    int get_first_empty_addr(void);
    void rotate(void);

    // data members
    size_t _base, _page_size;
};

#endif
