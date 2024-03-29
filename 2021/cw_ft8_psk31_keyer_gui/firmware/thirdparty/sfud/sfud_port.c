/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016-2018, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2016-04-23
 */

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

/** * SPI write data then read data */
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

    set_cs(0); /* 开始读写数据 */
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
    /**
     * add your port spi bus and device object initialize code like this:
     * 1. rcc initialize
     * 2. gpio initialize
     * 3. spi device initialize
     * */
    flash->spi.wr = spi_write_read;
    flash->spi.lock = spi_lock;
    flash->spi.unlock = spi_unlock;
    flash->spi.user_data = NULL;
    flash->retry.delay = delay_f;
    flash->retry.times = 10000;

//    flash->spi.qspi_read = qspi_read;
    /*
     * 4. flash->spi and flash->retry item initialize
     *    flash->spi.wr = spi_write_read; //Required
     *    flash->spi.qspi_read = qspi_read; //Required when QSPI mode enable
     *    flash->spi.lock = spi_lock;
     *    flash->spi.unlock = spi_unlock;
     *    flash->spi.user_data = &spix;
     *    flash->retry.delay = null;
     *    flash->retry.times = 10000; //Required
     */

    return result;
}

/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void sfud_log_debug(const char* file, const long line, const char* format, ...)
{
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD](%s:%ld) ", file, line);
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\n", log_buf);
    va_end(args);
}

/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void sfud_log_info(const char* format, ...)
{
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD]");
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\n", log_buf);
    va_end(args);
}
