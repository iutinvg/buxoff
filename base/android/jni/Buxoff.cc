#include <iostream>

#include <jni.h>

#include <leveldb/options.h>
#include <leveldb/db.h>

#include "Storage.h"
#include "Record.h"
#include "Validation.h"
#include "Email.h"

#include "jutils.h"


using namespace Buxoff;

static Storage *storage;

extern "C" {
    JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_init(JNIEnv *, jobject, jstring);
    JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_add(JNIEnv *, jobject, jstring amount, jstring desc, jstring tag, jstring account);
    JNIEXPORT jint JNICALL Java_com_sevencrayons_buxoff_Buxoff_count(JNIEnv *, jobject);
    JNIEXPORT jstring JNICALL Java_com_sevencrayons_buxoff_Buxoff_emailSubject(JNIEnv *, jobject);
    JNIEXPORT jstring JNICALL Java_com_sevencrayons_buxoff_Buxoff_emailBody(JNIEnv *, jobject);
};

JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_init(JNIEnv *env, jobject obj, jstring fn) {
    JStr filename{env, fn};
    LOGE("full path: %s", filename.c_str());
    LOGE("lib version: %d", 17);

    storage = new Storage(filename);
    LOGE("open status: %s", storage->last_status.ToString().c_str());
}

JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_add(JNIEnv *env, jobject obj,
    jstring amount, jstring desc, jstring tag, jstring account) {
    try {
        Record r{JStr{env, amount}, JStr{env, desc}, {JStr{env, tag}}, JStr{env, account}};
        r.validate();
        storage->put(r);
    } catch (ValidationError& e) {
        throw_java_exception(env, e.what());
    }
}

JNIEXPORT jint JNICALL Java_com_sevencrayons_buxoff_Buxoff_count(JNIEnv *env, jobject obj) {
    return storage->get_total_count();
}

JNIEXPORT jstring JNICALL Java_com_sevencrayons_buxoff_Buxoff_emailSubject(JNIEnv *env, jobject obj) {
    return env->NewStringUTF(Email::subject().c_str());
}

JNIEXPORT jstring JNICALL Java_com_sevencrayons_buxoff_Buxoff_emailBody(JNIEnv *env, jobject obj) {
    Email e{storage->get_records()};
    return env->NewStringUTF(e.body().c_str());
}
