#ifndef _TXT_DECODER_H
#define _TXT_DECODER_H

#include "fs_lock.h"

class TXT_DECODER: public IDECODER {
  public:
    using IDECODER::IDECODER;
//    virtual RESULT open(const char* path);
    uint32_t get_max_frame_size(void)
    {
      return 512;
    }
    RESULT decode_frame(AUDIO_SYS::FRAME& frame)
    {
      unsigned int len = 0;
      {
        FS_LOCK lock;
        if(f_eof(&_file))
          return FAILED;
        f_read(&_file, _frame, 512, &len);
      }
      frame.channels = 0;
      frame.data = _frame;
      frame.sample_rate = 0;
      frame.size = len;
      frame.file_type = AUDIO_SYS::FILE_TYPE::TXT;
      return SUCCESS;
    }
  private:
    uint8_t _frame[512];
};

#endif
