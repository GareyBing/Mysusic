//
// Created by hgb on 18-6-10.
//

#include "WlAudio.h"

WlAudio::WlAudio(WlPlaystatus *playstatus, int sample_rate, WlCallJAva *callJAva) {
    this->callJAva = callJAva;
    this->playstatus = playstatus;
    this->sample_rate = sample_rate;
    queue = new WLQueue(playstatus);
    buffer = (uint8_t *)(av_malloc(sample_rate * 2 * 2));

}

WlAudio::~WlAudio() {

}

void *decodPlay(void *data){
    WlAudio *wlAudio = (WlAudio *)(data);

    wlAudio->initOpenSLES();

    pthread_exit(&wlAudio->thread_play);
}

void WlAudio::play() {
    pthread_create(&thread_play, NULL,decodPlay , NULL);
}

//FILE *outFile= fopen("/mnt/sdcard/music.pcm", "w");

int WlAudio::resampleAudio() {//重采样,

    while(playstatus != NULL && !playstatus)
    {
        if(queue->getQueueSize() == 0)
        {
            if(playstatus->load)
            {
                playstatus->load = true;
                callJAva->onCallLoad(CHILD_THREAD, true);
            }
            continue;
        } else{
            if(playstatus->load)
            {
                callJAva->onCallLoad(CHILD_THREAD, false);
            }
        }


        avPacket = av_packet_alloc();
        if(queue->getAvpacket(avPacket) != 0)
        {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket=NULL;
            continue;
        }

       ret =  avcodec_send_packet(avCodecContext, avPacket);//将packet放到解码器里解码
        if(ret != 0)
        {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket=NULL;
            continue;
        }

        avFrame = av_frame_alloc();
        ret=  avcodec_receive_frame(avCodecContext, avFrame);
        if(ret ==0)
        {
            if(avFrame->channels > 0 &&  avFrame->channel_layout == 0)
            {
                avFrame->channel_layout = av_get_default_channel_layout(avFrame->channels);
            } else if(avFrame->channels == 0 && avFrame->channel_layout > 0) {
                avFrame->channels = av_get_channel_layout_nb_channels(avFrame->channel_layout);
            }

            SwrContext *swr_ctx;//重采样上下文
            swr_ctx = swr_alloc_set_opts(
            NULL,
            AV_CH_LAYOUT_STEREO,//立体声
            AV_SAMPLE_FMT_S16,
            avFrame->sample_rate,
            avFrame->channel_layout,
            static_cast<AVSampleFormat > (avFrame->format),
            avFrame->sample_rate,
            NULL,
            NULL
            );

            if(!swr_ctx || swr_init(swr_ctx) < 0)
            {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket=NULL;

                av_frame_free(&avFrame);
                av_free(avFrame);
                avFrame=NULL;

                if(swr_ctx != NULL)
                {
                    swr_free(&swr_ctx);
                    swr_ctx = NULL;
                }
                continue;
            }

            int nb = swr_convert(//采样个数
                    swr_ctx,
                    &buffer,
                    avFrame->nb_samples,
                    (const uint8_t **)(avFrame->data),
                    avFrame->nb_samples
            );

            //
            int out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
            data_size = nb * out_channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

            now_time = avFrame->pts * av_q2d(time_base);//当前时间
            if(now_time < clock)
            {
                now_time = clock;
            }
            clock = now_time;

          //  fwrite(buffer, 1, data_size, outFile);
            if(LOG_DEBUG )
            {
                LOGD("data_size  = ",data_size);
            }

            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket=NULL;

            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame=NULL;
            swr_free(&swr_ctx);
            swr_ctx = NULL;
            break;

        } else{
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket=NULL;

            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame=NULL;
            continue;
        }

    }
    return data_size;
}

void pcmBufferCallBack(SLAndroidSimpleBufferQueueItf bf, void *context)
{
    WlAudio *wlAudio = (WlAudio *)(context);
    if(wlAudio != NULL)
    {
        int buffersize  = wlAudio->resampleAudio();//转码
        if(buffersize > 0)
        {
            wlAudio->clock += buffersize / ((double)wlAudio->sample_rate *2 *2);

            if(wlAudio->clock - wlAudio->last_time > 0.1)
            {
                wlAudio->last_time = wlAudio->clock;
            }

            wlAudio->callJAva->onCallTimeInfo(CHILD_THREAD, wlAudio->clock, wlAudio->duration);

            (*wlAudio->pcmBufferQueue)->Enqueue(wlAudio->pcmBufferQueue,
                                                wlAudio->buffer, buffersize);
        }
    }
}

void WlAudio::initOpenSLES() {

    //创建引擎
    SLresult result;
    result =  slCreateEngine(&engineObject, 0, 0, 0, 0, 0);//create
    result =  (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);// get interance
    result =  (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);

    //创建混音器
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);

    (void) result;
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);

    (void) result;
    result=  (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                              &outputMixEnvironmentalReverb);

    if(SL_RESULT_SUCCESS == result) {
        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings);
        ( void) result;
    }

    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSink = {&outputMix, 0};


    //配置PCM格式信息
    SLDataLocator_AndroidBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,//pcm 数据
            2,//2个声道
            getCurrentSampleRateOpensles(sample_rate),//44100hz的频率
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,//立体声（前左前右）
            SL_BYTEORDER_LITTLEENDIAN//结束标志
    };

    SLDataSource slDataSource = {&android_queue, &pcm};


    const SLInterfaceID  ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean  req[1] = {SL_BOOLEAN_TRUE};


    (*engineEngine)->CreateAudioPlayer(engineEngine, &pcmPlayerObject, &slDataSource, &audioSink, 1, ids, req);

    //初始化播放器
    (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);

    //得到接口器调用， 获取 player接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pcmPlayerplay);


    //注册回调缓冲区， 获取缓冲队列接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE, &pcmBufferQueue);

    //缓冲接口回调
    (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, pcmBufferCallBack, NULL);

    //  获取播放状态接口
    (*pcmPlayerplay)->SetPlayState(pcmPlayerplay, SL_PLAYSTATE_PLAYING);
    pcmBufferCallBack(pcmBufferQueue, this);

}

int WlAudio::getCurrentSampleRateOpensles(int sample_rate) {
    int rate = 0;
    switch (sample_rate)
    {
        case 8000:
            rate = SL_SAMPLINGRATE_8;
            break;
        case 11025:
            rate = SL_SAMPLINGRATE_11_025;
            break;
        case 12000:
            rate = SL_SAMPLINGRATE_12;
            break;
        case 16000:
            rate = SL_SAMPLINGRATE_16;
            break;
        case 22050:
            rate = SL_SAMPLINGRATE_22_05;
            break;
        case 24000:
            rate = SL_SAMPLINGRATE_24;
            break;
        case 32000:
            rate = SL_SAMPLINGRATE_32;
            break;
        case 44100:
            rate = SL_SAMPLINGRATE_44_1;
            break;
        case 48000:
            rate = SL_SAMPLINGRATE_48;
            break;
        case 64000:
            rate = SL_SAMPLINGRATE_64;
            break;
        case 88200:
            rate = SL_SAMPLINGRATE_88_2;
            break;
        case 96000:
            rate = SL_SAMPLINGRATE_96;
            break;
        case 192000:
            rate = SL_SAMPLINGRATE_192;
            break;
        default:
            rate = SL_SAMPLINGRATE_44_1;
    }

    return rate;
}

void WlAudio::pause() {
    if(pcmPlayerObject != NULL)
    {
        (*pcmPlayerplay)->SetPlayState(pcmPlayerplay, SL_PLAYSTATE_PAUSED);
    }


}

void WlAudio::resume() {
    if(pcmPlayerObject != NULL)
    {
        (*pcmPlayerplay)->SetPlayState(pcmPlayerplay, SL_PLAYSTATE_PLAYING);
    }
}
