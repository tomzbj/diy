#include "platform.h"

#include "coder.h"
#include "mp3dec.h"

typedef unsigned long u32_t;
typedef unsigned short u16_t;

static struct {
    const char* fname;
    FIL f;
    FILINFO inf;
    HMP3Decoder decoder;
    MP3FrameInfo frame_info;
    int first_frame;
    int offset;
} g;

static void read_f(void* out_buf);
static void i2s_adjust(int sample_rate, int buf_size);

static int open_f(void)
{
    if(f_open(&g.f, g.fname, FA_READ) != FR_OK) {
        printf("Open file %s failed.\n", g.fname);
        return -1;
    }
    g.decoder = MP3InitDecoder();
    if(g.decoder == NULL) {
        printf("Failed to allocate memory.\n");
        return -2;
    }
    g.first_frame = 1;
    return 0;
}

static void close_f(void)
{
    // close ops
    f_close(&g.f);
    MP3FreeDecoder(g.decoder);
}

static void i2s_adjust(int sample_rate, int buf_size)
{
    DMA_Cmd(DMA1_Stream4, DISABLE);
    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, DISABLE);
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
    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
    DMA_Cmd(DMA1_Stream4, ENABLE);

    zplay_cfg_t cfg;
    DataConfig_t* pdc = DC_Get();
    cfg.buf = pdc->data.out_buf;
    cfg.buf_size = buf_size;    //g.synth.pcm.length * 8;    //g.synth.pcm.length * 2 * 2;    //1152 * 2 * 2;
    memset(cfg.buf, 0, DC_OUT_BUF_SIZE);

    cfg.dma_write_start_f = I2S_Write_DMA;
    cfg.dma_write_stop_f = I2S_Write_DMA_Stop;
    cfg.open_f = open_f;
    cfg.close_f = close_f;
    cfg.read_f = read_f;
    zplay_init(&cfg);
}

static void read_f(void* out_buf)
{
    static size_t size;
    static int bytesLeft;
    static unsigned char* in_ptr;
//    static int pos = 0;

    DataConfig_t* pdc = DC_Get();

    while(1) {
        if(g.first_frame) {
            size = DC_IN_BUF_SIZE;
            in_ptr = pdc->data.in_buf;
            bytesLeft = 0;
            f_lseek(&g.f, 0);
        }
        else {
            memmove(pdc->data.in_buf, in_ptr, bytesLeft);
            in_ptr = pdc->data.in_buf + bytesLeft;
            size = DC_IN_BUF_SIZE - bytesLeft;
        }

//        int ret =
        f_read(&g.f, in_ptr, size, &size);
//        printf("%d\n", ret);

        if(f_eof(&g.f)) {
            zplay_stop();
            printf("Playback ends. #1\n");
            return;
        }

        if(size <= 0) {
            zplay_stop();
            printf("Playback ends. #2\n");
            return;
        }

        g.offset = MP3FindSyncWord(pdc->data.in_buf, DC_IN_BUF_SIZE);
        if(g.offset < 0) {
            continue;    // frame start not found, read another frame.
        }
        else {
            int ret = MP3GetNextFrameInfo(g.decoder, &g.frame_info,
                pdc->data.in_buf + g.offset);
            if(ret == ERR_MP3_INVALID_FRAMEHEADER) {
                continue;    // sync word found, but not the start of a frame
            }
            else {
                printf("$$$ %d\n", ret);
                if(g.first_frame) {

                    printf("%d %d %d\n", g.frame_info.samprate,
                        g.frame_info.outputSamps, g.frame_info.nChans);
                    int samprate = g.frame_info.samprate;
                    int outputSamps = g.frame_info.outputSamps;
                    int nchans = g.frame_info.nChans;

                    if(nchans == 1)
                        samprate /= 2;
                    i2s_adjust(samprate, outputSamps * 4);
                    g.first_frame = 0;
                }
                break;
            }
        }
    }

    // Decode a frame, assuming that sync word was already found.
    in_ptr = pdc->data.in_buf + g.offset;
    bytesLeft = DC_IN_BUF_SIZE - g.offset;
    int err = MP3Decode(g.decoder, &in_ptr, &bytesLeft, out_buf, 0);
    if(err < 0) {
        printf("### %d %d %p \n", err, bytesLeft, in_ptr);
    }
    switch(err) {
        case ERR_MP3_INDATA_UNDERFLOW:
            zplay_stop();
            return;
        case ERR_MP3_MAINDATA_UNDERFLOW:
            //                ReadMoreMp3Data();
            break;
        case ERR_MP3_FREE_BITRATE_SYNC:
            zplay_stop();
            return;
        default:
//            zplay_stop();
            return;
    }
}

void play_mp3_init(const char* fname)
{
    DataConfig_t* pdc = DC_Get();

    g.fname = fname;
    zplay_cfg_t cfg;
    cfg.buf = pdc->data.out_buf;
    cfg.buf_size = 0;    //g.synth.pcm.length * 2 * 2;    //1152 * 2 * 2;
    cfg.dma_write_start_f = I2S_Write_DMA;
    cfg.dma_write_stop_f = I2S_Write_DMA_Stop;
    cfg.open_f = open_f;
    cfg.close_f = close_f;
    cfg.read_f = read_f;
    zplay_init(&cfg);
}
