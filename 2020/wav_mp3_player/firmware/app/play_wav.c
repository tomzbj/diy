#include "platform.h"

typedef unsigned long u32_t;
typedef unsigned short u16_t;

#define FCC(c1,c2,c3,c4)    (((u32_t)c4<<24)+((u32_t)c3<<16)+((u32_t)c2<<8)+(unsigned char)c1)    /* FourCC */

typedef struct {
    u32_t id;
    u32_t size;
    u32_t fmt;
} wave_header_t;

typedef struct {
    u32_t id;
    u32_t size;
} chunk_header_t;

typedef struct {
    u16_t wFormatTag;
    u16_t wChannels;
    u32_t dwSamplesPerSec;
    u32_t dwAvgBytesPerSec;
    u16_t wBlockAlign;
    u16_t wBitsPerSample;
} wave_format_t;

static struct {
    const char* fname;
    FIL f;
    FILINFO inf;
    u32_t addr;
} g;

static int open_f(void)
{
    unsigned int n;
    wave_header_t header;
    chunk_header_t chunk_header;
    wave_format_t fmt;

    f_stat(g.fname, &g.inf);
    f_open(&g.f, g.fname, FA_READ);
    f_read(&g.f, &header, sizeof(header), &n);

    if(header.id != FCC('R', 'I', 'F', 'F'))
        return -1;
    if(header.fmt != FCC('W', 'A', 'V', 'E'))
        return -1;

    while(1) {
        f_read(&g.f, &chunk_header, sizeof(chunk_header), &n);
        switch(chunk_header.id) {
            case FCC('f', 'm', 't', ' '): {

                u32_t sample_rate;
                f_read(&g.f, &fmt, sizeof(fmt), &n);
                printf("Filename:    %s\n", g.fname);
                printf("Channels:    %d\n", fmt.wChannels);
                printf("Bit Depth:   %d\n", fmt.wBitsPerSample);
                printf("Sample Rate: %lu\n", fmt.dwSamplesPerSec);
                printf("Byte Rate:   %lu\n", fmt.dwAvgBytesPerSec);
                f_lseek(&g.f, f_tell(&g.f) + chunk_header.size - 16);

                sample_rate = fmt.dwSamplesPerSec;
                if(fmt.wChannels == 1)
                    sample_rate /= 2;
                if(fmt.wBitsPerSample != 16)
                    return -3;

                I2S_InitTypeDef i2s_is;

                I2S_Cmd(SPI2, DISABLE);
                i2s_is.I2S_Standard = I2S_Standard_Phillips;
                i2s_is.I2S_DataFormat = I2S_DataFormat_16bextended;
                i2s_is.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
                i2s_is.I2S_AudioFreq = sample_rate;
                i2s_is.I2S_CPOL = I2S_CPOL_High;
                i2s_is.I2S_Mode = I2S_Mode_MasterTx;
                I2S_Init(SPI2, &i2s_is);
                I2S_Cmd(SPI2, ENABLE);
            }
                break;
            case FCC('d', 'a', 't', 'a'): {
                return 0;
            }
            case FCC('D', 'I', 'S', 'P'):
            case FCC('L', 'I', 'S', 'T'):
            case FCC('f', 'a', 'c', 't'):
                if(chunk_header.size & 1)
                    chunk_header.size++;
                f_lseek(&g.f, f_tell(&g.f) + chunk_header.size);
                break;
        }
    }
}

static void close_f(void)
{
    f_close(&g.f);
}

//static void read_f(u32_t addr, unsigned int size, void* buf)
static void read_f(void* buf)
{
    unsigned int size = DC_OUT_BUF_SIZE / 2;
    if(f_eof(&g.f)) {
        zplay_stop();
        return;
    }

    f_lseek(&g.f, g.addr);
    f_read(&g.f, buf, size, &size);
    g.addr += size;
}

void play_wav_init(const char* fname)
{
    DataConfig_t* pdc = DC_Get();

    g.fname = fname;
    g.addr = 0;
    zplay_cfg_t cfg;
    cfg.buf = pdc->data.out_buf;
    cfg.buf_size = DC_OUT_BUF_SIZE;
    cfg.dma_write_start_f = I2S_Write_DMA;
    cfg.dma_write_stop_f = I2S_Write_DMA_Stop;
    cfg.open_f = open_f;
    cfg.close_f = close_f;
    cfg.read_f = read_f;
    zplay_init(&cfg);
}
