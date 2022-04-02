#include "platform.h"
#include "ztar.h"

typedef struct {
    char name[100];
    char reserved_1[24];
    char size[12];
    char reserved_2[376];
} tar_header_t;

//void ztar_init(ztar_obj_t* cfg) { g = *cfg; }

int ztar_find(const ztar_obj_t* pobj, const char* fname, unsigned long* paddr,
    int* psize)
{
    tar_header_t header;
    unsigned long addr = pobj->start_addr, size;
    while(addr - pobj->start_addr < pobj->size) {
        pobj->read_f(addr, sizeof(header), &header);
        addr += sizeof(header);
        if(strlen(header.name) == 0)
            return -1;      // file 'fname' not found
        sscanf(header.size, "%lo", &size);
        if(strncmp(header.name, fname, sizeof(header.name)) == 0) {
            *paddr = addr;
            *psize = size;
            return 0;
        }
        if(size % 512 != 0)
            size += (512 - (size % 512));
        addr += size;
    }
    return -1;
}
