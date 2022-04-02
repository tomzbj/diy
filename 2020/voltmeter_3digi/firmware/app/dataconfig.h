#ifndef _DATACONFIG_H
#define _DATACONFIG_H

typedef struct {
    struct {
        unsigned char reserved;
    } cfg;
    struct {
        unsigned short data;
    } data;
    struct {
        unsigned char reserved;
    } status;
} DataConfig_t;

void DC_Config(void);
void DC_Update(void);
DataConfig_t* DC_Get(void);

#endif /* _DC_H */
