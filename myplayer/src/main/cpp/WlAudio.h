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
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
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


    int ret = -1;
    uint8_t *buffer = NULL;
    int data_size = 0;

    //接口引擎
    SLObjectItf engineObject = NULL;
    SLEngineItf  engineEngine = NULL;

    //混音器
    SLObjectItf  outputMixObject = NULL;
    SLEnvironmentalReverbItf  outputMixEnvironmentalReverb = NULL;
    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    //pcm
    SLObjectItf  pcmPlayerObject = NULL;
    SLPlayItf  pclPlayerplay;

    //缓冲器队列接口
    SLAndroidSimpleBufferQueueItf pcmBufferQueue;


    FILE *pcmFile;
    void *openslesbuffer;
    uint8_t  *out_buff;
public:
    WlAudio(WlPlaystatus *playstatus);
    ~WlAudio();

    void play();
    int resampleAudio();

    void initOpenSLES();
};


#endif //MYMUSIC_WLAUDIO_H
