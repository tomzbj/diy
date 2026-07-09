#include "idecoder.h"
#include "mp3_decoder.h"
#include "ff.h"
#include "fs_lock.h"
#include <string.h>

#define MINIMP3_NO_SIMD
#define MINIMP3_IMPLEMENTATION

#include "minimp3.h"

static const int MP3_READ_WATERMARK = 16384;

IDECODER::RESULT MP3_DECODER::decode_frame(AUDIO_SYS::FRAME& frame)
{
  for(int pass = 0; pass < 8; pass++) {
    if(_nbuf == 0) {
      FS_LOCK lock;
      if(f_eof(&_file))
        return FAILED;
    }

    if(_nbuf < MP3_READ_WATERMARK) {
      FS_LOCK lock;
      if(!f_eof(&_file)) {
        unsigned int len = 0;
        if(f_read(&_file, _common_buf + _nbuf, _buf_size - _nbuf, &len) != FR_OK)
          return FAILED;
        _nbuf += len;
      }
    }

    if(_nbuf == 0)
      return FAILED;

    mp3dec_frame_info_t info;
    int samples = mp3dec_decode_frame(&_mp3d, _common_buf, _nbuf, _frame, &info);

    if(info.frame_bytes > 0) {
      _nbuf -= info.frame_bytes;
      memmove(_common_buf, _common_buf + info.frame_bytes, _nbuf);
    }

    if(samples > 0) {
      frame.channels = info.channels;
      frame.data = (uint8_t*)_frame;
      frame.sample_rate = info.hz;
      frame.size = (uint32_t)samples * info.channels * sizeof(int16_t);
      frame.file_type = AUDIO_SYS::FILE_TYPE::MP3;
      return SUCCESS;
    }

    if(info.frame_bytes == 0)
      break;
  }

  {
    FS_LOCK lock;
    if(f_eof(&_file) && _nbuf == 0)
      return FAILED;
  }

  frame.file_type = AUDIO_SYS::FILE_TYPE::MP3;
  frame.size = 0;
  return SUCCESS;
}

IDECODER::RESULT MP3_DECODER::open(const char* path)
{
  {
    FS_LOCK lock;
    if(IDECODER::open(path) == FAILED)
      return FAILED;
  }

  unsigned int len = 0;
  {
    FS_LOCK lock;
    mp3dec_init(&_mp3d);
    f_rewind(&_file);
    f_read(&_file, _common_buf, _buf_size, &len);
  }

  mp3dec_frame_info_t mp3_info;
  mp3dec_decode_frame(&_mp3d, _common_buf, len, NULL, &mp3_info);
  if(mp3_info.hz == 0 || mp3_info.channels == 0) {
    printf("invalid mp3 header\n");
    return FAILED;
  }

  _info.channels = mp3_info.channels;
  _info.sample_rate = mp3_info.hz;
  _info.bit_depth = 16;
  printf("channels: %d sample_rate: %d\n", mp3_info.channels, mp3_info.hz);

  {
    FS_LOCK lock;
    f_rewind(&_file);
    f_read(&_file, _common_buf, _buf_size, &len);
  }
  mp3dec_init(&_mp3d);
  _nbuf = len;

  return SUCCESS;
}
