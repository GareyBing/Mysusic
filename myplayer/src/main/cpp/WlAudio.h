//
// Created by hgb on 18-6-10.
//

#ifndef MYMUSIC_WLAUDIO_H
#define MYMUSIC_WLAUDIO_H

#include "WLQueue.h"
#include "WlPlaystatus.h"
extern "C"
{
#include <libavcodec/avcodec.h>
#include "libswresample/swresample.h"
};

class WlAudio {
public:

    AVCodecParameters *codecpar = NULL;
    int streamIndex = -1;
    AVCodecContext *avCodecContext = NULL;
    WLQueue *queue = NULL;
    WlPlaystatus *playstatus = NULL;
    AVPacket *avPacket = NULL;
    AVFrame *avFrame = NULL;

    pthread_t thread_play;

    int resampleAudio();
    int ret = -1;
    uint8_t *buffer = NULL;
    int data_size = 0;

public:
    WlAudio(WlPlaystatus *playstatus);
    ~WlAudio();

    void play();
};


#endif //MYMUSIC_WLAUDIO_H
