#ifndef _CONF_H
#define _CONF_H

#include "misc.h"
#include "flasheep.h"

struct DataConfig {
    struct {
    } cfg;                // could be written only by user, stored in eeprom
    struct {
    } data;                     // could be written only by hardware
    struct {
        int debug;
    } state;                   // could be written both by user and hardware

//    data_config(size_t base_addr, size_t page_size) : _eep(base_addr, page_size)
    DataConfig(FlashEEP& eep) :
        _eep(eep)
    {
    }
    void init(void)
    {
        _eep.read(0, &cfg, sizeof(cfg));
    }
    void update(void)
    {
        unsigned char buf[sizeof(cfg)];
        _eep.read(0, buf, sizeof(cfg));
        if(memcmp(&cfg, buf, sizeof(cfg)) != 0)
            _eep.write(0, &cfg, sizeof(cfg));

    }
private:
    FlashEEP _eep;
};

#endif
