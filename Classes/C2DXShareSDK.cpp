//
//  C2DXShareSDK.cpp
//  C2DXShareSDKSample
//
//  Created by 冯 鸿杰 on 13-12-17.
//
//

#include "C2DXShareSDK.h"
#include "C2DXiOSShareSDK.h"

using namespace cn::sharesdk;

void C2DXShareSDK::open(CCString *appKey, bool useAppTrusteeship)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

    //TODO: Andorid
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    //TODO: iOS
    C2DXiOSShareSDK::open(appKey, useAppTrusteeship);
    
#endif
}

void C2DXShareSDK::close()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    //TODO: Andorid
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    //TODO: iOS
    C2DXiOSShareSDK::close();
    
#endif
}

void C2DXShareSDK::setPlatformConfig(C2DXPlatType platType, CCDictionary *configInfo)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    //TODO: Andorid
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    //TODO: iOS
    C2DXiOSShareSDK::setPlatformConfig(platType, configInfo);
    
#endif
}

void C2DXShareSDK::authorize(C2DXPlatType platType, C2DXAuthResultEvent callback)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    //TODO: Andorid
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    //TODO: iOS
    C2DXiOSShareSDK::authorize(platType, callback);
    
#endif
}

void C2DXShareSDK::cancelAuthorize(C2DXPlatType platType)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    //TODO: Andorid
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    //TODO: iOS
    C2DXiOSShareSDK::cancelAuthorize(platType);
    
#endif
}

bool C2DXShareSDK::hasAutorized(C2DXPlatType platType)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    //TODO: Andorid
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    //TODO: iOS
    C2DXiOSShareSDK::hasAutorized(platType);
    
#endif
    return false;
}

void C2DXShareSDK::shareContent(C2DXPlatType platType, CCDictionary *content, C2DXShareResultEvent callback)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    //TODO: Andorid
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    //TODO: iOS
    C2DXiOSShareSDK::shareContent(platType, content, callback);
    
#endif
}

void C2DXShareSDK::oneKeyShareContent(CCArray *platTypes, CCDictionary *content, C2DXShareResultEvent callback)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    //TODO: Andorid
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    //TODO: iOS
    C2DXiOSShareSDK::oneKeyShareContent(platTypes, content, callback);
    
#endif
}

void C2DXShareSDK::showShareMenu(CCArray *platTypes, CCDictionary *content, C2DXShareResultEvent callback)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    //TODO: Andorid
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    //TODO: iOS
    C2DXiOSShareSDK::showShareMenu(platTypes, content, callback);
    
#endif
}
