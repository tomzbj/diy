#ifndef _PLAYER_H
#define _PLAYER_H

#include "decodermanager.h"
#include "zfifo.h"

class PLAYER {
  public:

    struct CBS {
        void (*pre_play_f)(void* params);
        void (*post_play_f)(void* params);
        void (*set_sample_rate_f)(void* params);
    };
    enum CB_ID {
      PRE_PLAY_F, POST_PLAY_F, SET_SAMPLE_RATE_F
    };

    PLAYER(ZFIFO& pcm_fifo) :
      _pcm_fifo(pcm_fifo), _cbs( {0}), _decoder_manager()
    {
    }
//    PLAYER(ZFIFO& pcm_fifo) : _pcm_fifo(pcm_fifo), _decoder_manager() { }

    void bind_cb(CB_ID id, void (*cb)(void* params))
    {
      if(id == PRE_PLAY_F)
        _cbs.pre_play_f = cb;
      else if(id == POST_PLAY_F)
        _cbs.post_play_f = cb;
      else if(id == SET_SAMPLE_RATE_F)
        _cbs.set_sample_rate_f = cb;
    }
    void poll(void);
    bool set_file(const char* path);
    void start(void);
    void pause(void);
    void stop(void);
    void next(void);
    void prev(void);

  private:

    ZFIFO& _pcm_fifo;
    CBS _cbs;
    DECODER_MANAGER _decoder_manager;
    IDECODER* _current_decoder = nullptr;
};

#endif
