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
#include "UserDefaults.h"
#include "TagsStorage.h"

#include "jutils.h"


using namespace Buxoff;

static Connection *connection = nullptr;

extern "C" {
    JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_init(JNIEnv *, jobject, jstring);
    JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_add(JNIEnv *, jobject,
        jstring amount, jstring desc, jstring tag, jstring account);
    JNIEXPORT jstring JNICALL Java_com_sevencrayons_buxoff_Buxoff_push(JNIEnv *, jobject,
        jstring amount, jstring desc, jstring tag, jstring account);
    JNIEXPORT jstring JNICALL Java_com_sevencrayons_buxoff_Buxoff_subject(JNIEnv *, jobject);
    JNIEXPORT jint JNICALL Java_com_sevencrayons_buxoff_Buxoff_count(JNIEnv *, jobject);
    JNIEXPORT jboolean JNICALL Java_com_sevencrayons_buxoff_Buxoff_enableAdd(JNIEnv *, jobject,
        jstring amount, jstring account);
    JNIEXPORT jboolean JNICALL Java_com_sevencrayons_buxoff_Buxoff_enablePush(JNIEnv *, jobject,
        jint records_count, jstring amount, jstring account, jstring email);

    JNIEXPORT jstring JNICALL Java_com_sevencrayons_buxoff_UserDefaults_get(JNIEnv *env, jobject,
        jstring key, jstring def);
    JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_UserDefaults_put(JNIEnv *env, jobject,
        jstring key, jstring val);

    JNIEXPORT jobjectArray JNICALL Java_com_sevencrayons_buxoff_TagsStorage_all(JNIEnv *env, jobject);
};

void JNI_OnUnload(JavaVM *vm, void *reserved) {
    LOGI("close connection");
    delete connection;
    connection = nullptr;
}

JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_init(JNIEnv *env, jobject obj, jstring fn) {
    if (connection) {
        LOGI("storage is already initialised");
        return;
    }

    JStr filename{env, fn};
    LOGI("full path: %s", filename.c_str());
    LOGI("lib version: %f", 20170407.2);
    connection = new Connection(filename);
    LOGI("open status: %s", connection->last_status.ToString().c_str());
}

JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_add(JNIEnv *env, jobject obj,
    jstring amount, jstring desc, jstring tag, jstring account) {
    Record r{JStr{env, amount}, JStr{env, desc}, {JStr{env, tag}}, JStr{env, account}};
    try {
        controller_add(connection, r);
    } catch (ValidationError& e) {
        throw_java_exception(env, e.what());
    }
}

JNIEXPORT jstring JNICALL Java_com_sevencrayons_buxoff_Buxoff_push(JNIEnv *env, jobject obj,
    jstring amount, jstring desc, jstring tag, jstring account) {
    Record r{JStr{env, amount}, JStr{env, desc}, {JStr{env, tag}}, JStr{env, account}};
    try {
        std::string email_body = controller_push(connection, r);
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
    RecordStorage rs(connection);
    return rs.count();
}

JNIEXPORT jboolean JNICALL Java_com_sevencrayons_buxoff_Buxoff_enableAdd(JNIEnv *env, jobject,
    jstring amount, jstring account) {
    return view_enable_add(JStr{env, amount}, JStr{env, account});
}

JNIEXPORT jboolean JNICALL Java_com_sevencrayons_buxoff_Buxoff_enablePush(JNIEnv *env, jobject,
    jint records_count, jstring amount, jstring account, jstring email) {
    return view_enable_push(
        records_count,
        JStr{env, amount},
        JStr{env, account},
        JStr{env, email}
    );
}

// User Defaults
JNIEXPORT jstring JNICALL Java_com_sevencrayons_buxoff_UserDefaults_get(JNIEnv *env, jobject,
    jstring key, jstring def) {
    UserDefaults ud{connection};
    std::string val{ud.get(JStr{env, key}, JStr{env, def})};
    return env->NewStringUTF(val.c_str());
}

JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_UserDefaults_put(JNIEnv *env, jobject,
    jstring key, jstring val) {
    UserDefaults ud{connection};
    ud.put(JStr{env, key}, JStr{env, val});
}

// Tags Storage
JNIEXPORT jobjectArray JNICALL Java_com_sevencrayons_buxoff_TagsStorage_all(
    JNIEnv *env, jobject jobj) {
    TagsStorage ts{connection};
    std::set<std::string> tags = ts.all_tags();
    jobjectArray res = env->NewObjectArray(
        tags.size(), env->FindClass("java/lang/String"), NULL);

    int i = 0;
    for (const std::string& tag: tags) {
        env->SetObjectArrayElement(res, i, env->NewStringUTF(tag.c_str()));
        ++i;
    }
    return res;
}
