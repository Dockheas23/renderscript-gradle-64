#include <jni.h>
#include <memory>
#include <rsCppStructs.h>
#include <ScriptC_example.h>

extern "C" {

JNIEXPORT void JNICALL
Java_com_example_renderscriptgradle64_MainActivity_invoke(JNIEnv *env, jclass, jstring cacheDir_) {
    const char *cacheDir = env->GetStringUTFChars(cacheDir_, nullptr);
    sp<RS> rs = new android::RSC::RS();
    rs->init(cacheDir, 0);
    std::unique_ptr<ScriptC_example> p = std::make_unique<ScriptC_example>(rs);
    p->invoke_example_func();
}

}
