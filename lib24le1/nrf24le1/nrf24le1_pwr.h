#ifndef _NRF24LE1_PWR_H
#define _NRF24LE1_PWR_H

// old style, need optimizing
typedef enum {
    PWR_Mode_DeepSleep = 1, PWR_Mode_MemoryRetention_TimerOff = 2,
    PWR_Mode_MemoryRetention_TimerOn = 3, PWR_Mode_RegisterRetention = 4,
    PWR_Mode_Standby = 7
} PWR_ModeTypeDef;

typedef enum {
    PWR_SubsetOfWakeupPins_ActiveLow = 1, PWR_AllWakeupPins_ActiveHigh = 0
} PWR_WakeupPinsTypeDef;

typedef enum {
    PWR_Latch_Open = 0, PWR_Latch_Locked = 1
} PWR_LatchCtrlTypeDef;

typedef struct {
    unsigned char PWR_WatchDog_Reset :1;
    unsigned char PWR_LatchCtrl :1;
    unsigned char PWR_Mode :1;
    unsigned char reserved :5;
} PWR_OpModeTypeDef;

typedef enum {
    PWR_EnableWakeupOnRFIRQ_IRQen = 0, PWR_EnableWakeupOnRFIRQ = 0x2,
    PWR_IgnoreRFIRQ = 0x3
} PWR_WakeupOnRFIRQTypeDef;

typedef enum {
    PWR_EnableWakeupOnTICK_IRQen = 0, PWR_EnableWakeupOnTICK = 0x2,
    PWR_IgnoreTICK = 0x3
} PWR_WakeupOnTICKTypeDef;

typedef enum {
    PWR_EnableWakeupOnWUOPIRQ_IRQen = 0, PWR_EnableWakeupOnWUOPIRQ = 0x2,
    PWR_IgnoreWUOPIRQ = 0x3
} PWR_WakeupOnWUOPIRQTypeDef;

typedef enum {
    PWR_EnableWakeupOnMISCIRQ_IRQen = 0, PWR_EnableWakeupOnMISCIRQ = 0x2,
    PWR_IgnoreMISCIRQ = 0x3
} PWR_WakeupOnMISCIRQTypeDef;

typedef struct {
    unsigned char PWR_WakeupOnMISCIRQ :2;
    unsigned char PWR_WakeupOnWUOPIRQ :2;
    unsigned char PWR_WakeupOnTICKIRQ :2;
    unsigned char PWR_WakeupOnRFIRQ :2;
} PWR_WakeupSourceTypeDef;

// for Q48 package: Pin0_0~0_7: P2.0~P2.7   Pin1_0~1_7: P3.0~P3.6, P1.7
// for Q24 package: Pin0_0~0_7: P0.0~P0.7   Pin1_0~1_6: P1.0~P1.6
// for Q48 package: Pin0_0~0_6: P0.0~P0.6

typedef struct {
    unsigned char PWR_WakeupOnPin_0_0 :1;
    unsigned char PWR_WakeupOnPin_0_1 :1;
    unsigned char PWR_WakeupOnPin_0_2 :1;
    unsigned char PWR_WakeupOnPin_0_3 :1;
    unsigned char PWR_WakeupOnPin_0_4 :1;
    unsigned char PWR_WakeupOnPin_0_5 :1;
    unsigned char PWR_WakeupOnPin_0_6 :1;
    unsigned char PWR_WakeupOnPin_0_7 :1;
    unsigned char PWR_WakeupOnPin_1_0 :1;
    unsigned char PWR_WakeupOnPin_1_1 :1;
    unsigned char PWR_WakeupOnPin_1_2 :1;
    unsigned char PWR_WakeupOnPin_1_3 :1;
    unsigned char PWR_WakeupOnPin_1_4 :1;
    unsigned char PWR_WakeupOnPin_1_5 :1;
    unsigned char PWR_WakeupOnPin_1_6 :1;
    unsigned char PWR_WakeupOnPin_1_7 :1;
} PWR_WakeupOnPinTypeDef;

typedef enum {
    PWR_FailThreshold_2V1 = 0, PWR_FailThreshold_2V3 = 1,
    PWR_FailThreshold_2V5 = 2, PWR_FailThreshold_2V7 = 3
} PWR_FailThresholdTypeDef;

typedef struct {
    unsigned char reserved:5;
    unsigned char PWR_FailThreshold:2;
    unsigned char PWR_FailComparator:1;
} PWR_FailComparatorTypeDef;

void PWR_SetMode(PWR_ModeTypeDef* mode);
void PWR_OpModeCtrl(PWR_OpModeTypeDef* opmode);
void PWR_WakeupSourceConfig(PWR_WakeupSourceTypeDef* source);
void PWR_WakeupOnPinConfig(PWR_WakeupOnPinTypeDef* pins);
void PWR_FailComparatorConfig(PWR_FailComparatorTypeDef* comp);


#endif
