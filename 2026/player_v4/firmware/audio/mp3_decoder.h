#ifndef _MP3_DECODER_H
#define _MP3_DECODER_H

#define MINIMP3_NO_SIMD
#include "minimp3.h"

class MP3_DECODER: public IDECODER {
  public:
    using IDECODER::IDECODER;
    uint32_t get_max_frame_size(void)
    {
      return MINIMP3_MAX_SAMPLES_PER_FRAME * sizeof(int16_t);
    }

    RESULT decode_frame(AUDIO_SYS::FRAME& frame);
    RESULT open(const char* path);

  private:
//    uint8_t _frame[MINIMP3_MAX_SAMPLES_PER_FRAME];
    int16_t _frame[MINIMP3_MAX_SAMPLES_PER_FRAME];
    mp3dec_t _mp3d;

    int _nbuf;
};

#endif
