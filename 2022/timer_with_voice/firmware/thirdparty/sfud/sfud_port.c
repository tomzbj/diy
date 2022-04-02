#include <sfud.h>
#include <stdarg.h>

#include "misc.h"
#include "platform.h"

static char log_buf[256];
void sfud_log_debug(const char* file, const long line, const char* format, ...);

#define SF_GPIO GPIOB
#define SF_PIN GPIO_Pin_12

static void set_cs(int cs)
{
    (cs) ? ( SF_GPIO->BSRR = SF_PIN) : ( SF_GPIO->BRR = SF_PIN);
}

static sfud_err spi_write_read(const sfud_spi* spi, const uint8_t* write_buf,
        size_t write_size, uint8_t* read_buf, size_t read_size)
{
    sfud_err result = SFUD_SUCCESS;
    if(write_size) {
        SFUD_ASSERT(write_buf);
    }
    if(read_size) {
        SFUD_ASSERT(read_buf);
    }
    set_cs(0); 
    for(int i = 0; i < write_size; i++)
        SPI2_ReadWrite(*write_buf++);
    if(read_size > 0) {
        //        for(int i = 0; i < read_size; i++) *read_buf++ = SPI2_ReadWrite(0xff);
        SPI2_Read_DMA(read_buf, read_size);
    }
    set_cs(1);
    return result;
}

static void spi_lock(const sfud_spi* spi)
{
    __disable_irq();
}

static void spi_unlock(const sfud_spi* spi)
{
    __enable_irq();
}

static void delay_f(void)
{
    _delay_us(10);
}

sfud_err sfud_spi_port_init(sfud_flash* flash)
{
    sfud_err result = SFUD_SUCCESS;
    // initialized before here.
    flash->spi.wr = spi_write_read;
    flash->spi.lock = spi_lock;
    flash->spi.unlock = spi_unlock;
    flash->spi.user_data = NULL;
    flash->retry.delay = delay_f;
    flash->retry.times = 10000;
    //    flash->spi.qspi_read = qspi_read;
    return result;
}

void sfud_log_debug(const char* file, const long line, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    printf("[SFUD](%s:%ld) ", file, line);
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\n", log_buf);
    va_end(args);
}

void sfud_log_info(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    printf("[SFUD]");
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\n", log_buf);
    va_end(args);
}
