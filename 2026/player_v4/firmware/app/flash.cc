#include "misc.h"
#include "platform.h"

FATFS fatfs = {0};
FS fs(fatfs);

void FLASH_Config(void)
{
  fs.init();
  print_log();
}
