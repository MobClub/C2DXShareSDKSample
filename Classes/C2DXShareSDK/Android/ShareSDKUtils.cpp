#include "ShareSDKUtils.h"

#if 1
#define  LOG_TAG    "ShareSDKUtils"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define  LOGD(...) 
#endif

#ifdef __cplusplus
extern "C" {
#endif

C2DXAuthResultEvent authCb;
C2DXGetUserInfoResultEvent infoCb;
C2DXShareResultEvent shareCb;

JNIEXPORT void JNICALL Java_cn_sharesdk_ShareSDKUtils_onCancel
		(JNIEnv * env, jobject thiz, jobject platform, jint action) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "platformToId", "(Lcn/sharesdk/framework/Platform;)I");
	if (!isHave) {
		return;
	}

	int platformId = mi.env->CallStaticIntMethod(mi.classID, mi.methodID, platform);
	if (action == 1) { // 1 = ACTION_AUTHORIZING
		if (NULL != authCb) {
			authCb(C2DXResponseStateCancel, (C2DXPlatType) platformId, NULL);
		}
	} else if (action == 8) { // 8 = ACTION_USER_INFOR
		if (NULL != infoCb) {
			infoCb(C2DXResponseStateCancel, (C2DXPlatType) platformId, NULL, NULL);
		}
	}
}

JNIEXPORT void JNICALL Java_cn_sharesdk_ShareSDKUtils_onComplete
		(JNIEnv * env, jobject thiz, jobject platform, jint action, jobject res) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "platformToId", "(Lcn/sharesdk/framework/Platform;)I");
	if (!isHave) {
		return;
	}
	int platformId = mi.env->CallStaticIntMethod(mi.classID, mi.methodID, platform);
	if (action == 1) { // 1 = ACTION_AUTHORIZING
		if (NULL != authCb) {
			authCb(C2DXResponseStateSuccess, (C2DXPlatType) platformId, NULL);
		}
	} else if (action == 8) { // 8 = ACTION_USER_INFOR
		if (NULL != infoCb) {
			CCDictionary *dic = CCDictionary::create();
			hashmapToCCDictionary(res, dic);
			infoCb(C2DXResponseStateSuccess, (C2DXPlatType) platformId, dic, NULL);
		}
	}
}

JNIEXPORT void JNICALL Java_cn_sharesdk_ShareSDKUtils_onError
		(JNIEnv * env, jobject thiz, jobject platform, jint action, jobject res) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "platformToId", "(Lcn/sharesdk/framework/Platform;)I");
	if (!isHave) {
		return;
	}

	int platformId = mi.env->CallStaticIntMethod(mi.classID, mi.methodID, platform);
	CCDictionary *dic = CCDictionary::create();
	const char* errMsg = throwableToString(res);
	CCString* valueStr = CCString::create(errMsg);
	dic->setObject(valueStr, "error_msg");
	if (action == 1) { // 1 = ACTION_AUTHORIZING
		if (NULL != authCb) {
			authCb(C2DXResponseStateFail, (C2DXPlatType) platformId, dic);
		}
	} else if (action == 8) { // 8 = ACTION_USER_INFOR
		if (NULL != infoCb) {
			infoCb(C2DXResponseStateFail, (C2DXPlatType) platformId, NULL, dic);
		}
	}
}

void hashmapToCCDictionary(jobject hashmap, CCDictionary *dic) {
	
}

const char* throwableToString(jobject t) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "throwableToJson", "(Ljava/lang/Throwable;)Ljava/lang/String;");
	if (!isHave) {
		return NULL;
	}

	jstring msg = (jstring) mi.env->CallStaticObjectMethod(mi.classID, mi.methodID, t);
	return (const char*)mi.env->GetStringUTFChars(msg, JNI_FALSE);  
}

bool getMethod(JniMethodInfo &mi, const char *methodName, const char *paramCode) {
	return JniHelper::getStaticMethodInfo(mi, "cn/sharesdk/ShareSDKUtils", methodName, paramCode);
}


void CCDictionaryToHashMap(CCDictionary *info, jobject &hashmap) {
	JniMethodInfo miPut;
	bool isHave = getMethod(miPut, "putData", "(Ljava/util/HashMap;Ljava/lang/String;Ljava/lang/String;)V");
	if (isHave) {
		CCArray* keys = info->allKeys();
		int index = 0;
		int count = keys->count();
		while (index < count) {
			CCString* key = (CCString*) keys->objectAtIndex(index);
			CCString* value = (CCString*) info->objectForKey(key->getCString());
			jstring keyStr = miPut.env->NewStringUTF(key->getCString());
			jstring valueStr = miPut.env->NewStringUTF(value->getCString());
			miPut.env->CallStaticVoidMethod(miPut.classID, miPut.methodID, hashmap, keyStr, valueStr);
			index++;
		}
	}
}

bool initShareSDK(const char* appKey, bool useAppTrusteeship) {
	JniMethodInfo mi;
 	bool isHave = getMethod(mi, "initSDK", "(Ljava/lang/String;Z)V");
	if (!isHave) {
		return false;
	}
 
	jstring appKeyStr = mi.env->NewStringUTF(appKey);
	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, appKeyStr, useAppTrusteeship);
	return true;
}

bool stopSDK() {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "stopSDK", "()V");
	if (!isHave) {
		return false;
	}
	
	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID);
	return true;
}

bool setPlatformDevInfo(int platformId, CCDictionary *info) {
	JniMethodInfo miNew;
	bool isHave = getMethod(miNew, "newHashMap", "()Ljava/util/HashMap;");
	if (!isHave) {
		return false;
	}

	jobject hashmap = miNew.env->CallStaticObjectMethod(miNew.classID, miNew.methodID);
	CCDictionaryToHashMap(info, hashmap);
	
	JniMethodInfo mi;
	isHave = getMethod(mi, "setPlatformConfig", "(ILjava/util/HashMap;)V");
	if (!isHave) {
		return false;
	}
	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platformId, hashmap);
	return true;
}

bool doAuthorize(int platformId, C2DXAuthResultEvent callback) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "authorize", "(I)V");
	if (!isHave) {
		return false;
	}

	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platformId);
	authCb = callback;
	return true;
}

bool removeAccount(int platformId) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "removeAccount", "(I)V");
	if (!isHave) {
		return false;
	}

	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platformId);
	return true;
}

bool isValid(int platformId) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "isValid", "(I)Z");
	if (!isHave) {
		return false;
	}

	jboolean valid = mi.env->CallStaticBooleanMethod(mi.classID, mi.methodID, platformId);
	return valid == JNI_TRUE;
}

bool showUser(int platformId, C2DXGetUserInfoResultEvent callback){
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "showUser", "(I)V");
	if (!isHave) {
		return false;
	}
	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platformId);
	infoCb = callback;
	return true;
}

bool doShare(int platformId, CCDictionary *content, C2DXShareResultEvent callback){
	JniMethodInfo miNew;
	bool isHave = getMethod(miNew, "newHashMap", "()Ljava/util/HashMap;");
	if (!isHave) {
		return false;
	}

	jobject hashmap = miNew.env->CallStaticObjectMethod(miNew.classID, miNew.methodID);
	CCDictionaryToHashMap(content, hashmap);
	JniMethodInfo mi;
	isHave = getMethod(mi, "share", "(ILjava/util/HashMap;)V");
	if (!isHave) {
		return false;
	}
	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platformId, hashmap);
	
	shareCb = callback;
	return false;
}

bool multiShare(CCArray *platTypes, CCDictionary *content, C2DXShareResultEvent callback) {
	int index = 0;
	int count = platTypes->count();
	while(index < count) {
		CCInteger* item = (CCInteger*) platTypes->objectAtIndex(index);
		int platformId = item->getValue();
		doShare(platformId, content, callback);
		index++;
	}
	return true;
}

bool onekeyShare(CCArray *platTypes, CCDictionary *content, C2DXShareResultEvent callback) {
	// TODO create arraylist from java
	// TODO put data into arraylist
	// TODO call onekeyshare
}


#ifdef __cplusplus
}
#endif
