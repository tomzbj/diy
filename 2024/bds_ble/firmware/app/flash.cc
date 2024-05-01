#include "misc.h"
#include "platform.h"

static void page_erase_f(uint32_t addr);
static void word_program_f(uint32_t addr, uint32_t data);
static void unlock_f(void);
static void lock_f(void);

const FlashEEP::cb_t cbs3 = {unlock_f, lock_f, page_erase_f, word_program_f};
FlashEEP eep(0x0801e000, 2048, cbs3);
DataConfig dc(eep);

static void page_erase_f(uint32_t addr)
{
    FLASH_EraseInitTypeDef feit;
    feit.TypeErase = FLASH_TYPEERASE_PAGES;
    feit.PageAddress = addr;
    feit.NbPages = 1;
    uint32_t error;
    HAL_FLASHEx_Erase(&feit, &error);
}

static void word_program_f(uint32_t addr, uint32_t data)
{
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, data);
}

static void lock_f(void)
{
    HAL_FLASH_Lock();
}
static void unlock_f(void)
{
    HAL_FLASH_Unlock();
}

void FLASH_Config(void)
{
}
