#include "misc.h"
#include "platform.h"

static void set_cs(int cs)
{
    if(cs)
        GPIOB->BSRR = GPIO_Pin_1;
    else
        GPIOB->BRR = GPIO_Pin_1;
}

static void set_reset(int reset)
{
}

int RFID_ReadID(void)
{
    int status, ret, id;
    RC522_AntennaOn();

    do {
        status = RC522_Request(0x52, &ret);
        if(status == MI_ERR) {
            ret = -1;
            break;
        }
        status = RC522_Anticoll(&id);
        if(status == MI_ERR) {
            ret = -2;
            break;
        }
        status = RC522_Halt();
        if(status == MI_ERR) {
            ret = -3;
            break;
        }
        ret = id;
    } while(0);
    RC522_AntennaOff();
    return ret;
}

void RFID_Read(void)
{
    unsigned short status, ret;
    unsigned long id;
    char* str_type = "";

    do {
        status = RC522_Request(0x52, &ret);
        if(status == MI_ERR) {
            xprintf("Request failed!\n");
            break;
        }
        switch(ret) {
            case 0x4:
                str_type = "MF1-S50";
                break;
            case 0x8:
                str_type = "MF-Pro";
                break;
            case 0x2:
                str_type = "MF1-S70";
                break;
            case 0x44:
                str_type = "MF-Desire";
                break;
            default:
                str_type = "Unknown";
        }
        status = RC522_Anticoll(&id);
        if(status == MI_ERR) {
            xprintf("Anticoll failed!\n");
            break;
        }
        xprintf("Card Type: %s      Card ID: %08lx\n", str_type, id);
        status = RC522_Select(&id);
        if(status == MI_ERR) {
            xprintf("Card select failed!\n");
            break;
        }
        unsigned char default_key[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
//        unsigned char default_key[] = {0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0};

        DataConfig_t* pdc = DC_Get();
        unsigned char* p = pdc->data.buffer;
        pdc->data.buffer_id = id;
        for(int i = 0; i < 64; i++) {
            status = RC522_AuthState(0x61, i, default_key, &id);
            if(status == MI_ERR) {
                xprintf("Auth failed!\n");
                continue;
            }
            status = RC522_Read(i, p);
            if(status == MI_ERR) {
                xprintf("Read failed!");
            }
            p += 16;
        }
        status = RC522_Halt();
        if(status == MI_ERR) {
            xprintf("Halt failed!");
            break;
        }
        xprintf("Read done!\n");
        unsigned long crc = 0xffffffff;
        crc = crc32_soft(crc, p, 1024);
        xprintf("CRC Checksum: %08lx\n", ~crc);
    } while(0);
}

void RFID_Write(void)
{
    unsigned short status, ret;
    unsigned long id;
    char* str_type = "";
    do {
        status = RC522_Request(0x52, &ret);
        if(status == MI_ERR) {
            xprintf("Request failed!\n");
            break;
        }
        switch(ret) {
            case 0x4:
                str_type = "MF1-S50";
                break;
            case 0x8:
                str_type = "MF-Pro";
                break;
            case 0x2:
                str_type = "MF1-S70";
                break;
            case 0x44:
                str_type = "MF-Desire";
                break;
            default:
                str_type = "Unknown";
        }
        status = RC522_Anticoll(&id);
        if(status == MI_ERR) {
            xprintf("Anticoll failed!\n");
            break;
        }
        xprintf("Card Type: %s      Card ID: %08lx\n", str_type, id);
        _dbg();
        status = RC522_Select(&id);
        if(status == MI_ERR) {
            xprintf("Card select failed!\n");
            break;
        }
//        unsigned char default_key[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
        unsigned char default_key[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

        for(int i = 0; i < 64; i++) {
            if(i != 0) {
                status = RC522_AuthState(0x61, i, default_key, &id);
                if(status == MI_ERR) {
                    xprintf("Auth failed!\n");
                    continue;
                }
            }
            DataConfig_t* pdc = DC_Get();
            status = RC522_Write(i, &(pdc->data.buffer[i * 16]));
            if(status == MI_ERR) {
                xprintf("Write failed!");
            }
        }
        status = RC522_Halt();
        if(status == MI_ERR) {
            xprintf("Halt failed!");
            break;
        }

        xprintf("Write done!\n");
    } while(0);
}

void RFID_Config(void)
{
    rc522_cfg_t cfg;
    set_cs(1);
    cfg.cs_f = set_cs;
    cfg.reset_f = set_reset;
    cfg.writebyte_f = SPI1_ReadWrite;
    cfg.delay_us_f = _delay_us;
    RC522_Init(&cfg);
    RC522_AntennaOn();
}
