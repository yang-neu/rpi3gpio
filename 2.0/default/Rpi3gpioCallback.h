#ifndef ANDROID_HARDWARE_RPI3GPIO_V2_0_RPI3GPIOCALLBACK_H
#define ANDROID_HARDWARE_RPI3GPIO_V2_0_RPI3GPIOCALLBACK_H

#include <android/hardware/rpi3gpio/2.0/IRpi3gpioCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

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

struct Rpi3gpioCallback : public IRpi3gpioCallback {
    // Methods from ::android::hardware::rpi3gpio::V2_0::IRpi3gpioCallback follow.
    Return<void> sendEvent(uint32_t event_id) override;
    Return<void> sendData(const hidl_vec<uint8_t>& data) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IRpi3gpioCallback* HIDL_FETCH_IRpi3gpioCallback(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace rpi3gpio
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_RPI3GPIO_V2_0_RPI3GPIOCALLBACK_H
