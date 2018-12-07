#ifndef _VS1003_H_
#define _VS1003_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "vs1003_port.h"

/* Private define ------------------------------------------------------------*/
#define VS_WRITE_COMMAND 	0x02
#define VS_READ_COMMAND 	0x03

#define SM_DIFF         	0x01   
#define SM_JUMP         	0x02   
#define SM_RESET        	0x04   
#define SM_OUTOFWAV     	0x08   
#define SM_PDOWN        	0x10   
#define SM_TESTS        	0x20   
#define SM_STREAM       	0x40   
#define SM_PLUSV        	0x80   
#define SM_DACT         	0x100   
#define SM_SDIORD       	0x200   
#define SM_SDISHARE     	0x400   
#define SM_SDINEW       	0x800   
#define SM_ADPCM        	0x1000   
#define SM_ADPCM_HP     	0x2000 	

/* Private function prototypes -----------------------------------------------*/
void VS1003_Init(void);
void VS1003_SetVol(void); 
void VS1003_Reset(void);
void VS1003_RamTest(void);
void VS1003_SineTest(void);
void VS1003_SoftReset(void); 
void VS1003_Record_Init(void);
void VS1003_ResetDecodeTime(void);
unsigned short VS1003_GetDecodeTime(void);
void VS1003_WriteData(unsigned char* buf, int size);
	 
#endif
