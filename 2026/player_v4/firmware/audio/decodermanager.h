#ifndef _DECODER_MANAGER_H
#define _DECODER_MANAGER_H

#include "idecoder.h"
#include "audio_types.h"
#include "txt_decoder.h"
#include "wav_decoder.h"
#include "mp3_decoder.h"

class DECODER_MANAGER {
  public:

    DECODER_MANAGER(void) :
      _wav_decoder(_common_buf, sizeof(_common_buf)),    //
      _mp3_decoder(_common_buf, sizeof(_common_buf)),    //
      _txt_decoder(_common_buf, sizeof(_common_buf)),    //
      _flac_decoder(_common_buf, sizeof(_common_buf))
    {
    }

    IDECODER* get_decoder(const char* path);

  private:

    WAV_DECODER _wav_decoder;
    MP3_DECODER _mp3_decoder;
    TXT_DECODER _txt_decoder;
    FLAC_DECODER _flac_decoder;

    bool _running = false;
    AUDIO_SYS::FILE_TYPE detect_file_type(const char* path);
    uint8_t _common_buf[65536];
};
#endif
