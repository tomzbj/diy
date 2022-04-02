#ifndef _ZTAR_H
#define _ZTAR_H

typedef struct {
    unsigned long start_addr, size;
    int (*read_f)(unsigned long addr, int size, void* buf);
} ztar_obj_t;

int ztar_find(const ztar_obj_t* pobj, const char* fname, unsigned long* paddr,
    int* psize);

#endif
