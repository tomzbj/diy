#ifndef _CMD_H
#define _CMD_H

#define CMD_LS  1
#define CMD_LL  1
#define CMD_INFO 1
#define CMD_INITIALIZE 0
#define CMD_MKFS 1
#define CMD_CHECK 0
#define CMD_TOUCH 1
#define CMD_RM 1
#define CMD_CAT 1
#define CMD_CRC32 0
#define CMD_MV 0
#define CMD_VIEWSECT 1
#define CMD_L 0
#define CMD_STATUS 1
#define CMD_FLASHID 1
#define CMD_PLAY 1

void FSCMD_ls(void);
void FSCMD_ll(void);
void FSCMD_info(void);
void FSCMD_initialize(void);
void FSCMD_check(void);

void FSCMD_touch(char* fname);
void FSCMD_rm(char* fname);
void FSCMD_cat(char* fname);
void FSCMD_crc32(char* fname);
void FSCMD_l(char* fname);

void FSCMD_mv(char* fname, char* fname2);

//void FSCMD_Parse(char* const tokens[], int count);
void FSCMD_Parse(char* msg);

#endif
