//
// Created by hgb on 18-6-10.
//

#include "WlCallJAva.h"

WlCallJAva::WlCallJAva(JavaVM *javaVM, JNIEnv *env, jobject *obj) {
    this->javaVM = javaVM;
    this->jniEnv = env;
    this->jobj = env->NewGlobalRef(*obj);

    jclass  jlz = jniEnv->GetObjectClass(jobj);
    if(!jlz)
    {
        if(LOG_DEBUG)
        {
            LOGD("Get jclass wring")
        }
        return ;
    }

    jmid_parpared = env->GetMethodID(jlz, "onCallParpare","()V");


}

WlCallJAva::~WlCallJAva() {

}

void WlCallJAva::onCallParpare(int type) {
    if(type == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj, jmid_parpared);
    } else if (type == CHILD_THREAD) {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
           if(LOG_DEBUG)
           {
               LOGD("Get child jniEnv wrong");
               return;

           }
        }
        jniEnv->CallVoidMethod(jobj, jmid_parpared);
        javaVM->DetachCurrentThread();
    }


}