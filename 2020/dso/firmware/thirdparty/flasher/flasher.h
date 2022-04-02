#ifndef _FLASHER_H
#define _FLASHER_H

typedef int (*fl_read_t)(unsigned long addr, int size, void* buf);
typedef int (*fl_write_t)(unsigned long addr, int size, const void* buf);
typedef int (*fl_erase_t)(unsigned long addr, int size);
typedef unsigned long (*fl_readinfo_t)(void);
typedef int (*fl_uartwrite_t)(const void* data, int size);
typedef unsigned long (*fl_crc32_t)(unsigned long initial, const void* data, int size);
typedef struct {
    fl_read_t read_f;
    fl_write_t write_f;
    fl_erase_t erase_f;
    fl_readinfo_t readinfo_f;
    fl_uartwrite_t uwrite_f;
    fl_crc32_t crc32_f;
} fl_config_t;

void fl_init(fl_config_t* cfg);
void fl_parse(const void* msg, int size);

#endif
