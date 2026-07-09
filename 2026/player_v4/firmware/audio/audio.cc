#include "platform.h"

ZFIFO pcm_fifo(32768);
PLAYER player(pcm_fifo);

static void pre_play_f(void* params)
{
  const char* path = (const char*)params;
  printf("Start playing %s\n", path);
}

static void post_play_f(void* params)
{
  printf("Playback finished.\n");
}

static void set_sample_rate_f(void* params)
{
  uint32_t sample_rate = (uint32_t)params;
  I2S_SetSampleRate(sample_rate);
}

void AUDIO_Config(void)
{
  player.bind_cb(PLAYER::PRE_PLAY_F, pre_play_f);
  player.bind_cb(PLAYER::POST_PLAY_F, post_play_f);
  player.bind_cb(PLAYER::SET_SAMPLE_RATE_F, set_sample_rate_f);

  print_log();
}
