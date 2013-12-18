#include "ShareSDKUtils.h"

USING_NS_CC;

#if 1
#define  LOG_TAG    "ShareSDKUtils"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define  LOGD(...) 
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     ShareSDKUtils
 * Method:    onCancel
 * Signature: (Lcn/sharesdk/framework/Platform;I)V
 */
JNIEXPORT void JNICALL Java_cn_sharesdk_ShareSDKUtils_onCancel
		(JNIEnv * env, jobject thiz, jobject platform, jint action) {
}

/*
 * Class:     ShareSDKUtils
 * Method:    onComplete
 * Signature: (Lcn/sharesdk/framework/Platform;ILjava/util/HashMap;)V
 */
JNIEXPORT void JNICALL Java_cn_sharesdk_ShareSDKUtils_onComplete
		(JNIEnv * env, jobject thiz, jobject platform, jint action, jobject res) {

}

/*
 * Class:     ShareSDKUtils
 * Method:    onError
 * Signature: (Lcn/sharesdk/framework/Platform;ILjava/lang/Throwable;)V
 */
JNIEXPORT void JNICALL Java_cn_sharesdk_ShareSDKUtils_onError
		(JNIEnv * env, jobject thiz, jobject platform, jint action, jobject res) {

}

bool initShareSDK(const char* appKey, bool useAppTrusteeship) {
	JniMethodInfo mi;
	bool isHave = JniHelper::getStaticMethodInfo(mi, "cn/sharesdk/ShareSDKUtils", "initSDK", "(Ljava/lang/String;Z)V");
	if (!isHave) {
		return false;
	}

	jstring appKeyStr = mi.env->NewStringUTF(appKey);
	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, appKeyStr, useAppTrusteeship);
	return true;
}

bool stopSDK() {
	JniMethodInfo mi;
	bool isHave = JniHelper::getStaticMethodInfo(mi, "cn/sharesdk/ShareSDKUtils", "stopSDK", "()V");
	if (!isHave) {
		return false;
	}
	
	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID);
	return true;
}

#ifdef __cplusplus
}
#endif
