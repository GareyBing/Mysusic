//
// Created by hgb on 18-6-10.
//

#ifndef MYMUSIC_WLCALLJAVA_H
#define MYMUSIC_WLCALLJAVA_H

#include "AndroidLog.h"
#include <cwchar>
#include "jni.h"
#define MAIN_THREAD 0
#define CHILD_THREAD 1

class WlCallJAva {

public:
    JavaVM *javaVM = NULL;
    JNIEnv *jniEnv = NULL;
    jobject jobj;

    jmethodID jmid_parpared;

public:
    WlCallJAva(JavaVM *javaVM, JNIEnv *env, jobject *obj);
    ~WlCallJAva();

    void onCallParpare(int type);

};


#endif //MYMUSIC_WLCALLJAVA_H
