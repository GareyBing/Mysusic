//
// Created by hgb on 18-6-10.
//


#include "WlFFmpeg.h"

WlFFmpeg::WlFFmpeg(WlPlaystatus *playstatus,WlCallJAva *callJAva, const char *url) {
    this->callJAva = callJAva;
    this->url= url;
    this->playstatus = playstatus;
}

void *decodeFFmpeg(void *data)
{
    WlFFmpeg *wlFFmpeg = (WlFFmpeg *)(data);
    wlFFmpeg->decodeFFmpegThread();
    pthread_exit(&wlFFmpeg->decodeThread);
}
void WlFFmpeg::parpared() {
    pthread_create(&decodeThread, NULL, decodeFFmpeg,this);

}

void WlFFmpeg::decodeFFmpegThread() {
    LOGD("call decodeFFmpegThread");


    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();
    LOGD("url =  %s", url);
    if(avformat_open_input(&pFormatCtx, url, NULL, NULL) != 0)
    {
        if(LOG_DEBUG)
        {
            LOGD("can not open url : %s", url);
        }
        return;
    }

    if(avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
        if(LOG_DEBUG)
        {
            LOGD("can not find streams from  : %s", url);
        }
        return;
    }

    int stream = pFormatCtx->nb_streams;
    LOGD("pFormatCtx->nb_streams****............ = %d", stream);
    for (int i = 0;  i< stream; i++)
    {
        if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            if(audio == NULL)
            {
                audio = new WlAudio(playstatus, pFormatCtx->streams[i]->codecpar->sample_rate, callJAva);
                audio->streamIndex = i;
                audio->codecpar = pFormatCtx->streams[i]->codecpar;
                audio->duration = pFormatCtx->duration / AV_TIME_BASE;//总时长
                audio->time_base = pFormatCtx->streams[i]->time_base;//流里每一帧持续时间分数表达式

            }
        }
    }

    AVCodec *dec = avcodec_find_decoder(audio->codecpar->codec_id);
    if(!dec)
    {
        if(LOG_DEBUG)
        {
            LOGD("can not find decoder");

        }
        return;
    }
    audio->avCodecContext = avcodec_alloc_context3(dec);
    if(!audio->avCodecContext)
    {
        if(LOG_DEBUG)
        {
            LOGD("can not allow new decoderctx");
        }
        return;
    }

    if(avcodec_parameters_to_context(audio->avCodecContext, audio->codecpar) < 0)
    {
        if(!audio->avCodecContext)
        {
            if(LOG_DEBUG)
            {
                LOGD("can not fill decoderctx");

            }
            return;
        }
    }
    if(avcodec_open2(audio->avCodecContext, dec, 0) != 0)
    {
        if(LOG_DEBUG)
        {
            LOGD("can not open stream");

        }
            return;
    }
        callJAva->onCallParpare(CHILD_THREAD);
}

void WlFFmpeg::start() {
    if(audio == NULL)
    {
        if(LOG_DEBUG)
        {
            LOGD("audio is null");
        }
        return;
    }
    LOGD("WlFFmpeg::start().....");

    audio->play();

    int count = 0;
    while (playstatus != NULL && !playstatus->exit)
    {
        AVPacket *avPacket= av_packet_alloc();
        int frame = av_read_frame(pFormatCtx, avPacket);
        LOGD("av_read_frame ");
        if(frame == 0)
        {   LOGD("av_read_frame  = %d",frame);
            LOGD("avPacket->stream_index = %d",avPacket->stream_index);
            LOGD("audio->streamIndex = %d",audio->streamIndex);
            if(avPacket->stream_index == audio->streamIndex)
            {
                count++;
                if(LOG_DEBUG)
                {
                    LOGD("解码第 %d 帧", count)
                }
//                av_packet_free(&avPacket);
//                av_free(avPacket);
//                avPacket = NULL;
                audio->queue->putAcpacket(avPacket);
                break;
            } else {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
                break;
            }
        } else {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;

            while (playstatus != NULL && !playstatus->exit)
            {
                if(audio->queue->getQueueSize() > 0 )
                {
                    continue;
                } else{
                    playstatus->exit = true;
                }
            }
        }
    } 

//    while(audio->queue->getQueueSize() > 0)
//    {
//        AVPacket *avPacket = av_packet_alloc();
//        audio->queue->getAvpacket(avPacket);
//        av_packet_free(&avPacket);
//        av_free(avPacket);
//
//        avPacket = NULL;
//
//    }

    if(LOG_DEBUG)
    {
        LOGD("解码完成");
    }

}

void WlFFmpeg::pause() {
    if(audio != NULL)
    {
        audio->pause();
    }

}

void WlFFmpeg::resume() {
    if(audio != NULL)
    {
        audio->resume();
    }

}



