#include "player.h"
#include "platform.h"
#include "fs_lock.h"

enum class DECODE_PUT {
  OK,
  NO_SPACE,
  NEED_DATA,
  FAILED
};

static DECODE_PUT decode_one_frame(IDECODER* decoder, ZFIFO& fifo, AUDIO_SYS::FRAME& frame)
{
  uint32_t required_size = decoder->get_max_frame_size();
  if(fifo.size() - fifo.available() < required_size)
    return DECODE_PUT::NO_SPACE;

  if(decoder->decode_frame(frame) != IDECODER::SUCCESS)
    return DECODE_PUT::FAILED;
  if(frame.size == 0)
    return DECODE_PUT::NEED_DATA;

  if(frame.file_type == AUDIO_SYS::FILE_TYPE::TXT) {
    extern class ZUART4 u1;
    u1.write(frame.data, frame.size);
  }
  else {
    fifo.put(frame.data, frame.size);
  }
  return DECODE_PUT::OK;
}

void PLAYER::poll(void)
{
  if(!fs_playback_allowed())
    return;
  if(_current_decoder == nullptr)
    return;

  const uint32_t t0 = DWT->CYCCNT;
  const uint32_t budget = (SystemCoreClock * 10) / 1000;

  while((DWT->CYCCNT - t0) < budget) {
    AUDIO_SYS::FRAME frame;
    switch(decode_one_frame(_current_decoder, _pcm_fifo, frame)) {
      case DECODE_PUT::OK:
        continue;
      case DECODE_PUT::FAILED:
        if(_cbs.post_play_f != nullptr)
          _cbs.post_play_f(nullptr);
        _current_decoder->close();
        _current_decoder = nullptr;
        return;
      case DECODE_PUT::NO_SPACE:
      case DECODE_PUT::NEED_DATA:
      default:
        return;
    }
  }
}

bool PLAYER::set_file(const char* path)
{
  if(!fs_playback_allowed()) {
    printf("playback blocked while MTP transfer is in progress.\n");
    return false;
  }

  if(_current_decoder != nullptr) {
    _current_decoder->close();
    _current_decoder = nullptr;
  }

  _current_decoder = _decoder_manager.get_decoder(path);

  if(_current_decoder == nullptr) {
    printf("failed to get appropriate decoder for file %s!\n", path);
    return false;
  }
  if(_current_decoder->open(path) == IDECODER::FAILED) {
    printf("failed to open file %s!\n", path);
    _current_decoder = nullptr;
    return false;
  }
  auto info = _current_decoder->get_info();
  auto sample_rate = info.sample_rate * info.channels / 2;
  _cbs.set_sample_rate_f((void*)sample_rate);
  printf("%d %d %lu\n", info.bit_depth, info.channels, info.sample_rate);
  _pcm_fifo.clear();

  const uint32_t t0 = DWT->CYCCNT;
  const uint32_t budget = SystemCoreClock / 10;
  while((DWT->CYCCNT - t0) < budget) {
    AUDIO_SYS::FRAME frame;
    if(decode_one_frame(_current_decoder, _pcm_fifo, frame) != DECODE_PUT::OK)
      break;
  }

  if(_cbs.pre_play_f != nullptr)
    _cbs.pre_play_f((void*)path);

  return true;
}

void PLAYER::stop(void)
{
  if(_current_decoder != nullptr) {
    _current_decoder->close();
    _current_decoder = nullptr;
  }
  _pcm_fifo.clear();
}

void PLAYER::start(void)
{
}

void PLAYER::pause(void)
{
}

void PLAYER::next(void)
{
}

void PLAYER::prev(void)
{
}
