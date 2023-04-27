#ifndef _CONF_H
#define _CONF_H

#include "misc.h"
#include "flash_eeprom.h"

struct data_config {
    struct {
    } cfg;                // could be written only by user, stored in eeprom
    struct {
        unsigned short RH, T;
    } data;                     // could be written only by hardware
    struct {
    } state;                   // could be written both by user and hardware

    data_config(flash_eeprom& eep) :
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
    flash_eeprom _eep;
};

#endif
