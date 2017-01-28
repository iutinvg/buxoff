#include <iostream>

#include <jni.h>
#include <android/log.h>

#include <leveldb/options.h>
#include <leveldb/db.h>

#include "Storage.h"
#include "Record.h"
#include "Validation.h"

#include "jutils.h"


#define  LOG_TAG    "buxoff"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


using namespace Buxoff;

static Storage *storage;

extern "C" {
    JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_init(JNIEnv *, jobject, jstring);
    JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_add(JNIEnv *, jobject, jstring amount, jstring desc, jstring tag, jstring account);
    JNIEXPORT jint JNICALL Java_com_sevencrayons_buxoff_Buxoff_count(JNIEnv *, jobject);
    JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_exc(JNIEnv *, jobject);
};

JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_init(JNIEnv *env, jobject obj, jstring fn) {
    // const char *filename = env->GetStringUTFChars(fn, 0);
    // ...filename usage
    // env->ReleaseStringUTFChars(fn, filename);

    JStr filename{env, fn};

    LOGE("full path: %s", filename.c_str());
    LOGE("lib version: %d", 13);

    storage = new Storage(filename);
    LOGE("open status: %s", storage->last_status.ToString().c_str());
}

JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_add(JNIEnv *env, jobject obj,
    jstring amount, jstring desc, jstring tag, jstring account) {
    Record r{JStr{env, amount}, JStr{env, desc}, {JStr{env, tag}}, JStr{env, account}};
    storage->put(r);
}

JNIEXPORT jint JNICALL Java_com_sevencrayons_buxoff_Buxoff_count(JNIEnv *env, jobject obj) {
    return storage->get_total_count();
}

JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_exc(JNIEnv *env, jobject obj) {
    try {
        Record r{"", "", {}, ""};
        r.validate();
    } catch (ValidationError& e) {
        throw_java_exception(env, e.what());
    }
}
