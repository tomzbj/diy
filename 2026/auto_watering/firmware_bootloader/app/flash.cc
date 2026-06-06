#include "misc.h"

void flash_page_erase(uint32_t addr)
{
  FLASH_EraseInitTypeDef eis;
  uint32_t error;

  HAL_FLASH_Unlock();
  eis.TypeErase = FLASH_TYPEERASE_PAGES;
  eis.PageAddress = addr;
  eis.NbPages = 1;
  if(HAL_FLASHEx_Erase(&eis, &error) != HAL_OK)
    while(1);
  HAL_FLASH_Lock();
}

void flash_program(uint32_t addr, const void* data, int size)
{
  uint8_t* pdata = (uint8_t*)data;
  HAL_FLASH_Unlock();
  for(int i = 0; i < size; i += 2) {
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr + i, *(uint16_t*)&pdata[i]);
  }
  HAL_FLASH_Lock();
}

