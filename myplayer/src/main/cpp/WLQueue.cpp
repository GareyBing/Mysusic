//
// Created by hgb on 18-6-25.
//

#include "WLQueue.h"

WLQueue::WLQueue(WlPlaystatus *playstatus) {
    this->playstatus = playstatus;
    pthread_mutex_init(&mutexPacket,NULL);
    pthread_cond_init(&condPacket, NULL);
}

WLQueue::~WLQueue() {
   pthread_mutex_destroy(&mutexPacket);
    pthread_cond_destroy(&condPacket);
}

int WLQueue::putAcpacket(AVPacket *packet) {

    pthread_mutex_lock(&mutexPacket);
    queuePacket.push(packet);
    if(LOG_DEBUG)
    {
        LOGD("放入一个AVpacket 到队列里， 个数为 %d", queuePacket.size());
    }
    pthread_cond_signal(&condPacket);


    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

int WLQueue::getAvpacket(AVPacket *packet) {
    pthread_mutex_lock(&mutexPacket);

    while(playstatus != NULL && !playstatus->exit)
    {
        if(queuePacket.size() > 0)
        {
            AVPacket *avPacket = queuePacket.front();
            if(av_packet_ref(packet, avPacket) == 0)
            {
                queuePacket.pop();
            }
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            if(LOG_DEBUG)
            {
                LOGD("从队列里拿出一个avPacket, 还剩 %d 个", queuePacket. size());
            }
            break;

        } else{
            pthread_cond_wait(&condPacket, &mutexPacket);

        }
    }
    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

int WLQueue::getQueueSize() {
    int size = 0;
    pthread_mutex_lock(&mutexPacket);
    size = queuePacket.size();

    pthread_mutex_unlock(&mutexPacket);

    return 0;
}
