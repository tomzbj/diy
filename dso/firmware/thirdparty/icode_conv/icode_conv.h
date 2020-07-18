#ifndef _ICODE_CONV_H
#define _ICODE_CONV_H

#define UNI2GB_TAB_SIZE (29780 / 4)
#define UNI2GB_TAB_OFS (0x131000)
#define uni2gb_rec_get SPIFLASH_FastRead
unsigned short unicode_gb2312(unsigned short in_chr);
void str_utf2gb(const unsigned char* str, unsigned char* str_out);

#endif
