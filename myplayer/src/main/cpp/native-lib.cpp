#include <jni.h>
#include <string>
#include "android/log.h"

#include "WlCallJAva.h"
#include "WlFFmpeg.h"
extern "C"
{
#include <libavformat/avformat.h>

}

JavaVM *javaVM = NULL;
WlCallJAva *callJAva = NULL;
WlFFmpeg *fFmpeg = NULL;
WlPlaystatus *playstatus = NULL;

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    jint result = -1;
    javaVM = vm;
    JNIEnv *env;
    if(vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK)
    {
        return result;
    }
    return JNI_VERSION_1_4;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_myplayer_palyer_WlPlay_n_1parpared(JNIEnv *env, jobject instance, jstring source_) {
    const char *source = env->GetStringUTFChars(source_, 0);

    // TODO
    if(fFmpeg == NULL )
    {
        if(callJAva == NULL)
        {
            callJAva = new WlCallJAva(javaVM, env, &instance);
        }
        fFmpeg = new WlFFmpeg(playstatus, callJAva, source);
        fFmpeg->parpared();
        playstatus = new WlPlaystatus();
    }

    env->ReleaseStringUTFChars(source_, source);
}extern "C"
JNIEXPORT void JNICALL
Java_com_myplayer_palyer_WlPlay_n_1start(JNIEnv *env, jobject instance) {

    // TODO
    if(fFmpeg != NULL) {
        LOGD("Java_com_myplayer_palyer_WlPlay_n_1start.....");
        fFmpeg->start();
    }

}