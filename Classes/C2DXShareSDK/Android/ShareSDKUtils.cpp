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
	} else if (action == 9) { // 9 = ACTION_SHARE
		if (NULL != shareCb) {
			shareCb(C2DXResponseStateCancel, (C2DXPlatType) platformId, NULL, NULL);
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
	} else {
		CCDictionary *dic = new CCDictionary();
		hashmapToCCDictionary(res, dic);
		if (action == 8) { // 8 = ACTION_USER_INFOR
			if (NULL != infoCb) {
				infoCb(C2DXResponseStateSuccess, (C2DXPlatType) platformId, dic, NULL);
			}
		} else if (action == 9) { // 9 = ACTION_SHARE
			if (NULL != shareCb) {
				shareCb(C2DXResponseStateSuccess, (C2DXPlatType) platformId, dic, NULL);
			}
		}
		dic->autorelease();
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
	} else if (action == 9) { // 9 = ACTION_SHARE
		if (NULL != shareCb) {
			shareCb(C2DXResponseStateFail, (C2DXPlatType) platformId, NULL, dic);
		}
	}
}

void hashmapToCCDictionary(jobject hashmap, CCDictionary *dic) {
	JniMethodInfo mimGetKeys;
	bool isHave = getMethod(mimGetKeys, "getMapKeys", "(Ljava/util/HashMap;)Ljava/util/ArrayList;");
	if (!isHave) {
		return;
	}

	jobject keys = mimGetKeys.env->CallStaticObjectMethod(mimGetKeys.classID, mimGetKeys.methodID, hashmap);
	JniMethodInfo mimListSize;
	isHave = getMethod(mimListSize, "getListSize", "(Ljava/util/ArrayList;)I");
	if (!isHave) {
		return;
	}

	int count = mimListSize.env->CallStaticIntMethod(mimListSize.classID, mimListSize.methodID, keys);
	if (count <= 0) {
		return;
	}

	JniMethodInfo mimGetListData;
	isHave = getMethod(mimGetListData, "getData", "(Ljava/util/ArrayList;I)Ljava/lang/String;");
	if (!isHave) {
		return;
	}

	JniMethodInfo miGetMapData;
	isHave = getMethod(miGetMapData, "getData", "(Ljava/util/HashMap;Ljava/lang/String;)Ljava/lang/Object;");
	if (!isHave) {
		return;
	}

	int index = 0;
	while (index < count) {
		jstring key = (jstring) mimGetListData.env->CallStaticObjectMethod(mimGetListData.classID, mimGetListData.methodID, keys, index);
		const char* ccKey = mimGetListData.env->GetStringUTFChars(key, JNI_FALSE);
		jobject value = miGetMapData.env->CallStaticObjectMethod(miGetMapData.classID, miGetMapData.methodID, hashmap, key);
		int type = getJObjectType(value);
		switch(type) {
			case 1:
			case 2:
			case 3: { // jint, ilong, jdouble
				double jValue = jObjectToJDouble(value);
				CCDouble* ccValue = CCDouble::create(jValue);
//				CCLog("=========== %f", jValue);
//				CCLog("=========== %s", ccKey);
				dic->setObject(ccValue, ccKey);
			}
			break;
			case 4: { // jboolean
				bool jValue = jObjectToJBoolean(value);
				CCBool* ccValue = CCBool::create(jValue);
//				CCLog("=========== %d", jValue);
//				CCLog("=========== %s", ccKey);
				dic->setObject(ccValue, ccKey);
			}
			break;
			case 5: { // jstring
				const char* jValue = jObjectToJString(mimGetListData, value);
				if (NULL != jValue) {
					CCString* ccValue = CCString::create(jValue);
//					CCLog("=========== %s", jValue);
//					CCLog("=========== %s", ccKey);
					dic->setObject(ccValue, ccKey);
				}
			}
			break;
			case 6: { // arraylist
				CCArray* ccValue = new CCArray();
//				CCLog("=========== arraylist[]");
				arraylistToCCArray(value, ccValue);
//				CCLog("=========== %s", ccKey);
				dic->setObject(ccValue, ccKey);
				ccValue->autorelease();
			}
			break;
			case 7: { // hashmap
				CCDictionary* ccValue = new CCDictionary();
//				CCLog("=========== hashmap{}");
				hashmapToCCDictionary(value, ccValue);
//				CCLog("=========== %s", ccKey);
				dic->setObject(ccValue, ccKey);
				ccValue->autorelease();
			}
			break;
		}
		index++;
	}
}

int getJObjectType(jobject value) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "getObjectType", "(Ljava/lang/Object;)I");
	if (!isHave) {
		return 0;
	}

	return mi.env->CallStaticIntMethod(mi.classID, mi.methodID, value);
}

double jObjectToJDouble(jobject value) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "jObjectToJDouble", "(Ljava/lang/Object;)D");
	if (!isHave) {
		return 0;
	}

	return mi.env->CallStaticDoubleMethod(mi.classID, mi.methodID, value);
}

bool jObjectToJBoolean(jobject value) {
	double jValue = jObjectToJDouble(value);
	return 1 == jValue;
}

const char* jObjectToJString(JniMethodInfo mi, jobject value) {
	return mi.env->GetStringUTFChars((jstring)value, JNI_FALSE);
}

void arraylistToCCArray(jobject arraylist, CCArray* arr) {
	JniMethodInfo mimListSize;
	bool isHave = getMethod(mimListSize, "getListSize", "(Ljava/util/ArrayList;)I");
	if (!isHave) {
		return;
	}

	int count = mimListSize.env->CallStaticIntMethod(mimListSize.classID, mimListSize.methodID, arraylist);
	if (count <= 0) {
		return;
	}

	JniMethodInfo miGetListType;
	isHave = getMethod(miGetListType, "getListType", "(Ljava/util/ArrayList;)I");
	if (!isHave) {
		return;
	}

	int type = miGetListType.env->CallStaticIntMethod(miGetListType.classID, miGetListType.methodID, arraylist);
	JniMethodInfo miGetItem;
	isHave = getMethod(miGetItem, "getObjectData", "(Ljava/util/ArrayList;I)Ljava/lang/Object;");
	if (!isHave) {
		return;
	}
	
	int index = 0;
	while(index < count) {
		jobject jItem = miGetItem.env->CallStaticObjectMethod(miGetItem.classID, miGetItem.methodID, arraylist, index);
		switch (type) {
			case 1:
			case 2:
			case 3: { // jint, jlong, jdouble
				double jValue = jObjectToJDouble(jItem);
				CCDouble* ccItem = CCDouble::create(jValue);
				arr->addObject(ccItem);
			}
			break;
			case 4: { // jboolean
				bool jValue = jObjectToJBoolean(jItem);
				CCBool* ccItem = CCBool::create(jValue);
				arr->addObject(ccItem);
			}
			break;
			case 5: { // jstring
				const char* jValue = jObjectToJString(miGetItem, jItem);
				if (NULL != jValue) {
					CCString* ccItem = CCString::create(jValue);
					arr->addObject(ccItem);
				}
			}
			break;
			case 6: { // arraylist
				CCArray* ccItem = new CCArray();
				arraylistToCCArray(jItem, ccItem);
				arr->addObject(ccItem);
				ccItem->autorelease();
			}
			break;
			case 7: { // hashmap
				CCDictionary* ccItem = new CCDictionary();
				hashmapToCCDictionary(jItem, ccItem);
				arr->addObject(ccItem);
				ccItem->autorelease();
			}
			break;
		}
		index++;
	}
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
	return true;
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
	JniMethodInfo miNewMap;
	bool isHave = getMethod(miNewMap, "newHashMap", "()Ljava/util/HashMap;");
	if (!isHave) {
		return false;
	}

	jobject hashmap = miNewMap.env->CallStaticObjectMethod(miNewMap.classID, miNewMap.methodID);
	CCDictionaryToHashMap(content, hashmap);
	
	if (NULL == platTypes || platTypes->count() <= 0) {
		JniMethodInfo miOks;
		isHave = getMethod(miOks, "onekeyShare", "(Ljava/util/HashMap;)V");
		if (!isHave) {
			return false;
		}

		miOks.env->CallStaticVoidMethod(miOks.classID, miOks.methodID, hashmap);
	} else {
		JniMethodInfo miNew;
		isHave = getMethod(miNew, "newArrayList", "()Ljava/util/ArrayList;");
		if (!isHave) {
			return false;
		}

		jobject arraylist = miNew.env->CallStaticObjectMethod(miNew.classID, miNew.methodID);
		JniMethodInfo mi;
		isHave = getMethod(mi, "putData", "(Ljava/util/ArrayList;I)V");
		if (!isHave) {
			return false;
		}
		
		int index = 0;
		int count = platTypes->count();
		while(index < count) {
			CCInteger* item = (CCInteger*) platTypes->objectAtIndex(index);
			int platformId = item->getValue();
			mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, arraylist, platformId);
			index++;
		}
		
		JniMethodInfo miOks;
		isHave = getMethod(miOks, "onekeyShare", "(Ljava/util/ArrayList;Ljava/util/HashMap;)V");
		if (!isHave) {
			return false;
		}

		miOks.env->CallStaticVoidMethod(miOks.classID, miOks.methodID, arraylist, hashmap);
	}
		
	shareCb = callback;
	return true;

	// TODO put data into arraylist
	// TODO call onekeyshare
}


#ifdef __cplusplus
}
#endif
