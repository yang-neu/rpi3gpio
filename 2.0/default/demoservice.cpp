//#define LOG_NDEBUG 0
#define LOG_TAG "RPI3GPIO_DEMOSERVICE"  


#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>  
//#include "common/IDemoService.h"  
#include "demoservice.h"

  
#include <utils/Log.h>  
#include "Rpi3gpio.h"

using android::hardware::rpi3gpio::V2_0::IRpi3gpio;
using android::hardware::rpi3gpio::V2_0::LedStatus;
using android::hardware::hidl_vec;
using android::sp;
using android::hardware::hidl_string;


namespace com {

namespace example {

namespace gpio_led {

void* hidlThread_work(void* param) {

	LedStatus stat;
	//int32_t ret;
	DemoService* pParent;
    String16 sOn("Calllback from hidlThread_work ON");
    String16 sOff("Calllback from hidlThread_work OFF");
    

	if(NULL == param) {
		ALOGE("NULL == param.");
		return NULL;
	}
	pParent = (DemoService*) param;

	sp<IRpi3gpio> service = IRpi3gpio::getService();
	if( service == nullptr ){
		ALOGE("Can't find IRpi3gpio service...");
		return NULL;
	}

	while(1){
		ALOGD("Before service->get()");
		stat = service->get();     // block the thread by hidl

		pthread_mutex_lock(&(pParent->mutexHidlSW)); 
		ALOGD("After service->get(%d)",(int)stat);
		if (LedStatus::LED_ON == stat){
			ALOGE("hidlThread_work: Turn on by SW");
			pParent->mSwStatus = 1;
			service->on();
			//pParent->LedOn(&ret);
			if (true == pParent->mHaveNotify){
				pParent->mNotifyDemoClient->dataCallback(sOn);
			}
			else{
				ALOGE("nullptr == mNotifyDemoClient");
			}		
		}
		else
		{
			ALOGE("hidlThread_work: Turn off by SW");
			pParent->mSwStatus = 0;
			service->off();
			//pParent->LedOff(&ret);
			if (true == pParent->mHaveNotify){
				pParent->mNotifyDemoClient->dataCallback(sOff);
			}
			else{
				ALOGE("nullptr == mNotifyDemoClient");
			}
		}
		pthread_cond_signal(&(pParent->conHidlSW)); 
		pthread_mutex_unlock(&(pParent->mutexHidlSW)); 
		
	}
}

DemoService::DemoService()
{
		mHaveNotify = false;
		mSwStatus = -1;
        mGpioCallback = new ::android::hardware::rpi3gpio::V2_0::implementation::Rpi3gpioCallback();
        sp<::android::hardware::rpi3gpio::V2_0::IRpi3gpio> service = IRpi3gpio::getService();
		if( service == nullptr ){
			ALOGE("Can't find IRpi3gpio@1.1 service...");
		}
		else{
			service->registerCallback(mGpioCallback);

			if (pthread_create(&hidlThread, NULL, hidlThread_work, this)) {
				ALOGE("pthread creation failed %d", errno);
			}
		}
}

//virtual ::android::binder::Status DemoService::AddNumber(int a,int b)
::android::binder::Status DemoService::AddNumber(int32_t a, int32_t b, int32_t* _aidl_return)
{
	::android::status_t _aidl_ret_status = ::android::OK;
	::android::binder::Status _aidl_status;
    ALOGD("DemoService AddNumber a:%d, b:%d\n", a, b);
	//return a + b
    (*_aidl_return) = a + b;
    _aidl_status.setFromStatusT(_aidl_ret_status);
    return _aidl_status;
}

//virtual ::android::binder::Status DemoService::MaxNumber(int a,int b)
::android::binder::Status DemoService::MaxNumber(int32_t a, int32_t b, int32_t* _aidl_return)
{
	::android::status_t _aidl_ret_status = ::android::OK;
	::android::binder::Status _aidl_status;
	ALOGD("DemoService MaxNumber a:%d, b:%d\n", a, b);
    //return a > b ? a : b;
    (*_aidl_return) = (a > b ? a : b);
    _aidl_status.setFromStatusT(_aidl_ret_status);
    return _aidl_status;
}

//virtual ::android::binder::Status DemoService::SetLed(int32_t a)
::android::binder::Status DemoService::SetLed(int32_t a, int32_t* _aidl_return)
{
	::android::status_t _aidl_ret_status = ::android::OK;
	::android::binder::Status _aidl_status;
	sp<IRpi3gpio> service = IRpi3gpio::getService();
	String16 sOff("Calllback from servcie LedOff");
	String16 sOn("Calllback from servcie LedOn");

	if( service == nullptr ){
		ALOGE("Can't find IRpi3gpio@2.0 service...");
		//return -1;
		_aidl_ret_status = ::android::UNKNOWN_ERROR;
		_aidl_status.setFromStatusT(_aidl_ret_status);
		return _aidl_status;
	}


	if(0 == a){
		//Turn off led
		ALOGI("IRpi3gpio OFF");
		service->off();
	}
	else{
		//Turn on led
		ALOGI("IRpi3gpio ON");
		service->on();
	}
	
	(*_aidl_return) = a;
	
    ALOGD("DemoService SetLed ");
    _aidl_status.setFromStatusT(_aidl_ret_status);
    return _aidl_status;
    //return 0;
}


::android::binder::Status DemoService::pollSW(int32_t* _aidl_return)
{
	::android::status_t _aidl_ret_status = ::android::OK;
	::android::binder::Status _aidl_status;
	sp<IRpi3gpio> service = IRpi3gpio::getService();	
	if( service == nullptr ){
		ALOGE("Can't find IRpi3gpio@2.0 service...");
		//return -1;
		_aidl_ret_status = ::android::UNKNOWN_ERROR;
		_aidl_status.setFromStatusT(_aidl_ret_status);
		return _aidl_status;
	}

	ALOGD("Before pollSW->get()");
	pthread_mutex_lock(&mutexHidlSW); //mutex lock

	pthread_cond_wait(&conHidlSW, &mutexHidlSW); //wait for the SW(gpio 18) pressed
	(*_aidl_return) = mSwStatus;

	pthread_mutex_unlock(&mutexHidlSW);
	ALOGD("After pollSW->get()");


    _aidl_status.setFromStatusT(_aidl_ret_status);
    return _aidl_status;
}

::android::binder::Status DemoService::LedOn(int32_t* _aidl_return)
{
	::android::status_t _aidl_ret_status = ::android::OK;
	::android::binder::Status _aidl_status;
	sp<IRpi3gpio> service = IRpi3gpio::getService();

	if( service == nullptr ){
		ALOGE("Can't find IRpi3gpio@2.0 service...");
		//return -1;
		_aidl_ret_status = ::android::UNKNOWN_ERROR;
		_aidl_status.setFromStatusT(_aidl_ret_status);
		return _aidl_status;
	}


	//Turn on led
	ALOGE("IRpi3gpio ON");
	service->on();

	(*_aidl_return) = 1;

    // String16 s("Calllback from servcie LedOn");
    // mNotifyDemoClient->dataCallback(s);
    // ALOGD("DemoService Ledon ");
    _aidl_status.setFromStatusT(_aidl_ret_status);
    return _aidl_status;
    //return 0;
}

::android::binder::Status DemoService::LedOff(int32_t* _aidl_return)
{
	::android::status_t _aidl_ret_status = ::android::OK;
	::android::binder::Status _aidl_status;
	sp<IRpi3gpio> service = IRpi3gpio::getService();

	if( service == nullptr ){
		ALOGE("Can't find IRpi3gpio@2.0 service...");
		//return -1;
		_aidl_ret_status = ::android::UNKNOWN_ERROR;
		_aidl_status.setFromStatusT(_aidl_ret_status);
		return _aidl_status;
	}


	//Turn on led
	ALOGE("IRpi3gpio OFF");
	service->off();

    // String16 s("Calllback from servcie LedOff");

    // mNotifyDemoClient->dataCallback(s);
    
    (*_aidl_return) = 0;

    // ALOGD("DemoService SetLed ");
    _aidl_status.setFromStatusT(_aidl_ret_status);
    return _aidl_status;
    //return 0;
}

::android::binder::Status DemoService::regist(const ::android::sp<::com::example::gpio_led::ICallback>& cb) 
{
    ::android::status_t _aidl_ret_status = ::android::OK;
    ::android::binder::Status _aidl_status;
    mNotifyDemoClient = cb;
	mHaveNotify = true;

    ALOGD("DemoService regist cb.");
    _aidl_status.setFromStatusT(_aidl_ret_status);
    return _aidl_status;
}


}  // namespace gpio_led

}  // namespace example

}  // namespace com

using namespace com::example::gpio_led;

//int main(int argc, char** argv)  
int main()  
{  
	//ProcessState::initWithDriver("/dev/vndbinder");

    com::example::gpio_led::DemoService::instance();  
	//pthread_t hidlThread;
	//DemoService* sv = new DemoService();

	//defaultServiceManager()->addService(String16(SERVICE_NAME), sv);

    ProcessState::self()->startThreadPool();  
    IPCThreadState::self()->joinThreadPool();  
  
    return 0;  
} 


































