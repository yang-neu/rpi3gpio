# rpi3gpio


This is a complete android treble architecture sample worked on Raspberry pi 3b+ ,including:

1. HIDL, control a LED and switch connected to gpio port.
1. AIDL-CPP service / client
1. AIDL-CPP JNI client        (Ref: https://github.com/yang-neu/gpio_led_jni)
1. AIDL java client activity  (Ref: https://github.com/yang-neu/gpio_led)
1. SELinux configuration      (Ref: https://github.com/yang-neu/pie-device-rpi3)

This experiment achieved:

1. Toggle LED by a physical push button.
1. Count up the button push on the UI screen.
1. Turn on/off LED by the LED switch on the UI screen.

Some necessary modification
project build/make/
diff --git a/target/product/vndk/28.txt b/target/product/vndk/28.txt
index 712e91c58..7104adb6c 100644
--- a/target/product/vndk/28.txt
+++ b/target/product/vndk/28.txt
@@ -119,6 +119,9 @@ VNDK-core: android.hardware.radio.deprecated@1.0.so
 VNDK-core: android.hardware.radio@1.0.so
 VNDK-core: android.hardware.radio@1.1.so
 VNDK-core: android.hardware.radio@1.2.so
+VNDK-core: android.hardware.rpi3gpio@2.0.so
 VNDK-core: android.hardware.secure_element@1.0.so
 VNDK-core: android.hardware.sensors@1.0.so
 VNDK-core: android.hardware.soundtrigger@2.0.so

Specially appreciate:
=====================
- https://www.brobwind.com/
- https://www.jianshu.com/p/ca6823b897b5
- https://blog.csdn.net/zhuyong006/article/details/102488616
- https://blog.csdn.net/kuang_tian_you/article/details/86599869
- https://www.cnblogs.com/Anker/p/3263780.html
- https://www.cnblogs.com/aaronLinux/p/6684260.html
- https://github.com/gburca/BinderDemo/
- https://github.com/YancyGitHub/native-service  With SELinux configuration
- https://github.com/yuanhuihui/BinderSample
- https://qiita.com/okhiroyuki/items/50ce22099eb5ac9b862b
- https://github.com/qianjigui/android_system_service_example
- https://www.sitepoint.com/aidl-for-sharing-functionality-between-android-apps/
