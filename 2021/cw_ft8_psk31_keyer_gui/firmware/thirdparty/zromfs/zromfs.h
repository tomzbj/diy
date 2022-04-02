#ifndef __ZROMFS_H
#define __ZROMFS_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

// configurations
#define ZR_ENDIAN_LITTLE        // x86, avr, arm
//#define ZR_ENDIAN_BIG         // 51, stm8
#define ZR_MAX_VOLUMNS 2
#define ZR_MAX_OPENED_FILES 2

typedef unsigned char zr_u8_t;
typedef unsigned short zr_u16_t;
typedef unsigned long zr_u32_t;
typedef signed long zr_s32_t;

typedef enum {
    ZR_OK = 0,
    ZR_NO_FILESYSTEM = -1,
    ZR_DISK_ERR = -2,
    ZR_FILE_NOT_FOUND = -3,
    ZR_DIR_NOT_FOUND = -4,
    ZR_NO_FILE = -5,
    ZR_NOT_A_DIR = -6,
    ZR_FILETYPE_NOT_SUPPORTED = -7,
    ZR_FILE_NOT_OPENED = -8,
    ZR_OPENED_FILE_EXCEED = -9,
    ZR_VOLUME_NOT_MOUNTED = -10,
    ZR_VOLUME_NUM_EXCEED = -11
} ZR_RESULT;

enum {
    ZR_FTYPE_HARDLINK,
    ZR_FTYPE_DIR,
    ZR_FTYPE_REGULAR,
    ZR_FTYPE_SYMBOL_LINK,
    ZR_FTYPE_BLOCK_DEV,
    ZR_FTYPE_CHAR_DEV,
    ZR_FTYPE_SOCKET,
    ZR_FTYPE_FIFO
};

typedef struct {
    zr_u32_t start;
    void (*read_f)(zr_u32_t offset, void* buf, zr_u32_t size);
    zr_u32_t size;
} zr_fs_t;

typedef struct {
    zr_u32_t offset;
} zr_dir_t;

typedef struct {
    char fname[16];
    zr_u32_t fsize;
    zr_u32_t spec;
    zr_u32_t offset;
    zr_u32_t next;
    zr_u32_t ftype;
} zr_finfo_t;

int zr_mount(zr_fs_t* fs);                                  // mount a volume
ZR_RESULT zr_select_volume(int volume_id);                  // select current volume
int zr_open(const char* path);                              // open a file
ZR_RESULT zr_close(int fd);                                 // close a opened file
int zr_read(int fd, void* buff, zr_u32_t nbytes);           // read data from a file
ZR_RESULT zr_lseek(int fd, zr_u32_t offset, int seek_opt);  // move current read position to offset
zr_u32_t zr_tell(int fd);                                   // return current read position of fd
ZR_RESULT zr_stat(const char* path, zr_finfo_t* finfo);     // get file status
ZR_RESULT zr_opendir(zr_dir_t* dir, const char* path);      // open a directory
ZR_RESULT zr_readdir(zr_dir_t* dir, zr_finfo_t* finfo);     // read a directory item

#ifdef __cplusplus
}
#endif

#endif 
