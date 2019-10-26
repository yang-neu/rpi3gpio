#define SERVICE_NAME    "com.example.gpio_led"
#define LOG_TAG "RPI3_DEMOCLIENT" 

#include <binder/IServiceManager.h>  
#include <com/example/gpio_led/IDemoService.h>
#include <com/example/gpio_led/ICallback.h>
#include <com/example/gpio_led/BnCallback.h>


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


using namespace android;
using namespace com::example::gpio_led;


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

int main(int argc, char *argv[])
{
    int opt; 
    int onoff = -1;
    int ret;
	
    sp<Callback> cb = new Callback(); 

    sp<IBinder> binder = defaultServiceManager()->getService(String16(SERVICE_NAME));  
    if(binder == NULL)
    {  
        printf("Failed to get service: %s.\n", SERVICE_NAME);  
        return -1;  
    }  

    sp<IDemoService> service = IDemoService::asInterface(binder);  
    if(service == NULL)  
    {  
        return -2;  
    }

    service->regist(cb);
    // put ':' in the starting of the 
    // string so that program can  
    //distinguish between '?' and ':' 
    while((opt = getopt(argc, argv, ":i:")) != -1)  
    {  
        switch(opt)  
        {  
            case 'i':  
		
		printf("option i=: %s\n", optarg);
		if (0 == strcmp(optarg, "on"))
		{
			onoff = 1;
		}
		if (0 == strcmp(optarg, "off"))
		{
			onoff = 0;
		}
		printf("onoff=: %d\n", onoff); 
		break;
            case ':':  
                printf("option needs a value\n");  
                break;  
            case '?':  
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  

	if(1 == onoff){
		ALOGE("IRpi3gpio ON");
		service->SetLed(onoff, &ret);
	}else if (0 == onoff)
	{
		ALOGE("IRpi3gpio OFF");
		service->SetLed(onoff, &ret);
    }else
    {
	// Don nothing
	ALOGE("IRpi3gpio does nothing.");
    }

    service->AddNumber(12, 13, &ret);
    printf("AddNumber(12, 13)=%d\n", ret);

    service->MaxNumber(12, 13, &ret);
    printf("MaxNumber(12, 13)=%d\n", ret);
    
    service->MaxNumber(16, 13, &ret);
    printf("MaxNumber(16, 13)=%d\n", ret);


    while(!cb->mCalled)
    {
	sleep(100);
	ALOGE("Waiting for callback...");
    }

}

