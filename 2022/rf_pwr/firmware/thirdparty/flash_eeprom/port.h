#ifndef _PORT_H
#define _PORT_H

#include "misc.h"

#define FLASH_Unlock() fmc_unlock()
#define FLASH_Lock() fmc_lock()
#define FLASH_ErasePage(addr) fmc_page_erase(addr)
#define FLASH_ProgramHalfWord(addr, data) fmc_halfword_program(addr, data)
#define FLASH_ProgramWord(addr, data) fmc_word_program(addr, data)
#define FLASH_ClearFlag(flag) fmc_flag_clear(flag)
#define FLASH_FLAG_PGERR FMC_FLAG_PGERR

#endif
