#ifndef _FLASH_H
#define _FLASH_H

void flash_page_erase(uint32_t addr);
void flash_program(uint32_t addr, const void* data, int size);

#endif
