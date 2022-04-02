#include "platform.h"
#include "usbd_msc_core.h"

USB_CORE_HANDLE USB_Device_dev;
FATFS g_fs;

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
}

void SPI1_WriteData(void* buf, int nbytes)
{
    while(nbytes > 0) {
        SPI1_WriteByte(*(unsigned char*)buf);
        buf++;
        nbytes--;
    }
}

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
    FLASH_EEPROM_Config(0x08002800 - 2048, 2048);
    SysTick_Config(48000UL);
    GPIO_Config();
    USART_Config();
    {
        SPI1_SetCS(1);
        static hc595_cfg_t cfg;
        cfg.cs_f = SPI1_SetCS;
        cfg.writebyte_f = SPI1_WriteByte;
//        cfg.fastwrite_f = SPI1_Write_DMA;
        cfg.fastwrite_f = SPI1_WriteData;
        cfg.delay_f = _delay_ms_loop;
        HC595_Init(&cfg);
    }
    SPI_Config();
    SPIFLASH_Config();
    extern FATFS g_fs;
    f_mount(&g_fs, "0:", 1);

    DC_Config();
    USBD_Init(&USB_Device_dev, &USR_desc, &USBD_MSC_cb, &USR_cb);
}
