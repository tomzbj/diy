#include "flash_eeprom.h"
#include "misc.h"

// flash ops for gd32
#define FLASH_Unlock() fmc_unlock()
#define FLASH_Lock() fmc_lock()
#define FLASH_ErasePage(addr) fmc_page_erase(addr)
#define FLASH_ProgramHalfWord(addr, data) fmc_halfword_program(addr, data)
#define FLASH_ProgramWord(addr, data) fmc_word_program(addr, data)
#define FLASH_ClearFlag(flag) fmc_flag_clear(flag)
#define FLASH_FLAG_PGERR FMC_FLAG_PGERR

#define MAIN_ADDR(x) (_base + (x))
#define EEPROM_SIZE (_page_size / 4)   // 384/768bytes for 1k/2k page
#define MAX_DUMMY_ADDR (EEPROM_SIZE - 2)
#define LOAD_ADDR(x) (*(unsigned short*)MAIN_ADDR(x))
#define LOAD_DATA(x) (*(unsigned short*)MAIN_ADDR((x) + 2))

int flash_eeprom::get_first_empty_addr(void)
{
    unsigned short i;
    for(i = 0; i < _page_size; i += 4) {
        if(*(size_t*)MAIN_ADDR(i) == 0xffffffff)
            return i;
    }
    return -1;    // Full
}

void flash_eeprom::rotate(void)
{
    unsigned short buf[256];    // 128 for 1k, 256 for 2k, 2k for 16k
    this->read(0, buf, EEPROM_SIZE);

    FLASH_Unlock();
    FLASH_ErasePage(_base);
    for(size_t i = 0; i < EEPROM_SIZE; i += 2) {
//        FLASH_ProgramHalfWord(MAIN_ADDR(i * 2), i);
//        FLASH_ProgramHalfWord(MAIN_ADDR(i * 2) + 2, buf[i / 2]);
        unsigned long data = (unsigned long)buf[i / 2];
        data <<= 16;
        FLASH_ProgramWord(MAIN_ADDR(i*2), i | data);
    }
    FLASH_Lock();
}

void flash_eeprom::write_word(unsigned short dummy_addr, unsigned short data)
{
    if(dummy_addr > MAX_DUMMY_ADDR) {
        return;     // do nothing
    }
    int addr = get_first_empty_addr();
    if(addr == -1) {            // Page full, rotate needed
        rotate();
        addr = get_first_empty_addr();
    }
    unsigned orig_data = read_word(dummy_addr);
    if(orig_data == data)
        return;            // skip writing if data unchanged

    FLASH_Unlock();
//    FLASH_ProgramHalfWord(MAIN_ADDR(addr), dummy_addr);
//    FLASH_ProgramHalfWord(MAIN_ADDR(addr) + 2, data);
    FLASH_ProgramWord(MAIN_ADDR(addr),
        dummy_addr | (((unsigned long )data) << 16));
    FLASH_Lock();
}

unsigned short flash_eeprom::read_word(unsigned short dummy_addr)
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

void flash_eeprom::write(unsigned short addr, void* data, int nbytes)
{
    unsigned short* pdata = (unsigned short*)data;
    if(nbytes % 2 != 0)
        nbytes++;
    while(nbytes > 0) {
        write_word(addr, *pdata);
        addr += 2;
        pdata++;
        nbytes -= 2;
    }
}

void flash_eeprom::read(unsigned short addr, void* data, int nbytes)
{
    unsigned short* pdata = (unsigned short*)data;
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

unsigned short flash_eeprom::get_size(void)
{
    return EEPROM_SIZE;
}

flash_eeprom::flash_eeprom(size_t base_addr, size_t page_size)
{
    _base = base_addr;
    _page_size = page_size;
}

void flash_eeprom::erase_all(void)
{
    FLASH_Unlock();
    FLASH_ErasePage(_base);
    FLASH_Lock();
}
