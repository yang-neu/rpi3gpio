#ifndef ANDROID_HARDWARE_RPI3GPIO_V2_0_RPI3GPIO_H
#define ANDROID_HARDWARE_RPI3GPIO_V2_0_RPI3GPIO_H

#include <android/hardware/rpi3gpio/2.0/IRpi3gpio.h>
#include <android/hardware/rpi3gpio/2.0/IRpi3gpioCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <log/log.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "TEST_RPI3GPIO_HAL_20"
#define UEVENT_MSG_LEN 2048

namespace android {
namespace hardware {
namespace rpi3gpio {
namespace V2_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;
using ::android::hardware::rpi3gpio::V2_0::IRpi3gpioCallback;
using ::android::hardware::rpi3gpio::V2_0::LedStatus;

struct Rpi3gpio : public IRpi3gpio {

    public:
        Rpi3gpio();  // Add manually
        ~Rpi3gpio();  // Add manually
        // Methods from ::android::hardware::rpi3gpio::V2_0::IRpi3gpio follow.
        Return<LedStatus> get() override;
        Return<int32_t> set(LedStatus val) override;
        Return<void> on() override;
        Return<void> off() override;
        Return<void> registerCallback(const sp<IRpi3gpioCallback>& cb) override;

    	sp<IRpi3gpioCallback> mCallback;
        pthread_mutex_t mutexSW = PTHREAD_MUTEX_INITIALIZER;
        pthread_cond_t conditionSW;
    // Methods from ::android::hidl::base::V1_0::IBase follow.
    private:
        LedStatus state;
        int valid;

        pthread_t mPoll;
        pthread_mutex_t mLock = PTHREAD_MUTEX_INITIALIZER;

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IRpi3gpio* HIDL_FETCH_IRpi3gpio(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace rpi3gpio
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_RPI3GPIO_V2_0_RPI3GPIO_H
