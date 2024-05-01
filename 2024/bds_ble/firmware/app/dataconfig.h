#ifndef _CONF_H
#define _CONF_H

#include "misc.h"
#include "flasheep.h"

class DataConfig {
public:
    enum {
        RF_MODE_RX = 0, RF_MODE_TX = 1
    };

    DataConfig(FlashEEP& eep) : _eep(eep)
    {
        state.inteval = 3;
        state.mode = RF_MODE_TX;
    }
    void init(void)
    {
        _eep.read(0, &cfg, sizeof(cfg));
        cfg.tz_ofs = +(8 * 3600);
    }
    void update(void)
    {
        uint8_t buf[sizeof(cfg)];
        _eep.read(0, buf, sizeof(cfg));
//        if(memcmp(&cfg, buf, sizeof(cfg)) != 0) _eep.write(0, &cfg, sizeof(cfg));
    }
    struct {
        int tz_ofs;
        char name[8];
    } cfg;                // could be written only by user, stored in eeprom
    struct {
        // time, vbat, rh, t
        uint32_t time;    // 000000-235959, 18bits
        int rh, t, vbat, hours_since_last_sync;
    } data;                     // could be written only by hardware
    struct {
        int inteval;
        int mode;
        int debug;
    } state;                   // could be written both by user and hardware

private:
    FlashEEP _eep;
};

#endif
