#include "misc.h"
#include "platform.h"
#include "zeep.h"

static void page_erase_f(uint32_t addr)
{
  uint32_t error = 0;
  FLASH_EraseInitTypeDef eis;

  HAL_FLASH_Unlock();
  eis.TypeErase = FLASH_TYPEERASE_PAGES;
  eis.PageAddress = addr;
  eis.NbPages = 1;
  if(HAL_FLASHEx_Erase(&eis, &error) != HAL_OK)
    while(1);
  HAL_FLASH_Lock();
}

static void word_program_f(uint32_t addr, uint32_t data)
{
  HAL_FLASH_Unlock();
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, data);
  HAL_FLASH_Lock();
}

static uint32_t word_read_f(uint32_t addr)
{
  return *(uint32_t*)addr;
}

static ZEEP::CBS cbs = {page_erase_f, word_program_f, word_read_f};
ZEEP eep(0x0807f800, 2048, cbs);

void EEPROM_Write(const void* data, int size)
{
  eep.write(0, data, size);
}

void EEPROM_Read(void* data, int size)
{
  eep.read(0, data, size);
}
