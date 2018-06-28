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

    jmethodID  jmid_load;

public:
    WlCallJAva(JavaVM *javaVM, JNIEnv *env, jobject *obj);
    ~WlCallJAva();

    void onCallParpare(int type);

    void onCallLoad(int type, bool load);

};


#endif //MYMUSIC_WLCALLJAVA_H
