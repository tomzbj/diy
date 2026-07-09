#ifndef _DECODER_H
#define _IDECODER_H

#include "audio_types.h"
#include "ff.h"
#include "zuart4.h"

class IDECODER {
  public:

    enum RESULT {
      SUCCESS = 0, FAILED = -1
    };

    IDECODER(uint8_t* common_buf, int buf_size) :
      _common_buf(common_buf), _buf_size(buf_size)
    {
    }
    virtual RESULT open(const char* path);
    virtual void close(void);
    virtual bool running(void);
    virtual uint32_t get_max_frame_size(void) = 0;
    virtual RESULT decode_frame(AUDIO_SYS::FRAME& frame) = 0;
    virtual AUDIO_SYS::FILE_INFO get_info(void);

  private:
    bool _running;

  protected:
    FIL _file;
    AUDIO_SYS::FILE_INFO _info;
    uint8_t* _common_buf;
    int _buf_size;
};

class FLAC_DECODER: public IDECODER {
  public:
    using IDECODER::IDECODER;
//    virtual RESULT open(const char* path);
    uint32_t get_max_frame_size(void)
    {
      return 8192;
    }
    RESULT decode_frame(AUDIO_SYS::FRAME& frame)
    {
      _dbg();
      return SUCCESS;
    }
  private:
};

#endif
