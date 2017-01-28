#ifndef __Buxoff__Jutils__
#define __Buxoff__Jutils__

#include <string>

// convert jstring manipulations to stack allocated object routine
class JStr {
    const char* cstr;
    std::string str;
public:
    JStr(JNIEnv* env, jstring& jstr):
        cstr{env->GetStringUTFChars(jstr, 0)}, str{cstr} {
            env->ReleaseStringUTFChars(jstr, cstr);
        }
    const char* c_str() const { return str.c_str(); }
    operator std::string() const { return str; }
};

// http://stackoverflow.com/a/8492085/444966
void throw_java_exception(JNIEnv *env, const char *msg)
{
    // You can put your own exception here
    // jclass c;
    // if (c==nullptr) {
    //     c = env->FindClass("java/lang/NullPointerException");
    // } else {
    jclass c = env->FindClass("java/lang/RuntimeException");
    // }
    env->ThrowNew(c, msg);
}

#endif
