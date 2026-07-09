#include "misc.h"
#include "platform.h"
#include "fs.h"
#include "fs_lock.h"
#include <cctype>
#include "zcrc32.h"
#include <memory>

#define ok() printf("ok\n")

#define chkret(expr) ((expr == 0) ? chk_ok() : chk_failed(expr, (char *)__FILE__, __LINE__))
#define chk_failed(x, file, line) ({printf("Err %d %s line %d\n", x, file, line); return; })
#define chk_ok() (void)0

void FS::cmd_info(const char* tokens[])
{
  (void)tokens;
  DWORD fre_clst = 0;
  FATFS* fs = nullptr;
  const FRESULT res = fs_getfree_safe("/", &fre_clst, &fs);
  if(res != FR_OK || fs == NULL) {
    printf("f_getfree failed: %d\n\n", res);
    return;
  }

  const int tot_k = (int)((fs->n_fatent - 2) * fs->csize / 2);
  const int fre_k = (int)(fre_clst * fs->csize / 2);

  printf("f_getfree ret=%d fre_clst=%lu cached=%lu n_fatent=%lu csize=%u\n",
    res, fre_clst, fs->free_clst, (unsigned long)fs->n_fatent, fs->csize);
  printf("%10d K total space.\n%10d K available.\n\n", tot_k, fre_k);
}

void FS::cmd_rename(const char* tokens[])
{
  FS_LOCK lock;
  const char* old_fname = tokens[1];
  const char* new_fname = tokens[2];
  char tmp[258] = {0};

  sprintf(_genbuf, "%s/%s", _pwd, old_fname);
  sprintf(tmp, "%s/%s", _pwd, new_fname);

  if(f_rename(_genbuf, tmp) != FR_OK)
    printf("Failed to rename %s to %s.\n\n", old_fname, new_fname);
}

void FS::cmd_rm(const char* tokens[])
{
  FS_LOCK lock;
  const char* fname = tokens[1];
  sprintf(_genbuf, "%s/%s", _pwd, fname);

  if(f_unlink(_genbuf) != FR_OK)
    printf("Failed to delete %s.\n\n", fname);
}

static int check_fname(const char* fname)
{
  if(strlen(fname) < 1) {
    printf("Filename too short.\n");
    return -1;
  }
  else if(strlen(fname) > 16) {
    printf("Filename too long.\n");
    return -2;
  }
  for(auto i = 0UL; i < strlen(fname); i++) {
    int c = fname[i];
    if(!isalpha(c) && !isdigit(c) && (c != '_') && (c != '-') && (c != '.')) {
      printf("Filename should contain only alphanumeric characters "
        "or dots or underscores.\n");
      return -3;
    }
  }
  return 0;
}

void FS::cmd_touch(const char* tokens[])
{
  FS_LOCK lock;
  const char* fname = tokens[1];
  FIL fp;
  chkret(check_fname(fname));
  sprintf(_genbuf, "%s/%s", _pwd, fname);
  if(f_open(&fp, _genbuf, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
    printf("Failed to create or overwrite %s.\n\n", fname);
  else
    f_close(&fp);    // 关闭文件
}

/* void FS::cmd_fsinfo(const char* tokens[])
 {
 lfs_fsinfo fsinfo;
 chkret_void(LFS_ERR_OK, lfs_fs_stat(&_lfs, &fsinfo));
 printf("%12s%12s%12s%12s%12s%s\n", "DISK_VER", "BLOCK_SIZE", "BLOCK_COUNT",
 "FNAME_MAX", "FILESIZE_MAX", "CUSTOM_ATTR_MAX");
 printf("%12lu%12lu%12lu%12lu%12lu%12lu\n", fsinfo.disk_version,
 fsinfo.block_size, fsinfo.block_count, fsinfo.name_max, fsinfo.file_max,
 fsinfo.attr_max);
 }*/

int FS::find_match_fname(char* fname)
{
  FS_LOCK lock;
  DIR dir;
  FILINFO dir_info;
  int err = f_opendir(&dir, _pwd);
  if(err != FR_OK)
    return 0;
  int tot_size = 0, n = 512;
  while(f_readdir(&dir, &dir_info) == FR_OK && n--) {
    tot_size += dir_info.fsize;
    //    printf("%s\t", dir_info.name);
    if(strcasecmp(fname, dir_info.fname) == 0) {
      strcpy(fname, dir_info.fname);
      f_closedir(&dir);
      return 1;    // filename matched
    }
  }
  f_closedir(&dir);
  return 0;
}

void FS::cmd_pwd(const char* tokens[])
{
  printf("%s\n", _pwd);
}

void FS::cmd_cd(const char* tokens[])
{
  FS_LOCK lock;
  DIR dir = {0};

  if(strcmp(tokens[1], "..") == 0) {
    if(strcmp(_pwd, "/") == 0)
      return;
    else {
      do {
        _pwd[strlen(_pwd) - 1] = '\0';
      } while(_pwd[strlen(_pwd) - 1] != '/');
    }
  }
  else if(strcmp(tokens[1], "/") == 0)
    strcpy(_pwd, "/");
  else {
    sprintf(_genbuf, "%s/%s", _pwd, tokens[1]);
    auto ret = f_opendir(&dir, _genbuf);
    if(ret == FR_OK) {
      if(_pwd[strlen(_pwd) - 1] != '/')
        strcat(_pwd, "/");
      strcat(_pwd, tokens[1]);
    }
    else if(ret == FR_NO_PATH)
      printf("%s: Not a directory.\n\n", tokens[1]);
    else if(ret == FR_INVALID_OBJECT)
      printf("%s: No such file or directory.\n\n", tokens[1]);
  }
  cmd_pwd(tokens);
}

void FS::cmd_ls(const char* tokens[])
{
  FS_LOCK lock;
  DIR dir = {0};
  FILINFO info;

  int n = 512, ll = 0, tot_size = 0, count = 0;
  if(strcasecmp(tokens[0], "ll") == 0)
    ll = 1;

  int err = f_opendir(&dir, _pwd);
  if(err != FR_OK) {
    printf("err: %d\n", err);
    return;
  }
  if(ll)
    printf("%-8s %-8s %-30s\n", "Size", "Attrib", "Filename");    //, "CRC32");
  while(f_readdir(&dir, &info) == FR_OK && n--) {
    if(!strlen(info.fname))
      continue;
    tot_size += info.fsize;
    count++;
    if(ll) {
      char fname[258] = {0};
      if(info.fattrib & AM_DIR)
        sprintf(fname, "[%s]", info.fname);
      else
        strcpy(fname, info.fname);
      char attrib_str[10] = "RHS-DA--";
      for(int i = 0; i < 8; i++) {
        if((info.fattrib & (1 << i)) == 0)
          attrib_str[i] = '-';
      }
      printf("%-8lu %8s %-30s\n", info.fsize, attrib_str, fname);    //, 0UL);
    }
    //        crc32(info.fname));
    else {
      if(info.fattrib & AM_DIR)
        printf("[%s]\t", info.fname);
      else
        printf("%s\t", info.fname);
    }
  }
  printf("\n");
  if(ll) {
    printf("\t%d files, %d bytes in total.\n", count, tot_size);
  }
  cmd_info(tokens);
  printf("\n");
}

int FS::filesize(const char* path)
{
  FS_LOCK lock;
  FILINFO info;
  int ret = f_stat(path, &info);
  if(ret != FR_OK)
    return ret;
  return info.fsize;
}

void FS::cmd_hexview(const char* tokens[])
{
  FS_LOCK lock;
  FIL f;
  FILINFO inf;
  uint8_t buf[16] = {0};
  uint32_t size, pos = 0;
  unsigned int n;

  const char* fname = tokens[1];
  sprintf(_genbuf, "%s/%s", _pwd, fname);

  chkret(f_stat(_genbuf, &inf));
  chkret(f_open(&f, _genbuf, FA_READ));

  size = inf.fsize;

  while(size > 0) {
    chkret(f_read(&f, buf, sizeof(buf), &n));
    if(n <= 0)
      break;
    printf("%04lX: ", pos);
    for(auto i = 0UL; i < n; i++) {
      printf("%02X ", buf[i]);
    }
    printf("\n");
    size -= n;
    pos += n;
  }
  printf("\n");
  f_close(&f);
}

uint32_t FS::crc32(const char* path)
{
  FS_LOCK lock;
  char buf[1024];
  unsigned int n;
  FIL f;
  (f_open(&f, path, FA_READ));
  unsigned long crc = 0xffffffff;
  while(1) {
    f_read(&f, buf, sizeof(buf), &n);
    if(n <= 0)
      break;
    crc = ZCRC32::calc(crc, buf, n);
  }
  f_close(&f);
  return ~crc;
}

void FS::cmd_crc32(const char* tokens[])
{
  FS_LOCK lock;
  const char* fname = tokens[1];
  sprintf(_genbuf, "%s/%s", _pwd, fname);
  char buf[1024] = {0};
  unsigned int n;
  FIL f;

  chkret(f_open(&f, _genbuf, FA_READ));

  unsigned long crc = 0xffffffff;
  while(1) {
    f_read(&f, buf, sizeof(buf), &n);
    if(n <= 0)
      break;
    crc = ZCRC32::calc(crc, buf, n);
  }
  printf("0x%08lx\n", ~crc);
  f_close(&f);
}

/*void FS::cmd_playwav(const char* tokens[])
 {
 char fname[64] = {0};
 strncpy(fname, tokens[1], sizeof(fname) - 2);
 find_match_fname(fname);

 sprintf(_genbuf, "%s/%s", _pwd, fname);
 FIL file;
 int err = f_open(&file, _genbuf, FA_READ);
 if(err != FR_OK)
 return;

 unsigned int size = filesize(_genbuf);

 printf("%d\n", size);
 auto buf = (uint8_t*)malloc(size);    //std::make_unique<uint8_t[]>(size);
 if(buf == nullptr) {
 printf("Not enough memory.\n");
 f_close(&file);
 return;
 }
 else {
 printf("OK!\n");
 f_read(&file, buf, size, &size);
 _dbg();

 //    for(int i = 0; i < size / 2; i++) { while(spi_i2s_flag_get(SPI0, SPI_FLAG_TBE) == RESET); spi_i2s_data_transmit(SPI0, ((uint16_t*)buf)[i / 2]); }

 //    I2S2_Start(buf, size / 2);
 _dbg();
 //    _delay_ms(2000); _dbg();
 free(buf);
 f_close(&file);
 }

 char buf[64] = {0};

 while(size > 0) {
 unsigned int n = sizeof(buf);
 f_read(&file, buf, n, &n);
 if(n <= 0)
 break;
 fwrite(buf, n, 1, stdout);
 fflush(stdout);
 size -= n;
 }
 f_close(&file);
 }*/

void FS::cmd_cat(const char* tokens[])
{
  FS_LOCK lock;
  char fname[64] = {0};
  strncpy(fname, tokens[1], sizeof(fname) - 2);
  find_match_fname(fname);    // in pwd
  sprintf(_genbuf, "%s/%s", _pwd, fname);

  FIL file;
  int err = f_open(&file, _genbuf, FA_READ);
  if(err != FR_OK)
    return;

  int size = filesize(_genbuf);
  char buf[64] = {0};

  while(size > 0) {
    unsigned int n = sizeof(buf);
    f_read(&file, buf, n, &n);
    if(n <= 0)
      break;
    fwrite(buf, n, 1, stdout);
    fflush(stdout);
    size -= n;
  }
  f_close(&file);
}

void FS::init(void)
{
  FS_LOCK lock;
  strcpy(_pwd, "/");
  auto result = f_mount(&_fs, "0:", 1);
  if(result != FR_OK) {
    printf("fatfs mount failed.\n");
//    while(1);
  }
  update_boot_count();
}

void FS::update_boot_count(void)
{
  FS_LOCK lock;
  int err;
  uint32_t boot_count = 0;
  FIL file;
  unsigned int n;
  err = f_open(&file, "boot_cnt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
  err = f_read(&file, &boot_count, sizeof(boot_count), &n);
  printf("boot_count: %lu\n", boot_count);
  boot_count++;
  err = f_rewind(&file);
  err = f_write(&file, &boot_count, sizeof(boot_count), &n);
  err = f_close(&file);
  err = err;
}
