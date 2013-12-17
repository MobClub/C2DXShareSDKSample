//
//  C2DXiOSShareSDK.cpp
//  C2DXShareSDKSample
//
//  Created by 冯 鸿杰 on 13-12-17.
//
//

#include "C2DXiOSShareSDK.h"
#import <ShareSDK/ShareSDK.h>

using namespace cn::sharesdk;

void C2DXiOSShareSDK::open(CCString *appKey, bool useAppTrusteeship)
{
    NSString *appKeyStr = [NSString stringWithCString:appKey -> getCString() encoding:NSUTF8StringEncoding];
    [ShareSDK registerApp:appKeyStr useAppTrusteeship:useAppTrusteeship];
}

void C2DXiOSShareSDK::close()
{

}

void C2DXiOSShareSDK::setPlatformConfig(C2DXPlatType platType, CCDictionary *configInfo)
{
    NSMutableDictionary *configDict = [NSMutableDictionary dictionary];
    
    CCArray *configInfoKeys = configInfo -> allKeys();
    for (int i = 0; i < configInfoKeys -> count(); i++)
    {
        CCString *key = (CCString *)configInfoKeys -> objectAtIndex(i);
        CCString *value = (CCString *)configInfo -> objectForKey(key -> getCString());
        
        NSString *keyStr = [NSString stringWithCString:key -> getCString() encoding:NSUTF8StringEncoding];
//        NSString *valueStr = [NSString stringWithCString:<#(const char *)#> encoding:<#(NSStringEncoding)#>]
    }
    
    [ShareSDK connectPlatformWithType:(ShareType)platType
                             platform:nil
                              appInfo:configDict];
}

void C2DXiOSShareSDK::authorize(C2DXPlatType platType, C2DXAuthResultEvent callback)
{

}

void C2DXiOSShareSDK::cancelAuthorize(C2DXPlatType platType)
{

}

bool C2DXiOSShareSDK::hasAutorized(C2DXPlatType platType)
{

    return false;
}

void C2DXiOSShareSDK::shareContent(C2DXPlatType platType, CCDictionary *content, C2DXShareResultEvent callback)
{

}

void C2DXiOSShareSDK::oneKeyShareContent(CCArray *platTypes, CCDictionary *content, C2DXShareResultEvent callback)
{

}

void C2DXiOSShareSDK::showShareMenu(CCArray *platTypes, CCDictionary *content, C2DXShareResultEvent callback)
{

}