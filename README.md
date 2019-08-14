# RenderScript Gradle 64

This highlights an issue when building an app using the Android Gradle Plugin where the app contains RenderScript and native code, and `renderscriptNdkModeEnabled` is set to `true`. At runtime, the `armeabi-v7a` version of this app opens fine, but the `arm64-v8a` one crashes on launch.

## Steps to reproduce

1. Build the app: `$ ./gradlew assembleDebug`

2. Install a 64-bit version: `$ adb install --abi arm64-v8a app/build/outputs/apk/debug/app-debug.apk`

3. Open the app

This will result in a stack trace similar to the following:

```
2019-08-15 00:17:24.094 22281-22281/? E/bcc: Unable to compile the source to file /data/user/0/com.example.renderscriptgradle64/cache/example.o! (Error loading input bitcode)
2019-08-15 00:17:24.107 22224-22260/? E/RenderScript: Child process "/vendor/bin/bcc" terminated with status 256
2019-08-15 00:17:24.108 22224-22260/? E/RenderScript: bcc: FAILS to compile 'example'
2019-08-15 00:17:24.108 22224-22224/? E/rsC++: Internal error: Object id 0.
2019-08-15 00:17:24.108 22224-22260/? A/libc: Fatal signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0x0 in tid 22260 (rscriptgradle64), pid 22224 (rscriptgradle64)
2019-08-15 00:17:24.146 22285-22285/? I/crash_dump64: obtaining output fd from tombstoned, type: kDebuggerdTombstone
2019-08-15 00:17:24.147 6275-6275/? I//system/bin/tombstoned: received crash request for pid 22260
2019-08-15 00:17:24.147 22285-22285/? I/crash_dump64: performing dump of process 22224 (target tid = 22260)
2019-08-15 00:17:24.153 22285-22285/? A/DEBUG: *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG: Build fingerprint: 'samsung/starltexx/starlte:9/PPR1.180610.011/G960FXXU6CSG8:user/release-keys'
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG: Revision: '26'
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG: ABI: 'arm64'
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG: pid: 22224, tid: 22260, name: rscriptgradle64  >>> com.example.renderscriptgradle64 <<<
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG: signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0x0
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG: Cause: null pointer dereference
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG:     x0  000000754f299000  x1  0000000000000000  x2  0000000000000000  x3  0000000000000000
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG:     x4  0000000000000000  x5  0000000000000000  x6  6d64752dff2d6e71  x7  7f7f7f7f7f7f7f7f
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG:     x8  00000075343db5d8  x9  00000075343fb008  x10 0000000000000000  x11 0101010101010101
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG:     x12 0000000000000004  x13 ffffffffffffffff  x14 ffffffffff000000  x15 ffffffffffffffff
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG:     x16 00000075343fac78  x17 00000075343d03c8  x18 000000753439999a  x19 000000753439ab30
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG:     x20 000000754f299000  x21 000000000000001c  x22 000000753439ab30  x23 000000754f299590
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG:     x24 0000000000000001  x25 00000000ffffffff  x26 0000000000000000  x27 000000753439b588
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG:     x28 0000000000000008  x29 000000753439ab00
2019-08-15 00:17:24.154 22285-22285/? A/DEBUG:     sp  000000753439aae0  lr  00000075343db628  pc  00000075343d03c8
2019-08-15 00:17:24.155 22285-22285/? A/DEBUG: backtrace:
2019-08-15 00:17:24.155 22285-22285/? A/DEBUG:     #00 pc 00000000000353c8  /system/lib64/vndk-sp-28/libRS_internal.so (android::renderscript::rsi_ScriptInvokeV(android::renderscript::Context*, void*, unsigned int, void const*, unsigned long))
2019-08-15 00:17:24.155 22285-22285/? A/DEBUG:     #01 pc 0000000000040624  /system/lib64/vndk-sp-28/libRS_internal.so (android::renderscript::rsp_ScriptInvokeV(android::renderscript::Context*, void const*, unsigned long)+76)
2019-08-15 00:17:24.155 22285-22285/? A/DEBUG:     #02 pc 000000000003b834  /system/lib64/vndk-sp-28/libRS_internal.so (android::renderscript::ThreadIO::playCoreCommands(android::renderscript::Context*, int)+348)
2019-08-15 00:17:24.155 22285-22285/? A/DEBUG:     #03 pc 0000000000025ec8  /system/lib64/vndk-sp-28/libRS_internal.so (android::renderscript::Context::threadProc(void*)+2152)
2019-08-15 00:17:24.155 22285-22285/? A/DEBUG:     #04 pc 0000000000084df8  /system/lib64/libc.so (__pthread_start(void*)+208)
2019-08-15 00:17:24.155 22285-22285/? A/DEBUG:     #05 pc 0000000000023ac4  /system/lib64/libc.so (__start_thread+68)
2019-08-15 00:17:24.428 6275-6275/? E//system/bin/tombstoned: Tombstone written to: /data/tombstones/tombstone_04
2019-08-15 00:17:24.431 6222-6222/? E/audit: type=1701 audit(1565821044.426:8950): auid=4294967295 uid=10316 gid=10316 ses=4294967295 subj=u:r:untrusted_app:s0:c60,c257,c512,c768 pid=22224 comm="rscriptgradle64" exe="/system/bin/app_process64" sig=11
2019-08-15 00:17:24.440 6504-22288/? W/ActivityManager: crash : com.example.renderscriptgradle64,0
2019-08-15 00:17:24.443 6504-22288/? W/ActivityManager:   Force finishing activity com.example.renderscriptgradle64/.MainActivity
```

## Conclusion

The bug appears to be caused by the `-m32 / -m64` flag not being passed to the compiler: https://android.googlesource.com/platform/tools/base/+/refs/heads/master/build-system/builder/src/main/java/com/android/builder/internal/compiler/RenderScriptProcessor.java#224
