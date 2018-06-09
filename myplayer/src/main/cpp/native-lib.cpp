#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring

JNICALL
Java_com_myplayer_Demo_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "First Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
