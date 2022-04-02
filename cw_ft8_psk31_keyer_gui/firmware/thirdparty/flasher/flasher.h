#ifndef _FLASHER_H
#define _FLASHER_H

typedef struct {
    int (*read_f)(unsigned long addr, int size, void* buf);
    int (*write_f)(unsigned long addr, int size, const void* buf);
    int (*erase_f)(unsigned long addr, int size);
    unsigned long (*readinfo_f)(void);
    void (*uwrite_f)(const void* data, int size);
    unsigned long (*crc32_f)(unsigned long initial, const void* data, int size);
} fl_cfg_t;

typedef struct {
    unsigned char fname[40];
    unsigned long addr, size, timestamp, crc32;
} fl_finfo_t;

void fl_init(fl_cfg_t* cfg);
// called by usart
void fl_parse(const void* msg, int size);

int fl_get_file_num(void);
fl_finfo_t* fl_get_file_info(int id);

#endif
