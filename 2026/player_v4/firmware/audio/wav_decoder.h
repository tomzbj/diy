#ifndef _WAV_DECODER_H
#define _WAV_DECODER_H

//#include "idecoder.h"

class WAV_DECODER: public IDECODER {
  public:
    using IDECODER::IDECODER;
    uint32_t get_max_frame_size(void)
    {
      return 512;
    }

    RESULT decode_frame(AUDIO_SYS::FRAME& frame);

    RESULT open(const char* path);
  private:
    uint8_t _frame[512];
};

#endif
