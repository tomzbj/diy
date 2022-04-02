#include "misc.h"
#include "platform.h"

static int read_f(unsigned long addr, int size, void* buf)
{
    const sfud_flash* flash = sfud_get_device_table() + 0;
    sfud_read(flash, addr, size, buf);
    return 0;
}
static int write_f(unsigned long addr, int size, const void* buf)
{
    const sfud_flash* flash = sfud_get_device_table() + 0;
    sfud_write(flash, addr, size, buf);
    return 0;
}
static int erase_f(unsigned long addr, int size)
{
    const sfud_flash* flash = sfud_get_device_table() + 0;
//    printf("%08x %d\n", addr, size);
    sfud_erase(flash, addr, size);
    return 0;
}
static unsigned long readinfo_f(void)
{
    const sfud_flash* flash = sfud_get_device_table() + 0;
    return flash->chip.mf_id * 65536UL + flash->chip.type_id * 256UL
        + flash->chip.capacity_id;
}

static void uwrite_f(const void* data, int size)
{
    zu_writedata((void*)data, size, 0);
}

void FLASH_Config(void)
{
    {
        sfud_init();
//    sfud_qspi_fast_read_enable(sfud_get_device(SFUD_XXX_DEVICE_INDEX), 4);
//    const sfud_flash* flash = sfud_get_device_table() + 0;
//    printf("%02x %02x %02x\n", flash->chip.mf_id, flash->chip.type_id, flash->chip.capacity_id);
        printf("[%s: %d] SFUD Initialized.\n", __FILE__, __LINE__);
    }
    {
        static fl_cfg_t fl;
        fl.read_f = read_f;
        fl.write_f = write_f;
        fl.erase_f = erase_f;
        fl.readinfo_f = readinfo_f;
        fl.crc32_f = crc32;
//    fc.crc32_f = NULL;
        fl.uwrite_f = uwrite_f;
        fl_init(&fl);
        printf("[%s: %d] FLASHER Initialized.\n", __FILE__, __LINE__);
    }
    {
        DataConfig_t* pdc = DC_Get();
        pdc->tar.start_addr = 0;
        pdc->tar.size = 969728UL;
        pdc->tar.read_f = read_f;

        printf("[%s: %d] ZTAR Initialized.\n", __FILE__, __LINE__);
    }
}
