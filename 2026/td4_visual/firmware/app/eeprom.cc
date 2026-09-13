#include "misc.h"
#include "platform.h"
#include "zeep.h"

static void page_erase_f(uint32_t addr)
{
  uint32_t error = 0;
  FLASH_EraseInitTypeDef eis;

  HAL_FLASH_Unlock();
  __disable_irq();
  eis.TypeErase = FLASH_TYPEERASE_PAGES;
  eis.PageAddress = addr;
  eis.NbPages = 1;
  HAL_StatusTypeDef st = HAL_FLASHEx_Erase(&eis, &error);
  __enable_irq();
  HAL_FLASH_Lock();
  if(st != HAL_OK)
    printf("FLASH erase err %d e=%lu @%08lX\n", (int)st, error, addr);
}

/* GD32E230 FMC programs by 32-bit word only. STM32F0 HAL WORD path does two
 * halfword stores and typically fails (PGERR) on this chip — ZEEP then looks
 * like it “saves” but nothing persists. */
static void word_program_f(uint32_t addr, uint32_t data)
{
  HAL_FLASH_Unlock();
  __disable_irq();

  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
  SET_BIT(FLASH->CR, FLASH_CR_PG);
  *(__IO uint32_t*)addr = data;
  while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY))
    ;
  CLEAR_BIT(FLASH->CR, FLASH_CR_PG);

  uint32_t sr = FLASH->SR;
  __enable_irq();
  HAL_FLASH_Lock();

  if(sr & (FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR))
    printf("FLASH prog err SR=%08lX @%08lX\n", sr, addr);
  else if(*(volatile uint32_t*)addr != data)
    printf("FLASH verify fail @%08lX got %08lX want %08lX\n", addr,
      *(volatile uint32_t*)addr, data);
}

static uint32_t word_read_f(uint32_t addr)
{
  return *(volatile uint32_t*)addr;
}

static ZEEP::CBS cbs = {page_erase_f, word_program_f, word_read_f};
/* Last 1KB page; linker FLASH_SIZE=63K leaves this free. */
ZEEP eep(0x0800fc00, 1024, cbs);

void EEPROM_Write(const void* data, int size)
{
  eep.write(0, data, size);
}

void EEPROM_Read(void* data, int size)
{
  eep.read(0, data, size);
}
