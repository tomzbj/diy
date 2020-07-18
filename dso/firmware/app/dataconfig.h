#ifndef _DATACONFIG_H
#define _DATACONFIG_H

#define DC_ADC_BUF_SIZE 8000

enum {
    DC_MODE_NORMAL = 0, DC_MODE_FAST_SETTING, DC_MODE_MENU
};

enum {
    DC_COUPLING_DC = 0,
    DC_COUPLING_AC,
    DC_COUPLING_GND,
    DC_COUPLING_MAX = DC_COUPLING_GND
};
enum {
    DC_SEN_1X = 0,
    DC_SEN_2X,
    DC_SEN_5X,
    DC_SEN_10X,
    DC_SEN_20X,
    DC_SEN_50X,
    DC_SEN_100X,
    DC_SEN_200X,
    DC_SEN_500X,
    DC_SEN_MAX = DC_SEN_500X
};
//enum { DC_SEN1_1X, DC_SEN1_10X, DC_SEN1_100X, DC_SEN1_MAX = DC_SEN1_100X };
//enum { DC_SEN2_1X, DC_SEN2_2X, DC_SEN2_5X, DC_SEN2_MAX = DC_SEN2_5X };
enum {
    DC_TRIG_MODE_AUTO = 0,
    DC_TRIG_MODE_NORMAL,
    DC_TRIG_MODE_SINGLE,
    DC_TRIG_MODE_MAX = DC_TRIG_MODE_SINGLE
};

enum {
    DC_TRIG_EDGE_FALLING = 0,
    DC_TRIG_EDGE_RISING,
    DC_TRIG_EDGE_BOTH,
    DC_TRIG_EDGE_MAX = DC_TRIG_EDGE_BOTH
};

enum {
    DC_TIMEBASE_5US = 0,
    DC_TIMEBASE_10US,
    DC_TIMEBASE_25US,
    DC_TIMEBASE_50US,
    DC_TIMEBASE_100US,
    DC_TIMEBASE_250US,
    DC_TIMEBASE_500US,
    DC_TIMEBASE_1MS,
    DC_TIMEBASE_2_5MS,
    DC_TIMEBASE_5MS,
    DC_TIMEBASE_10MS,
    DC_TIMEBASE_25MS,
    DC_TIMEBASE_MAX = DC_TIMEBASE_25MS
};

typedef struct {
    struct {
        unsigned char buf[DC_ADC_BUF_SIZE];
    } data;
    struct {
        signed short trig_level;    // in mV
        unsigned char coupling;    // dc / ac / gnd
        unsigned char sen;
        unsigned char trig_mode;    // auto / normal / single
        unsigned char trig_edge;    // rising / falling / both
        unsigned char timebase;    // in us
    } cfg;
    struct {
        unsigned char mode;
    } status;
} DataConfig_t;

void DC_Config(void);
void DC_Update(void);
DataConfig_t* DC_Get(void);

#endif /* _DC_H */
