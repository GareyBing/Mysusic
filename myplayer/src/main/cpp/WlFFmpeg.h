//
// Created by hgb on 18-6-10.
//

#ifndef MYMUSIC_WLFFMPEG_H
#define MYMUSIC_WLFFMPEG_H

#include "WlCallJAva.h"
#include "WlAudio.h"
extern "C" {
#include <libavformat/avformat.h>
};

#include "../../../../../../Android/Ndk/android-ndk-r14b/platforms/android-15/arch-x86/usr/include/pthread.h"

class WlFFmpeg {
public:
    WlCallJAva *callJAva = NULL;
    const char *url = NULL;

    pthread_t decodeThread;
    AVFormatContext *pFormatCtx= NULL;
    WlAudio *audio = NULL;

public:
    WlFFmpeg(WlCallJAva *callJAva, const char *url);
    ~WlFFmpeg();
    void parpared();
    void decodeFFmpegThread();
    void start();

};


#endif //MYMUSIC_WLFFMPEG_H
