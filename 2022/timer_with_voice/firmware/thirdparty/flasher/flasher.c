#include <stdlib.h>
#include <string.h>
#include "flasher.h"
#include "misc.h"   // debug

#include <time.h>

static fl_cfg_t g;

#define MAX_PACK_SIZE 1280

static unsigned long crc32_soft(unsigned long crc, const void* msg, int size)
{
    const unsigned long polynormial = 0xedb88320;
    for(int i = 0; i < size; i++) {
        crc ^= *(unsigned char*)(msg + i);
        for(int j = 0; j < 8; j++) {
            int lsb = crc % 2;
            crc >>= 1;
            if(lsb)
                crc ^= polynormial;
        }
    }
    return crc;
}

void fl_init(fl_cfg_t* cfg)
{
    memcpy(&g, cfg, sizeof(fl_cfg_t));
    if(g.crc32_f == NULL) {
        g.crc32_f = crc32_soft;
    }
}

static void ReadImage(unsigned long pos, int size)
{
    unsigned char buf[MAX_PACK_SIZE];
    if(size > MAX_PACK_SIZE)
        size = MAX_PACK_SIZE;

    g.read_f(pos, size, buf);
    g.uwrite_f(buf, size);
}

static void ReadCRC32(unsigned long pos, int total_size)
{
    unsigned char buf[MAX_PACK_SIZE];
    int size = MAX_PACK_SIZE;
    unsigned long crc;

    crc = 0xffffffff;
    while(total_size > 0) {
        if(total_size < size)
            size = total_size;
        g.read_f(pos, size, buf);
        crc = g.crc32_f(crc, buf, size);
        pos += size;
        total_size -= size;
    }
    crc = ~crc;
    g.uwrite_f(&crc, 4);
}

// Make sure the flash area was erased!
static void WriteImage(unsigned long pos, unsigned short size,
    const void* content)
{
    g.write_f(pos, size, (void*)content);
    g.uwrite_f(0, 1);
}

static void EraseImage(unsigned long pos, unsigned short size)
{
    pos &= 0xfffff000;
    g.erase_f(pos, size);
    g.uwrite_f(0, 1);
}

static void CheckBlockEmpty(unsigned long pos)
{
    unsigned char buf[4096];

    pos &= 0xfffff000;    // position to 4k block border
    int size = 4096;
    unsigned char ret;

    g.read_f(pos, size, buf);
    for(int i = 0; i < size; i += 4) {
        if(*(unsigned long*)(buf + i) != 0xffffffff)
            goto end;
    }
    ret = 1;
    g.uwrite_f(&ret, 1);
    return;

    end:
    ret = 0;
    g.uwrite_f(&ret, 1);
}

void fl_parse(const void* pmsg, int msg_size)
{
    unsigned char* msg = (unsigned char*)pmsg;
    unsigned char checksum = 0;
    const unsigned char* content;
    unsigned long pos;
    unsigned long size;

    if(msg_size < 2)
        return;
    for(int i = 0; i < msg_size; i++) {
        checksum += msg[i];
    }

    if(checksum != 0)
        return;

    switch(msg[0]) {            // func code
        case 0x90:              // get flash info
            pos = g.readinfo_f();
            g.uwrite_f(&pos, 4);
            break;
        case 0x91:              // check whether given block is empty
            pos = *(unsigned long*)(&msg[4]);
            CheckBlockEmpty(pos);
            break;
        case 0x92:
            size = *(unsigned long*)(&msg[4]);
            pos = *(unsigned long*)(&msg[8]);
            ReadCRC32(pos, size);
            break;
        case 0x9c:              // read full image
            size = *(unsigned short*)(&msg[2]);
            pos = *(unsigned long*)(&msg[4]);
            ReadImage(pos, size);
            break;
        case 0x9d:              // erase flash block
            size = *(unsigned short*)(&msg[2]);
            pos = *(unsigned long*)(&msg[4]);
            EraseImage(pos, size);
            break;
        case 0x9e:              // write image
            size = *(unsigned short*)(&msg[2]);
            pos = *(unsigned long*)(&msg[4]);
            content = &msg[8];
            WriteImage(pos, size, content);
            break;
    }
}

static void get_inf_block(int pos, void* block, int* available)
{
    unsigned long capacity = 1UL << (g.readinfo_f() & 0xff);
    if(pos >= 64)
        pos = 0;
    unsigned long addr = (capacity - 4096) + 64 * pos;
    g.read_f(addr, 64, block);
    unsigned char ff[64];
    memset(ff, 0xff, 64);
    *available = 1;
    if(*(unsigned char*)block == 0)
        *available = 0;
    if(memcmp(block, ff, 64) == 0)
        *available = 0;
}

int fl_get_file_num(void)
{
    int num = 0;
    for(int i = 0; i < 64; i++) {
        unsigned char buf[64];
        int available;
        get_inf_block(i, buf, &available);
        if(available)
            num++;
    }
    return num;
}

fl_finfo_t* fl_get_file_info(int id)
{
    int num = 0;
    static fl_finfo_t inf;
    for(int i = 0; i < 64; i++) {
        unsigned char buf[64];
        int available;
        get_inf_block(i, buf, &available);
        if(available) {
            if(id == num) {
                memcpy(&inf.fname, buf, 40);
                for(int j = 39; j > 0; j--)
                    if(inf.fname[j] == 0xff)
                        inf.fname[j] = 0;
                memcpy(&inf.addr, buf + 40, 4);
                memcpy(&inf.size, buf + 44, 4);
                memcpy(&inf.timestamp, buf + 48, 4);
                memcpy(&inf.crc32, buf + 52, 4);
                break;
            }
            num++;
        }
    }
    return &inf;
}
