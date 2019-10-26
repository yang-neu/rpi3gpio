#define LOG_TAG "android.hardware.rpi3gpio@2.0-service"
 
#include <android/hardware/rpi3gpio/2.0/IRpi3gpio.h>
#include <hidl/LegacySupport.h>
#include "Rpi3gpio.h"
using android::hardware::rpi3gpio::V2_0::IRpi3gpio;
using android::hardware::rpi3gpio::V2_0::implementation::Rpi3gpio;
using android::hardware::defaultPassthroughServiceImplementation;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::sp;
 
 
int main() {
// Binder approach
  sp<IRpi3gpio> service = new Rpi3gpio();
  configureRpcThreadpool(3, true /*callerWillJoin*/);
    if(android::OK !=  service->registerAsService())
      return 1; 
    joinRpcThreadpool();
}
