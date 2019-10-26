/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#define LOG_TAG "RPI3_JNI" 
#include <string>
//#include <string.h>
#include <inttypes.h>
#include <pthread.h>
#include <jni.h>
//#include <android/log.h>
#include <assert.h>
#include <utils/Log.h>

#define SERVICE_NAME    "com.example.gpio_led"


#include <binder/IServiceManager.h>  
#include <com/example/gpio_led/IDemoService.h>
#include <com/example/gpio_led/ICallback.h>
#include <com/example/gpio_led/BnCallback.h>

using namespace android;
using namespace com::example::gpio_led;

namespace com {
namespace example {
namespace gpio_led {

class Callback : public BnCallback {

public:
    virtual ::android::binder::Status dataCallback(const ::android::String16& s) override;

    int mCalled = 0;
}; //class Callback

} // namespace gpio_led
} // namespace example
} // namespace com




// Android log function wrappers
//static const char* kTAG = "hello-jniCallback";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

// processing callback to handler class
typedef struct tick_context {
    JavaVM  *javaVM;
    jclass   jniHelperClz;
    jobject  jniHelperObj;
    jclass   mainActivityClz;
    jobject  mainActivityObj;
    pthread_mutex_t  lock;
    int      done;
} TickContext;
TickContext g_ctx;
sp<Callback> cb = new Callback(); 
int32_t swStatus;



::android::binder::Status Callback::dataCallback(const ::android::String16& s16)
{
    ::android::status_t _aidl_ret_status = ::android::OK;
    ::android::binder::Status _aidl_status;

    String8 s8(s16);

    mCalled = 1;

    ALOGD("%s",s8.string());
    ALOGD("Called by Service. ");
    _aidl_status.setFromStatusT(_aidl_ret_status);
    return _aidl_status;
 
}

// Helper function to get a hold of the "Service_LED" service.
sp<IDemoService> getService_LEDServ() {
    sp<IServiceManager> sm = defaultServiceManager();
    if(sm == NULL)
    {  
        printf("Failed to get service: %s.\n", SERVICE_NAME);  
        return NULL;  
    }  
    sp<IBinder> binder = sm->getService(String16(SERVICE_NAME));
    // TODO: If the "Service_LED" service is not running, getService times out and binder == 0.
    if(binder == NULL)
    {  
        printf("Failed to get service: %s.\n", SERVICE_NAME);  
        return NULL;  
    }  
    sp<IDemoService> sv_led = interface_cast<IDemoService>(binder);

    if(sv_led == NULL)  
    {  
        return NULL;  
    }
    return sv_led;
}

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   hello-jniCallback/app/src/main/java/com/example/hellojnicallback/MainActivity.java
 */
//JNIEXPORT jstring JNICALL
extern "C" JNIEXPORT jstring JNICALL
//Java_com_example_hellojnicallback_MainActivity_stringFromJNI( JNIEnv* env, jobject thiz )
Java_com_example_hellojnicallback_MainActivity_stringFromJNI( JNIEnv* env)
{
#if defined(__arm__)
    #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a/NEON (hard-float)"
      #else
        #define ABI "armeabi-v7a/NEON"
      #endif
    #else
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a (hard-float)"
      #else
        #define ABI "armeabi-v7a"
      #endif
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__x86_64__)
#define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
#define ABI "mips64"
#elif defined(__mips__)
#define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
#define ABI "unknown"
#endif
    //return env->NewStringUTF(env, "Hello from JNI !  Compiled with ABI " ABI ".");
    return env->NewStringUTF("Compiled with ABI " ABI ". (From JNI)");
}

//JNIEXPORT jstring JNICALL
extern "C" JNIEXPORT jint JNICALL
//Java_com_example_hellojnicallback_MainActivity_stringFromJNI( JNIEnv* env, jobject thiz )
Java_com_example_hellojnicallback_MainActivity_swStatus( JNIEnv* env){
    
    if(NULL == env)
    {
      ; //do nothing
    }
    
    return swStatus;
}

/*
 *  A helper function to show how to call
 *     java static functions JniHelper::getBuildVersion()
 *     java non-static function JniHelper::getRuntimeMemorySize()
 *  The trivial implementation for these functions are inside file
 *     JniHelper.java
 */
void queryRuntimeInfo(JNIEnv *env, jobject instance) {
    // Find out which OS we are running on. It does not matter for this app
    // just to demo how to call static functions.
    // Our java JniHelper class id and instance are initialized when this
    // shared lib got loaded, we just directly use them
    //    static function does not need instance, so we just need to feed
    //    class and method id to JNI
    jmethodID versionFunc;
    versionFunc = env->GetStaticMethodID(g_ctx.jniHelperClz, "getBuildVersion",
                                         "()Ljava/lang/String;");
    if (!versionFunc) {
        //ALOGE("Failed to retrieve getBuildVersion() methodID @ line %d", __LINE__);
        return;
    }
    jstring buildVersion = (jstring)env->CallStaticObjectMethod(g_ctx.jniHelperClz, versionFunc);
    const char *version = env->GetStringUTFChars(buildVersion, NULL);
    if (!version) {
        //ALOGE("Unable to get version string @ line %d", __LINE__);
        return;
    }
    //ALOGI("Android Version - %s", version);
    env->ReleaseStringUTFChars(buildVersion, version);

    // we are called from JNI_OnLoad, so got to release LocalRef to avoid leaking
    env->DeleteLocalRef(buildVersion);

    // Query available memory size from a non-static public function
    // we need use an instance of JniHelper class to call JNI
    jmethodID memFunc = env->GetMethodID(g_ctx.jniHelperClz,
                                            "getRuntimeMemorySize", "()J");
    if (!memFunc) {
        //ALOGE("Failed to retrieve getRuntimeMemorySize() methodID @ line %d", __LINE__);
        return;
    }
    jlong result = env->CallLongMethod(instance, memFunc);
    //ALOGI("Runtime free memory size: %" PRId64, result);
    (void)result;  // silence the compiler warning
}

/*
 * processing one time initialization:
 *     Cache the javaVM into our context
 *     Find class ID for JniHelper
 *     Create an instance of JniHelper
 *     Make global reference since we are using them from a native thread
 * Note:
 *     All resources allocated here are never released by application
 *     we rely on system to free all global refs when it goes away;
 *     the pairing function JNI_OnUnload() never gets called at all.
 */
extern "C" JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* /*reserved*/) {
//JNIEXPORT jint JNI_OnLoad(JavaVM* vm){
    JNIEnv* env;
    memset(&g_ctx, 0, sizeof(g_ctx));

    ALOGD("JNI_OnLoad Start...");

    g_ctx.javaVM = vm;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        ALOGE("vm->GetEnv JNI_ERR");
        return JNI_ERR; // JNI version not supported.
    }

    jobject clz = env->FindClass("com/example/hellojnicallback/JniHandler");
    g_ctx.jniHelperClz = (jclass)env->NewGlobalRef(clz);

    jmethodID  jniHelperCtor = env->GetMethodID(g_ctx.jniHelperClz,
                                                   "<init>", "()V");
    jobject    handler = env->NewObject(g_ctx.jniHelperClz,
                                           jniHelperCtor);
    g_ctx.jniHelperObj = env->NewGlobalRef(handler);
    queryRuntimeInfo(env, g_ctx.jniHelperObj);

    g_ctx.done = 0;
    g_ctx.mainActivityObj = NULL;
    ALOGD("JNI_OnLoad OK.");
    return  JNI_VERSION_1_6;
}

/*
 * A helper function to wrap java JniHelper::updateStatus(String msg)
 * JNI allow us to call this function via an instance even it is
 * private function.
 */
void   sendJavaMsg(JNIEnv *env, jobject instance,
                   jmethodID func,const char* msg) {
    jstring javaMsg = env->NewStringUTF(msg);
    env->CallVoidMethod(instance, func, javaMsg);
    env->DeleteLocalRef(javaMsg);
}

/*
 * Main working thread function. From a pthread,
 *     calling back to MainActivity::updateTimer() to display ticks on UI
 *     calling back to JniHelper::updateStatus(String msg) for msg
 */
void*  UpdateTicks(void* context) {
    TickContext *pctx = (TickContext*) context;
    JavaVM *javaVM = pctx->javaVM;
    JNIEnv *env;


    ALOGD("UpdateTicks Start...");
    sp<IDemoService> sv_led = getService_LEDServ();

    ALOGD("UpdateTicks Before GetEnv.");
    jint res = javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
    ALOGD("UpdateTicks res = %d.",res);
    if (res != JNI_OK) {
        res = javaVM->AttachCurrentThread(&env, NULL);
        if (JNI_OK != res) {
            ALOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
            return NULL;
        }
    }
    ALOGD("UpdateTicks Before statusId.");

    jmethodID statusId = env->GetMethodID(pctx->jniHelperClz,
                                             "updateStatus",
                                             "(Ljava/lang/String;)V");
    sendJavaMsg(env, pctx->jniHelperObj, statusId,
                "TickerThread status: initializing...");

    ALOGD("UpdateTicks Before timerId.");

    // get mainActivity updateTimer function
    jmethodID timerId = env->GetMethodID(pctx->mainActivityClz,
                                            "updateTimer", "()V");

    struct timeval beginTime, curTime, usedTime, leftTime;
    const struct timeval kOneSecond = {
            (__kernel_time_t)1,
            (__kernel_suseconds_t) 0
    };

    ALOGD("UpdateTicks Before sendJavaMsg.");

    sendJavaMsg(env, pctx->jniHelperObj, statusId,
                "TickerThread status: start ticking ...");

    ALOGD("UpdateTicks before loop.");

    while(1) {
        gettimeofday(&beginTime, NULL);
        pthread_mutex_lock(&pctx->lock);
        int done = pctx->done;
        if (pctx->done) {
            pctx->done = 0;
        }
        pthread_mutex_unlock(&pctx->lock);
        if (done) {
            break;
        }
        env->CallVoidMethod(pctx->mainActivityObj, timerId);

        gettimeofday(&curTime, NULL);
        timersub(&curTime, &beginTime, &usedTime);
        timersub(&kOneSecond, &usedTime, &leftTime);
        struct timespec sleepTime;
        sleepTime.tv_sec = leftTime.tv_sec;
        sleepTime.tv_nsec = leftTime.tv_usec * 1000;

        if (sleepTime.tv_sec <= 1) {
            sv_led->pollSW(&swStatus);
            ALOGD("GPIO SW status (%d)",swStatus);
            //nanosleep(&sleepTime, NULL);
        } else {
            sendJavaMsg(env, pctx->jniHelperObj, statusId,
                        "TickerThread error: processing too long!");
        }
    }

    sendJavaMsg(env, pctx->jniHelperObj, statusId,
                "TickerThread status: ticking stopped");
    javaVM->DetachCurrentThread();
    return context;
}

/*
 * Interface to Java side to start ticks, caller is from onResume()
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_hellojnicallback_MainActivity_startTicks(JNIEnv *env, jobject instance) {
    pthread_t       threadInfo_;
    pthread_attr_t  threadAttr_;

    ALOGD("startTicks Start...");
    sp<IDemoService> sv_led = getService_LEDServ();
    sv_led->regist(cb);


    pthread_attr_init(&threadAttr_);
    pthread_attr_setdetachstate(&threadAttr_, PTHREAD_CREATE_DETACHED);

    pthread_mutex_init(&g_ctx.lock, NULL);

    jclass clz = env->GetObjectClass(instance);
    g_ctx.mainActivityClz = (jclass)env->NewGlobalRef(clz);
    g_ctx.mainActivityObj = env->NewGlobalRef(instance);

    int result  = pthread_create( &threadInfo_, &threadAttr_, UpdateTicks, &g_ctx);
    assert(result == 0);

    pthread_attr_destroy(&threadAttr_);

    ALOGD("startTicks OK");

    (void)result;
}

/*
 * Interface to Java side to stop ticks:
 *    we need to hold and make sure our native thread has finished before return
 *    for a clean shutdown. The caller is from onPause
 */
extern "C" JNIEXPORT void JNICALL
//Java_com_example_hellojnicallback_MainActivity_StopTicks(JNIEnv *env, jobject instance) {
Java_com_example_hellojnicallback_MainActivity_StopTicks(JNIEnv *env) {
    pthread_mutex_lock(&g_ctx.lock);
    g_ctx.done = 1;
    pthread_mutex_unlock(&g_ctx.lock);

    // waiting for ticking thread to flip the done flag
    struct timespec sleepTime;
    memset(&sleepTime, 0, sizeof(sleepTime));
    sleepTime.tv_nsec = 100000000;
    while (g_ctx.done) {
        nanosleep(&sleepTime, NULL);
    }

    // release object we allocated from StartTicks() function
    env->DeleteGlobalRef(g_ctx.mainActivityClz);
    env->DeleteGlobalRef(g_ctx.mainActivityObj);
    g_ctx.mainActivityObj = NULL;
    g_ctx.mainActivityClz = NULL;

    pthread_mutex_destroy(&g_ctx.lock);
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_hellojnicallback_MainActivity_ledON(
        JNIEnv *env,
jobject /* this */) {
	std::string retStr = "led ON";
  int32_t ret;

	ALOGD("LED OFF");

	sp<IDemoService> sv_led = getService_LEDServ();
	sv_led->LedOn(&ret);
	return env->NewStringUTF(retStr.c_str());
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_hellojnicallback_MainActivity_ledOFF(
        JNIEnv *env,
jobject /* this */) {

	std::string retStr = "led off";
  int32_t ret;

	ALOGD("LED OFF");
	sp<IDemoService> sv_led = getService_LEDServ();
	sv_led->LedOff(&ret);
	return env->NewStringUTF(retStr.c_str());
}
