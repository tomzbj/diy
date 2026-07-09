#ifndef _FS_H
#define _FS_H

#include "ff.h"
#include <cstdint>

class FS {
  public:
    FS(FATFS& fs) :
      _fs(fs)
    {
    }
    void init(void);

    // command line operations
    void cmd_ls(const char* tokens[]);
    void cmd_fsinfo(const char* tokens[]);
    void cmd_info(const char* tokens[]);
    void cmd_cat(const char* tokens[]);
    void cmd_touch(const char* tokens[]);
    void cmd_hexview(const char* tokens[]);
    void cmd_crc32(const char* tokens[]);
    //    void cmd_playwav(const char* tokens[]);
    void cmd_cd(const char* tokens[]);
    void cmd_pwd(const char* tokens[]);
    void cmd_rm(const char* tokens[]);
    void cmd_rename(const char* tokens[]);

    const char* pwd(void)
    {
      return _pwd;
    }

  private:
    int readfile_to_stdio(const char* fname, uint32_t pos, uint32_t size);
    int readfile(const char* fname, void* buf, uint32_t pos, uint32_t size);
    int filesize(const char* fname);
    bool file_exist(const char* fname);

    int find_match_fname(char* fname);
    uint32_t crc32(const char* fname);
    void update_boot_count(void);

//  public:
    FATFS& _fs;
    char _pwd[256];
    char _genbuf[1024];
};

#endif
