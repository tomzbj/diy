#include "idecoder.h"
#include "wav_decoder.h"
#include "ff.h"
#include "fs_lock.h"

IDECODER::RESULT WAV_DECODER::decode_frame(AUDIO_SYS::FRAME& frame)
{
  FS_LOCK lock;
  if(f_eof(&_file))
    return FAILED;

  unsigned int len = 0;

  f_read(&_file, _frame, 512, &len);
  frame.channels = _info.channels;
  frame.data = _frame;
  frame.sample_rate = _info.sample_rate;
  frame.size = len;
  frame.file_type = AUDIO_SYS::FILE_TYPE::WAV;
  return SUCCESS;
}

IDECODER::RESULT WAV_DECODER::open(const char* path)
{
  {
    FS_LOCK lock;
    if(IDECODER::open(path) == FAILED)
      return FAILED;
  }

  FS_LOCK lock;
  uint8_t buf[256] = {0};
  unsigned int len = 0;

  f_rewind(&_file);
  f_read(&_file, buf, 256, &len);
  for(int i = 0; i < 256; i++) {
    if(memcmp(&buf[i], "RIFF", 4) == 0)
      printf("RIFF_SIZE: %lu\n", *(uint32_t*)&buf[i + 4]);
    if(memcmp(&buf[i], "fmt ", 4) == 0)
      printf("FMT_SIZE : %lu\n", *(uint32_t*)&buf[i + 4]);
    if(memcmp(&buf[i], "fmt ", 4) == 0) {
      _info.channels = *(uint16_t*)&buf[i + 10];
      _info.sample_rate = *(uint16_t*)&buf[i + 12];
      _info.bit_depth = *(uint16_t*)&buf[i + 22];
      printf("Channels: %u\n", _info.channels);
      printf("Sample Rate: %lu\n", _info.sample_rate);
    }
    if(memcmp(&buf[i], "data", 4) == 0) {
      f_lseek(&_file, i + 4);
      break;
    }
  }

  return SUCCESS;
}
