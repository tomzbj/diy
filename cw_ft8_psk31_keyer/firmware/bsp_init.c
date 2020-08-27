#include "platform.h"

USB_CORE_HANDLE USB_Device_dev;

void BSP_Init(void)
{
    SystemInit();
    FLASH_EEPROM_Config(0x08002800 - 2048, 2048);
    SysTick_Config(48000UL);
    DC_Config();
    GPIO_Config();
    DAC_Config();
    USART_BindParser(usart_parser);
    USART_Config();
    {
        cw_cbs_t cbs;
        cbs.tx_char_f = cw_tx_char;
        cbs.tx_symbol_f = cw_tx_symbol;
        cbs.tx_start_f = cw_tx_start;
        cbs.tx_end_f = cw_tx_end;
        cw_init(&cbs);
    }
    {
        bpsk31_cbs_t cbs;
        cbs.tx_char_f = bpsk_tx_char;
        cbs.tx_start_f = bpsk_tx_start;
        cbs.tx_end_f = bpsk_tx_end;
        cbs.tx_symbol_f = bpsk_tx_symbol;
        bpsk31_init(&cbs);
    }
    {
        ft8_cbs_t cbs;
        cbs.tx_start_f = ft8_tx_start;
        cbs.tx_end_f = ft8_tx_end;
        cbs.tx_symbol_f = ft8_tx_symbol;
        ft8_init(&cbs);
    }
    TIMER_Config();
    USBD_Init(&USB_Device_dev, &USR_desc, &USBD_CDC_cb, &USR_cb);
}
