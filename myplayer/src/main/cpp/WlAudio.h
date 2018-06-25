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
};

class WlAudio {
public:

    AVCodecParameters *codecpar = NULL;
    int streamIndex = -1;
    AVCodecContext *avCodecContext = NULL;
    WLQueue *queue = NULL;
    WlPlaystatus *playstatus = NULL;

public:
    WlAudio(WlPlaystatus *playstatus);
    ~WlAudio();
};


#endif //MYMUSIC_WLAUDIO_H
