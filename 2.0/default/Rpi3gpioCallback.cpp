#define LOG_TAG "TEST_RPI3GPIO_HAL_CB_20"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <sys/signalfd.h>
#include <sys/timerfd.h>
#include <string.h>
#include <log/log.h>
#include "Rpi3gpioCallback.h"

namespace android {
namespace hardware {
namespace rpi3gpio {
namespace V2_0 {
namespace implementation {

// Methods from ::android::hardware::rpi3gpio::V2_0::IRpi3gpioCallback follow.
Return<void> Rpi3gpioCallback::sendEvent(uint32_t event_id) {
    // TODO implement
    ALOGD("Rpi3gpioCallback sendEvent (%d)", event_id);
    return Void();
}

Return<void> Rpi3gpioCallback::sendData(const hidl_vec<uint8_t>& data) {
    // TODO implement
    ALOGD("Rpi3gpioCallback sendData (%x)", (uint32_t)&data);
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IRpi3gpioCallback* HIDL_FETCH_IRpi3gpioCallback(const char* /* name */) {
    //return new Rpi3gpioCallback();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace rpi3gpio
}  // namespace hardware
}  // namespace android
