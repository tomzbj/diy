/* Author: tomzbj  Updated: 2024-04-18 Rev: 0.02 */

#include "flasheep.h"
#include "misc.h"

#define MAIN_ADDR(x) (_base + (x))
#define EEPROM_SIZE (_page_size / 4)   // 384/768bytes for 1k/2k page
#define MAX_DUMMY_ADDR (EEPROM_SIZE - 2)
#define LOAD_ADDR(x) (*(uint16_t*)MAIN_ADDR(x))
#define LOAD_DATA(x) (*(uint16_t*)MAIN_ADDR((x) + 2))

int FlashEEP::get_first_empty_addr(void)
{
    uint16_t i;
    for(i = 0; i < _page_size; i += 4) {
        if(*(size_t*)MAIN_ADDR(i) == 0xffffffff)
            return i;
    }
    return -1;    // Full
}

void FlashEEP::rotate(void)
{
    uint16_t buf[256];    // 128 for 1k, 256 for 2k, 2k for 16k
    this->read(0, buf, EEPROM_SIZE);

    _cbs.unlock_f();
    _cbs.page_erase_f(_base);
    for(size_t i = 0; i < EEPROM_SIZE; i += 2) {
        auto data = (uint32_t)buf[i / 2];
        data <<= 16;
        _cbs.word_program_f(MAIN_ADDR(i * 2), i | data);
    }
    _cbs.lock_f();
}

void FlashEEP::write_word(uint16_t dummy_addr, uint16_t data)
{
    if(dummy_addr > MAX_DUMMY_ADDR) {
        return;     // do nothing
    }
    int addr = get_first_empty_addr();
    if(addr == -1) {            // Page full, rotate needed
        rotate();
        addr = get_first_empty_addr();
    }
    uint16_t orig_data = read_word(dummy_addr);
    if(orig_data == data)
        return;            // skip writing if data unchanged

    _cbs.unlock_f();
    _cbs.word_program_f(MAIN_ADDR(addr), dummy_addr | (((uint32_t)data) << 16));
    _cbs.lock_f();
}

uint16_t FlashEEP::read_word(uint16_t dummy_addr)
{
    int addr = get_first_empty_addr();
    if(addr == 0 || addr == -1)
        addr = _page_size - 4;    // position to the end
    while(addr >= 0) {
        if(LOAD_ADDR(addr) == dummy_addr)
            return LOAD_DATA(addr);
        addr -= 4;
    }
    return 0xffff;              // Dummy_addr not found
}

void FlashEEP::write(uint16_t addr, void* data, int nbytes)
{
    uint16_t* pdata = (uint16_t*)data;
    if(nbytes % 2 != 0)
        nbytes++;
    while(nbytes > 0) {
        write_word(addr, *pdata);
        addr += 2;
        pdata++;
        nbytes -= 2;
    }
}

void FlashEEP::read(uint16_t addr, void* data, int nbytes)
{
    uint16_t* pdata = (uint16_t*)data;
    addr &= (~1);
    if(nbytes % 2 != 0)
        nbytes++;
    while(nbytes > 0) {
        *pdata = read_word(addr);
        addr += 2;
        pdata++;
        nbytes -= 2;
    }
}

uint16_t FlashEEP::get_size(void)
{
    return EEPROM_SIZE;
}

void FlashEEP::erase_all(void)
{
    _cbs.unlock_f();
    _cbs.page_erase_f(_base);
    _cbs.lock_f();
}
