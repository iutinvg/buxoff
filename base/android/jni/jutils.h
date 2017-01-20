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
