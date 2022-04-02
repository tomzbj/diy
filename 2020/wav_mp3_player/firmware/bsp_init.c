#include "misc.h"
#include "platform.h"

USB_OTG_CORE_HANDLE g_udev;
FATFS g_fs;

static void setsda(int state)
{
    (state) ? (GPIOA->BSRRL = GPIO_Pin_4) : (GPIOA->BSRRH = GPIO_Pin_4);
    _delay_us(30);
}

static void setscl(int state)
{
    (state) ? (GPIOA->BSRRL = GPIO_Pin_5) : (GPIOA->BSRRH = GPIO_Pin_5);
    _delay_us(30);
}

static int getsda(void)
{
    _delay_us(30);
    return (GPIOA->IDR & GPIO_Pin_4);
}

void BSP_Init(void)
{
    SystemInit();
    SysTick_Config(84000UL);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    GPIO_Config();
    USART_Config();
    USART_BindParser(USART_Parser);
    I2S_Config();

    USBD_Init(&g_udev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_MSC_cb, &USR_cb);

    DCD_DevDisconnect(&g_udev);
    _delay_ms(10);
    DCD_DevConnect(&g_udev);

    zi2c_t i2c;
    i2c.setsda_f = setsda;
    i2c.setscl_f = setscl;
    i2c.getsda_f = getsda;
    i2c.addr = 0x18;
    zi2c_init(&i2c);
    LIS3DHTR_Init(&i2c);

    _delay_ms(10);

    printf("%d\n", SD_Init());

//    extern FATFS g_fs;
    f_mount(&g_fs, "0:", 1);

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    TASKS_Config();
}
