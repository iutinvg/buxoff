#include <iostream>

#include <jni.h>
#include <android/log.h>

#include <leveldb/options.h>
#include <leveldb/db.h>

#include "Storage.h"
#include "Record.h"


#define  LOG_TAG    "buxoff"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


using namespace Buxoff;

static Storage storage;

extern "C" {
    JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_init(JNIEnv *, jobject, jstring);
    JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_add(JNIEnv *, jobject);
    JNIEXPORT jint JNICALL Java_com_sevencrayons_buxoff_Buxoff_count(JNIEnv *, jobject);
};

JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_init(JNIEnv *env, jobject obj, jstring fn) {
    const char *filename = env->GetStringUTFChars(fn, 0);
    LOGE("full path: %s", filename);
    LOGE("full path: %d", 10);

    // int* p = nullptr;
    // delete p;

    storage = Storage(filename);
    LOGE("open status: %s", storage.last_status.ToString().c_str());

    env->ReleaseStringUTFChars(fn, filename);
}

JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_add(JNIEnv *env, jobject obj) {
    storage.put(Record());
}

JNIEXPORT jint JNICALL Java_com_sevencrayons_buxoff_Buxoff_count(JNIEnv *env, jobject obj) {
    // storage.put(Storage::random_key(), "val");
    return storage.get_total_count();
}
