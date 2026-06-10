#ifndef __USB_MUSB_REG_H__
#define __USB_MUSB_REG_H__

#include <string.h>
#include <stdio.h>
#include "py32f0xx.h"

#define	__I	volatile const	/* defines 'read only' permissions */
#define __O	volatile	/* defines 'write only' permissions */
#define __IO	volatile	/* defines 'read / write' permissions */

/**
  * @brief Register map for USB0 peripheral (USB0)
  */
typedef struct {
	__IO uint8_t ADDR    			  ;//0X00;
	__IO uint8_t POWER    			;//0X01;
	__IO uint8_t REV0[2]			  ;//0X02~03;
	__IO uint8_t INT_USB  			;//0X04;
	__IO uint8_t INT_OUT1 			;//0X05;
	__IO uint8_t INT_IN1  			;//0X06;
	__IO uint8_t REV1[1]			  ;//0X07;
	__IO uint8_t INT_USBE 			;//0X08;
	__IO uint8_t INT_OUT1E			;//0X09;
	__IO uint8_t INT_IN1E 			;//0X0A;
	__IO uint8_t REV2[1]			  ;//0X0B;
	__IO uint16_t FRAME         ;//0X0C-0X0D;
	__IO uint8_t INDEX    			;//0X0E;

	__IO uint8_t REV3[1]			  ;//0X0F;
	__IO uint8_t EP0_CSR   		  ;//0X10;//EP_CSR
	__IO uint8_t EP0_COUNT			;//0X11; //EP_COUNT0
	__IO uint8_t REV4[2]			  ;//0X12-0X13;
	__IO uint8_t IN_CSR2   			;//0X14;
	__IO uint8_t IN_CSR1   			;//0X15;
	__IO uint8_t MAX_PKT_IN   	;//0X16;
	__IO uint8_t REV5[1]			  ;//0X17
	__IO uint8_t OUT_CSR2  			;//0X18;
	__IO uint8_t OUT_CSR1  			;//0X19;
	__IO uint8_t MAX_PKT_OUT  	;//0X1A;
	__IO uint8_t REV6[1]			  ;//0X1B

	__IO uint16_t OUT_COUNT 		;//0X1C-0X1D;

	__IO uint8_t REV7[2]			  ;//0X1E~1F;
	__IO uint8_t FIFO_EP0 			;//0X20;
	__IO uint8_t REV8[3]			  ;//0X21~23;
	__IO uint8_t FIFO_EP1 			;//0X24; 
	__IO uint8_t REV9[3]			  ;//0X25~27;
	__IO uint8_t FIFO_EP2 			;//0X28;
	__IO uint8_t REV10[3]			  ;//0X29~2B;
	__IO uint8_t FIFO_EP3 			;//0X2C;    
	__IO uint8_t REV11[3]			  ;//0X2D~2F;
	__IO uint8_t FIFO_EP4 			;//0X30;
	__IO uint8_t REV12[3]			  ;//0X31~33;
	__IO uint8_t FIFO_EP5 			;//0X34; 
	__IO uint8_t REV13[3]			  ;//0X25~37;
	__IO uint8_t FIFO_EP6 			;//0X38;
	__IO uint8_t REV14[3]			  ;//0X39~3B;
	__IO uint8_t FIFO_EP7 			;//0X3C;    

}USB_TypeDef;


#define USB                   ((USB_TypeDef *)0x40005C00)

#define TRUE           (1)      ///< Boolean true, define to use in API parameters or return value
#define FALSE          (0)      ///< Boolean false, define to use in API parameters or return value

/* Interrupt register bit masks */
#define USB_INTR_SUSPEND  			0x01
#define USB_INTR_RESUME   			0x02
#define USB_INTR_RESET    			0x04
#define USB_INTR_SOF    				0x08
#define USB_INTR_EP0      			0x01

/*power*/
#define USB_POWER_RESUME  			0x04
#define USB_POWER_SUSPENDENB  		0x01

/* CSR0 bit masks */
#define USB_CSR0_OUTPKTRDY        	0x01
#define USB_CSR0_INPKTRDY         	0x02
#define USB_CSR0_SENTSTALL        	0x04
#define USB_CSR0_DATAEND          	0x08
#define USB_CSR0_SETUPEND         	0x10
#define USB_CSR0_SENDSTALL        	0x20
#define USB_CSR0_SVDOUTPKTRDY     	0x40
#define USB_CSR0_SVDSETUPEND      	0x80

/* Endpoint CSR register bits */
#define USB_INCSR_IPR          		0x01
#define USB_INCSR_FifoNotEmpty 		0x02
#define USB_INCSR_UnderRun     		0x04
#define USB_INCSR_FF           		0x08
#define USB_INCSR_SendStall    		0x10
#define USB_INCSR_SentStall    		0x20
#define USB_INCSR_CDT          		0x40

#define USB_INCSR2_FrcDataTog 		0x08
#define USB_INCSR2_DMAEnab 			0x10
#define USB_INCSR2_MODE    			0x20
#define USB_INCSR2_ISO     			0x40
#define USB_INCSR2_AUTOSET 			0x80

#define USB_OUTCSR_OPR        		0x01
#define USB_OUTCSR_FifoFull   		0x02
#define USB_OUTCSR_OverRun    		0x04
#define USB_OUTCSR_DataError  		0x08
#define USB_OUTCSR_FF         		0x10
#define USB_OUTCSR_SendStall  		0x20
#define USB_OUTCSR_SentStall  		0x40
#define USB_OUTCSR_CDT        		0x80
		
#define USB_OUTCSR2_DMAMode   		0x10
#define USB_OUTCSR2_DMAEnab   		0x20
#define USB_OUTCSR2_ISO       		0x40
#define USB_OUTCSR2_AutoClear 		0x80



/* Register read/write macros */
//#define READ_REG(reg) 		(reg)
//#define WRITE_REG(reg,data) 	((reg) = (data))

/* Define endpoint call status values */
#define USB_EP_NORMAL     0
#define USB_EP_RESET      1

/* Define device states */
#define DEVSTATE_DEFAULT        0
#define DEVSTATE_ADDRESS        1
#define DEVSTATE_CONFIG         2

#define	USB_INT_OUT_EP1		(0x2)
#define	USB_INT_OUT_EP2		(0x4)
#define	USB_INT_OUT_EP3		(0x8)
#define	USB_INT_OUT_EP4		(0x10)
#define	USB_INT_OUT_EP5		(0x20)
#define	USB_INT_OUT_EP6		(0x40)
#define	USB_INT_OUT_EP7		(0x80)

#define	USB_INT_IN_EP1		(USB_INT_OUT_EP1 << (8))
#define	USB_INT_IN_EP2		(USB_INT_OUT_EP2 << (8))
#define	USB_INT_IN_EP3		(USB_INT_OUT_EP3 << (8))
#define	USB_INT_IN_EP4		(USB_INT_OUT_EP4 << (8))
#define	USB_INT_IN_EP5		(USB_INT_OUT_EP5 << (8))
#define	USB_INT_IN_EP6		(USB_INT_OUT_EP6 << (0))
#define	USB_INT_IN_EP7		(USB_INT_OUT_EP7 << (8))


#define DATA_NEED				(0)
#define DATA_NONEED				(1)

#define CTRL_BUF_SIZE				(64)
#define INT_BUF_SIZE				(64)
#define BULK_BUF_SIZE				(64)
#define ISO_BUF_SIZE				(64)


//*****************************************************************************
//
// The following are defines for the bit fields in the USB_O_TXFIFOSZ register.
//
//*****************************************************************************
#define USB_TXFIFOSZ_DPB        0x00000010  // Double Packet Buffer Support
#define USB_TXFIFOSZ_SIZE_M     0x00000040  // Max Packet Size 512
#define USB_TXFIFOSZ_SIZE_8     0x00000001  // 8
#define USB_TXFIFOSZ_SIZE_16    0x00000002  // 16
#define USB_TXFIFOSZ_SIZE_24    0x00000003  // 24
#define USB_TXFIFOSZ_SIZE_32    0x00000004  // 32
#define USB_TXFIFOSZ_SIZE_40    0x00000005  // 40
#define USB_TXFIFOSZ_SIZE_48    0x00000006  // 48
#define USB_TXFIFOSZ_SIZE_56    0x00000007  // 56
#define USB_TXFIFOSZ_SIZE_64    0x00000008  // 64


//*****************************************************************************
//
// The following are values that can be passed to USBHostEndpointConfig() and
// USBDevEndpointConfigSet() as the ui32Flags parameter.
//
//*****************************************************************************
#define USB_EP_AUTO_SET         0x00000001  // Auto set feature enabled
#define USB_EP_AUTO_CLEAR       0x00000002  // Auto clear feature enabled
#define USB_EP_DMA_MODE_0       0x00000004  // Enable DMA access using mode 0
#define USB_EP_DMA_MODE_1       0x00000008  // Enable DMA access using mode 1
#define USB_EP_MODE_ISOC        0x00000010  // Isochronous endpoint
#define USB_EP_MODE_BULK        0x00000020  // Bulk endpoint
#define USB_EP_MODE_INT         0x00000040  // Interrupt endpoint

#define USB_EP_MODE_MASK        0x00000070  // Mode Mask


#ifndef NULL
#define NULL 0
#endif

//*****************************************************************************
//
// The following are defines for the Univeral Serial Bus register offsets.
//
//*****************************************************************************

void USBD_IRQHandler(void);
void usb_dc_low_level_deinit(void);
void usb_dc_low_level_init(void);

#endif
