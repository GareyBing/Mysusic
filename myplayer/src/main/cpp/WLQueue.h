//
// Created by hgb on 18-6-25.
//

#ifndef MYMUSIC_WLQUEUE_H
#define MYMUSIC_WLQUEUE_H

#include "queue"
#include "pthread.h"
#include "WlPlaystatus.h"
#include "AndroidLog.h"

extern "C"
{
#include "libavcodec/avcodec.h"
};

class WLQueue {

public:
    std::queue<AVPacket *> queuePacket;
    pthread_mutex_t mutexPacket;
    pthread_cond_t condPacket;
    WlPlaystatus *playstatus = NULL;
public:
    WLQueue(WlPlaystatus *playstatus);
    ~WLQueue();

    int putAcpacket(AVPacket *packet);
    int getAvpacket(AVPacket *packet);
    int getQueueSize();
};


#endif //MYMUSIC_WLQUEUE_H
