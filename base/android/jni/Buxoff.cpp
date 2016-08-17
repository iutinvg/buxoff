#include <jni.h>
#include <android/log.h>
// #include "../base/BaseCost.h"
// #include "../base/BaseJob.h"
// #include "../base/BaseAccount.h"

using namespace Buxoff;

#define  LOG_TAG    "buxoff"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

// static Job calclib_job;
// static Cost calclib_cost;

extern "C" {
    // Job related method
    JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_init(JNIEnv *, jobject obj, jstring filename);
    // JNIEXPORT void JNICALL Java_com_smarttradeapp_calc_CalcLib_setStripePaymentProvider(JNIEnv* env, jobject obj,
    //     jdouble percentage_rate, jdouble fixed_rate, jdouble st_percentage_rate);
    // JNIEXPORT void JNICALL Java_com_smarttradeapp_calc_CalcLib_setCashPaymentProvider(JNIEnv* env, jobject obj,
    //     jdouble percentage_rate, jdouble fixed_rate, jdouble st_percentage_rate);
};

// Job related mthods
JNIEXPORT void JNICALL Java_com_sevencrayons_buxoff_Buxoff_init(JNIEnv *, jobject obj, jstring filename)
{
    LOGI("boo");
}
