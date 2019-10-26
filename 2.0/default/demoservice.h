#ifndef __DEMO_SERVICE_H__
#define __DEMO_SERVICE_H__

//#include "com/example/gpio_led/BnDemoService.h"
#include <com/example/gpio_led/IDemoService.h>
#include <com/example/gpio_led/BnDemoService.h>
#include <com/example/gpio_led/ICallback.h>
#include "Rpi3gpioCallback.h"

using namespace android;

#define SERVICE_NAME    "com.example.gpio_led"

namespace com {

namespace example {

namespace gpio_led {

using ::com::example::gpio_led::ICallback;
using ::android::hardware::rpi3gpio::V2_0::IRpi3gpioCallback;

class DemoService : public BnDemoService
{
public:
	DemoService();
	virtual ::android::binder::Status AddNumber(int32_t a, int32_t b, int32_t* _aidl_return) override;
	virtual ::android::binder::Status MaxNumber(int32_t a, int32_t b, int32_t* _aidl_return) override;
	virtual ::android::binder::Status SetLed(int32_t a, int32_t* _aidl_return) override;    
	virtual ::android::binder::Status LedOn(int32_t* _aidl_return) override;
	virtual ::android::binder::Status LedOff(int32_t* _aidl_return) override;
    virtual ::android::binder::Status regist(const sp<ICallback>& cb) override; 
	virtual ::android::binder::Status pollSW(int32_t* _aidl_return) override;

    static void instance()
    {
        defaultServiceManager()->addService(String16(SERVICE_NAME), new DemoService());
    };
    sp<ICallback> mNotifyDemoClient;
    bool mHaveNotify;
    int32_t mSwStatus;

    pthread_mutex_t mutexHidlSW = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t conHidlSW;
    
private:
    sp<IRpi3gpioCallback> mGpioCallback;
    pthread_t hidlThread;



}; // class DemoService

}  // namespace gpio_led

}  // namespace example

}  // namespace com

#endif /* __DEMO_SERVICE_H__ */
