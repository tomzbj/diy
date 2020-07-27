#include "platform.h"
#include "usbd_msc_core.h"

USB_CORE_HANDLE USB_Device_dev;

static void SPI2_SetCS(int cs)
{
    (cs) ? (GPIOB->BSRR = GPIO_Pin_12) : (GPIOB->BRR = GPIO_Pin_12);
}

static void SPI1_SetCS(int cs)
{
    // cs inverted
    (cs) ? (GPIOB->BRR = GPIO_Pin_6) : (GPIOB->BSRR = GPIO_Pin_6);
}

unsigned char SPI2_WriteByte(unsigned char data)
{
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_SendData8(SPI2, data);
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_ReceiveData8(SPI2);
}

void SPI1_WriteByte(unsigned char data)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_SendData8(SPI1, data);
//    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//    return SPI_ReceiveData8(SPI1);
}

//int uputs(const void* data, int size) { USART_WriteData(USART_USB, data, size); return 0; }

static void SPIFLASH_Config(void)
{
    SPI2_SetCS(1);
    static spiflash_cfg_t scfg;
    scfg.cs_f = SPI2_SetCS;
    scfg.writebyte_f = SPI2_WriteByte;
    scfg.fastread_f = SPI2_Read_DMA;
    SPIFLASH_Init(&scfg);
    SPIFLASH_LeaveLowPowerMode();
}

void BSP_Init(void)
{
    SystemInit();
    SysTick_Config(48000UL);
    GPIO_Config();
    SPI_Config();
    SPIFLASH_Config();
    {
        SPI1_SetCS(1);
        static hc595_cfg_t cfg;
        cfg.cs_f = SPI1_SetCS;
        cfg.writebyte_f = SPI1_WriteByte;
        cfg.fastwrite_f = SPI1_Write_DMA;
        cfg.delay_f = _delay_ms_loop;
        HC595_Init(&cfg);

    }
//    KEY_Config();
    USART_Config();
//    USBD_Init(&USB_Device_dev, &USR_desc, &USBD_HID_cb, &USR_cb);
    USBD_Init(&USB_Device_dev, &USR_desc, &USBD_MSC_cb, &USR_cb);

    DC_Config();
}
