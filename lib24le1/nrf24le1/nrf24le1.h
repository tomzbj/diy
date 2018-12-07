#ifndef _NRF24LE1_H
#define _NRF24LE1_H

typedef enum {
    DISABLE = 0, ENABLE = !DISABLE
} FuncState;

#define _BV(x) (1UL << (x))
#define nop() __asm nop __endasm	//No-operation instruction wrapper

// GPIO registers
__sfr __at 0x80 P0;
__sfr __at 0x90 P1;
__sfr __at 0xA0 P2;
__sfr __at 0xB0 P3;
__sfr __at 0x9E P0CON;
__sfr __at 0x9F P1CON;
__sfr __at 0x97 P2CON;
__sfr __at 0x8F P3CON;
__sfr __at 0x93 P0DIR;
__sfr __at 0x94 P1DIR;
__sfr __at 0x95 P2DIR;
__sfr __at 0x96 P3DIR;

// UART registers
__sfr __at 0x98 S0CON;
__sfr __at 0x99 S0BUF;
__sfr __at 0xAA S0RELL;
__sfr __at 0xBA S0RELH;
__sfr16 __at 0xBAAA S0REL;
__sfr __at 0xD8 ADCON;
__sfr __at 0x87 PCON;

// SYSCLK registers
__sfr __at 0xA3 CLKCTRL;

// INTERRUPT REGS
__sfr __at 0xA8 IEN0;
__sfr __at 0xB8 IEN1;
__sfr __at 0xA6 INTEXP;
__sfr __at 0xC0 IRCON;

// SPI REGS
__sfr __at 0xFC SPIMCON0;
__sfr __at 0xFD SPIMCON1;
__sfr __at 0xFE SPIMSTAT;
__sfr __at 0xFF SPIMDAT;

// ADC REGS
__sfr __at 0xD1 ADCCON3;
__sfr __at 0xD2 ADCCON2;
__sfr __at 0xD3 ADCCON1;
__sfr __at 0xD4 ADCDATH;
__sfr __at 0xD5 ADCDATL;
__sfr16 __at 0xD4D5 ADCDAT;

// PWM REGS
__sfr __at 0xB2 PWMCON;
__sfr __at 0xA1 PWMDC0;
__sfr __at 0xA2 PWMDC1;

// PWR REGS
__sfr __at 0xA4 PWRDWN;
__sfr __at 0xAE OPMCON;
__sfr __at 0xA5 WUCON;
__sfr __at 0xCE WUOPC1;
__sfr __at 0xCF WUOPC0;
__sfr __at 0xDC POFCON;

// RF REGS
__sfr __at 0xE8 RFCON;
__sfr __at 0xE4 SPIRCON0;
__sfr __at 0xE5 SPIRCON1;
__sfr __at 0xE6 SPIRSTAT;
__sfr __at 0xE7 SPIRDAT;

#include "nrf24le1_conf.h"

/*
__sfr __at 0x81 SP;
__sfr __at 0x82 DPL;
__sfr __at 0x83 DPH;
__sfr __at 0x84 DPL1;
__sfr __at 0x85 DPH1;
__sfr __at 0x88 TCON;
__sfr __at 0x89 TMOD;
__sfr __at 0x8A TL0;
__sfr __at 0x8B TL1;
__sfr __at 0x8C TH0;
__sfr __at 0x8D TH1;
__sfr __at 0x92 DPS;
__sfr __at 0xA7 MEMCON;
__sfr __at 0xA9 IP0;
__sfr __at 0xAB RTC2CPT01;
__sfr __at 0xAC RTC2CPT10;
__sfr __at 0xAD CLKLFCTRL;
__sfr __at 0xAF WDSV;
__sfr __at 0xB1 RSTREAS;
__sfr __at 0xB3 RTC2CON;
__sfr __at 0xB4 RTC2CMP0;
__sfr __at 0xB5 RTC2CMP1;
__sfr __at 0xB6 RTC2CPT00;
__sfr __at 0xB7 SPISRDSZ;
__sfr __at 0xB9 IP1;
__sfr __at 0xBC SPISCON0;
__sfr __at 0xBD SPISCON1;
__sfr __at 0xBE SPISSTAT;
__sfr __at 0xBF SPISDAT;
__sfr __at 0xC1 CCEN;
__sfr __at 0xC2 CCL1;
__sfr __at 0xC3 CCH1;
__sfr __at 0xC4 CCL2;
__sfr __at 0xC5 CCH2;
__sfr __at 0xC6 CCL3;
__sfr __at 0xC7 CCH3;
__sfr __at 0xC8 T2CON;
__sfr __at 0xC9 MPAGE;
__sfr __at 0xC9 _XPAGE; //for SDCC's memory initialization
__sfr __at 0xCA CRCL;
__sfr __at 0xCB CRCH;
__sfr __at 0xCC TL2;
__sfr __at 0xCD TH2;
__sfr __at 0xD0 PSW;
__sfr __at 0xD6 RNGCTL;
__sfr __at 0xD7 RNGDAT;
__sfr __at 0xD9 W2SADR;
__sfr __at 0xDA W2DAT;
__sfr __at 0xDB COMPCON;
__sfr __at 0xDD CCPDATIA;
__sfr __at 0xDE CCPDATIB;
__sfr __at 0xDF CCPDATO;
__sfr __at 0xE0 ACC;
__sfr __at 0xE1 W2CON1;
__sfr __at 0xE2 W2CON0;
__sfr __at 0xE9 MD0;
__sfr __at 0xEA MD1;
__sfr __at 0xEB MD2;
__sfr __at 0xEC MD3;
__sfr __at 0xED MD4;
__sfr __at 0xEE MD5;
__sfr __at 0xEF ARCON;
__sfr __at 0xF0 B;
__sfr __at 0xF8 FSR;
__sfr __at 0xF9 FPCR;
__sfr __at 0xFA FCR;
///////////////////////////////////////////
// Two-byte __sfrs
///////////////////////////////////////////
__sfr16 __at 0xC3C2 CC1;
__sfr16 __at 0xC5C4 CC2;
__sfr16 __at 0xC7C6 CC3;
__sfr16 __at 0xCBCA CRC;
__sfr16 __at 0x8C8A T0;
__sfr16 __at 0x8D8B T1;
__sfr16 __at 0xCDCC T2;

*/


#endif
