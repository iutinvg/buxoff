#include <cassert>
#include <iostream>

#include <jni.h>

#include <leveldb/options.h>
#include <leveldb/db.h>

#include "Storage.h"
#include "Record.h"
#include "Validation.h"
#include "Email.h"
#include "ControllerHelpers.h"
#include "ViewHelpers.h"

#include "jutils.h"


using namespace Buxoff;

static Storage *storage = nullptr;

extern "C" {
    JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_init(JNIEnv *, jobject, jstring);
    JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_add(JNIEnv *, jobject, jstring amount, jstring desc, jstring tag, jstring account);

    JNIEXPORT jstring JNICALL Java_com_sevencrayons_buxoff_Buxoff_push(JNIEnv *, jobject, jstring amount, jstring desc, jstring tag, jstring account);
    JNIEXPORT jstring JNICALL Java_com_sevencrayons_buxoff_Buxoff_subject(JNIEnv *, jobject);

    JNIEXPORT jint JNICALL Java_com_sevencrayons_buxoff_Buxoff_count(JNIEnv *, jobject);

    JNIEXPORT jboolean JNICALL Java_com_sevencrayons_buxoff_Buxoff_enableAdd(JNIEnv *, jobject, jstring amount, jstring account);
    JNIEXPORT jboolean JNICALL Java_com_sevencrayons_buxoff_Buxoff_enablePush(JNIEnv *, jobject, jint records_count, jstring amount, jstring account);
};

void JNI_OnUnload(JavaVM *vm, void *reserved) {
    LOGI("close storage");
    delete storage;
    storage = nullptr;
}

JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_init(JNIEnv *env, jobject obj, jstring fn) {
    if (storage) {
        LOGI("storage is already initialised");
        return;
    }

    JStr filename{env, fn};
    LOGI("full path: %s", filename.c_str());
    LOGI("lib version: %f", 20170202.5);
    storage = new Storage(filename);
    LOGI("open status: %s", storage->last_status.ToString().c_str());
}

JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_add(JNIEnv *env, jobject obj,
    jstring amount, jstring desc, jstring tag, jstring account) {
    Record r{JStr{env, amount}, JStr{env, desc}, {JStr{env, tag}}, JStr{env, account}};
    try {
        controller_add(*storage, r);
    } catch (ValidationError& e) {
        throw_java_exception(env, e.what());
    }
}

JNIEXPORT jstring JNICALL Java_com_sevencrayons_buxoff_Buxoff_push(JNIEnv *env, jobject obj,
    jstring amount, jstring desc, jstring tag, jstring account) {
    Record r{JStr{env, amount}, JStr{env, desc}, {JStr{env, tag}}, JStr{env, account}};
    try {
        std::string email_body = controller_push(*storage, r);
        return env->NewStringUTF(email_body.c_str());
    } catch (ValidationError& e) {
        throw_java_exception(env, e.what());
    }
    return nullptr;
}

JNIEXPORT jstring JNICALL Java_com_sevencrayons_buxoff_Buxoff_subject(JNIEnv *env, jobject obj) {
    return env->NewStringUTF(Email::subject().c_str());
}

JNIEXPORT jint JNICALL Java_com_sevencrayons_buxoff_Buxoff_count(JNIEnv *env, jobject obj) {
    return storage->get_records_count();
}

JNIEXPORT jboolean JNICALL Java_com_sevencrayons_buxoff_Buxoff_enableAdd(JNIEnv *env, jobject, jstring amount, jstring account) {
    return view_enable_add(JStr{env, amount}, JStr{env, account});
}

JNIEXPORT jboolean JNICALL Java_com_sevencrayons_buxoff_Buxoff_enablePush(JNIEnv *env, jobject, jint records_count, jstring amount, jstring account) {
    return view_enable_push(records_count, JStr{env, amount}, JStr{env, account});
}
