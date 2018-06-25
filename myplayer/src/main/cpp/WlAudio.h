//
// Created by hgb on 18-6-10.
//

#ifndef MYMUSIC_WLAUDIO_H
#define MYMUSIC_WLAUDIO_H


extern "C"
{
#include <libavcodec/avcodec.h>
};

class WlAudio {
public:

    AVCodecParameters *codecpar = NULL;
    int streamIndex = -1;
    AVCodecContext *avCodecContext = NULL;

public:
    WlAudio();
    ~WlAudio();
};


#endif //MYMUSIC_WLAUDIO_H
