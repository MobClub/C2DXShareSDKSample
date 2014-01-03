//
//  C2DXiOSShareSDK.cpp
//  C2DXShareSDKSample
//
//  Created by 冯 鸿杰 on 13-12-17.
//
//

#include "C2DXiOSShareSDK.h"
#import <ShareSDK/ShareSDK.h>
#import <AGCommon/CMRegexKitLite.h>

using namespace cn::sharesdk;

/**
 *	@brief	转换NSDictionary为CCDicationary类型
 *
 *	@param 	dict 	字典
 *
 *	@return	字典类型
 */
CCDictionary* convertNSDictToCCDict(NSDictionary *dict);

/**
 *	@brief	转换NSString为CCString类型
 *
 *	@param 	string 	字符串
 *
 *	@return	字符类型
 */
CCString* convertNSStringToCCString(NSString *string);

/**
 *	@brief	转换NSArray为CCArray类型
 *
 *	@param 	array 	数组
 *
 *	@return	数组类型
 */
CCArray* convertNSArrayToCCArray(NSArray *array);

/**
 *	@brief	转换分享内容
 *
 *	@param 	content 	内容字典结构
 *
 *	@return	分享内容对象
 */
id<ISSContent> convertPublishContent(CCDictionary *content);



CCDictionary* convertNSDictToCCDict(NSDictionary *dict)
{
    if (dict)
    {
        CCDictionary *ccDict = CCDictionary::create();
        
        NSArray *allKeys = [dict allKeys];
        for (int i = 0; i < [allKeys count]; i++)
        {
            NSString *key = [allKeys objectAtIndex:i];
            id value = [dict objectForKey:key];
            
            if ([value isKindOfClass:[NSString class]])
            {
                CCString *strValue = convertNSStringToCCString(value);
                if (strValue)
                {
                    ccDict -> setObject(strValue, [key UTF8String]);
                }
            }
            else if ([value isKindOfClass:[NSNumber class]])
            {
                CCString *strValue = convertNSStringToCCString([NSString stringWithFormat:@"%@", value]);
                if (strValue)
                {
                    ccDict -> setObject(strValue, [key UTF8String]);
                }
            }
            else if ([value isKindOfClass:[NSDate class]])
            {
                ccDict -> setObject(CCDouble::create([value timeIntervalSince1970] * 1000), [key UTF8String]);
            }
            else if ([value isKindOfClass:[NSDictionary class]])
            {
                CCDictionary *dictValue = convertNSDictToCCDict(value);
                if (dictValue)
                {
                    ccDict -> setObject(dictValue, [key UTF8String]);
                }
            }
            else if ([value isKindOfClass:[NSArray class]])
            {
                CCArray *arrValue = convertNSArrayToCCArray(value);
                if (arrValue)
                {
                    ccDict -> setObject(arrValue, [key UTF8String]);
                }
            }
        }
        
        return ccDict;
    }
    
    return NULL;
}

CCString* convertNSStringToCCString(NSString *string)
{
    if (string)
    {
        return CCString::create([string cStringUsingEncoding:NSUTF8StringEncoding]);
    }
    
    return NULL;
}

CCArray* convertNSArrayToCCArray(NSArray *array)
{
    if (array)
    {
        CCArray *ccArray = CCArray::create();
        
        for (int i = 0; i < [array count]; i++)
        {
            id value = [array objectAtIndex:i];
            if ([value isKindOfClass:[NSString class]])
            {
                CCString *strValue = convertNSStringToCCString(value);
                if (strValue)
                {
                    ccArray -> addObject(strValue);
                }
            }
            else if ([value isKindOfClass:[NSNumber class]])
            {
                CCString *strValue = convertNSStringToCCString([NSString stringWithFormat:@"%@", value]);
                if (strValue)
                {
                    ccArray -> addObject(strValue);
                }
            }
            else if ([value isKindOfClass:[NSDate class]])
            {
                ccArray -> addObject(CCDouble::create([value timeIntervalSince1970] * 1000));
            }
            else if ([value isKindOfClass:[NSDictionary class]])
            {
                CCDictionary *dictValue = convertNSDictToCCDict(value);
                if (dictValue)
                {
                    ccArray -> addObject(dictValue);
                }
            }
            else if ([value isKindOfClass:[NSArray class]])
            {
                CCArray *arrayValue = convertNSArrayToCCArray(value);
                if (arrayValue)
                {
                    ccArray -> addObject(arrayValue);
                }
            }
        }
        
        return ccArray;
    }
    
    return NULL;
}

id<ISSContent> convertPublishContent(CCDictionary *content)
{
    NSString *message = nil;
    id<ISSCAttachment> image = nil;
    NSString *title = nil;
    NSString *url = nil;
    NSString *desc = nil;
    SSPublishContentMediaType type = SSPublishContentMediaTypeText;
    
    if (content)
    {
        CCString *messageStr = dynamic_cast<CCString *>(content -> objectForKey("content"));
        if (messageStr)
        {
            message = [NSString stringWithCString:messageStr -> getCString() encoding:NSUTF8StringEncoding];
        }
        
        CCString *imagePathStr = dynamic_cast<CCString *>(content -> objectForKey("image"));
        if (imagePathStr)
        {
            NSString *imagePath = [NSString stringWithCString:imagePathStr -> getCString() encoding:NSUTF8StringEncoding];
            if ([imagePath isMatchedByRegex:@"\\w://.*"])
            {
                image = [ShareSDK imageWithUrl:imagePath];
            }
            else
            {
                image = [ShareSDK imageWithPath:imagePath];
            }
        }
        
        CCString *titleStr = dynamic_cast<CCString *>(content -> objectForKey("title"));
        if (titleStr)
        {
            title = [NSString stringWithCString:titleStr -> getCString() encoding:NSUTF8StringEncoding];
        }
        
        CCString *urlStr = dynamic_cast<CCString *>(content -> objectForKey("url"));
        if (urlStr)
        {
            url = [NSString stringWithCString:urlStr -> getCString() encoding:NSUTF8StringEncoding];
        }
        
        CCString *descStr = dynamic_cast<CCString *>(content -> objectForKey("description"));
        if (descStr)
        {
            desc = [NSString stringWithCString:descStr -> getCString() encoding:NSUTF8StringEncoding];
        }
        
        CCString *typeValue = dynamic_cast<CCString *>(content -> objectForKey("type"));
        if (typeValue)
        {
            type = (SSPublishContentMediaType)typeValue -> intValue();
        }
    }
    
    return  [ShareSDK content:message
               defaultContent:nil
                        image:image
                        title:title
                          url:url
                  description:desc
                    mediaType:type];
}

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
    NSMutableDictionary *configDict = nil;
    
    if (configInfo)
    {
        //转换配置信息
        CCArray *configInfoKeys = configInfo -> allKeys();
        if (configInfoKeys)
        {
            configDict = [NSMutableDictionary dictionary];
            
            for (int i = 0; i < configInfoKeys -> count(); i++)
            {
                CCString *key = (CCString *)configInfoKeys -> objectAtIndex(i);
                CCString *value = (CCString *)configInfo -> objectForKey(key -> getCString());
                
                NSString *keyStr = [NSString stringWithCString:key -> getCString() encoding:NSUTF8StringEncoding];
                NSString *valueStr = [NSString stringWithCString:value -> getCString() encoding:NSUTF8StringEncoding];
                if (keyStr && valueStr)
                {
                    [configDict setObject:valueStr forKey:keyStr];
                }
            }
        }
    }
    
    switch (platType)
    {
        case C2DXPlatTypeWeixiSession:
        case C2DXPlatTypeYiXinSession:
            [configDict setObject:[NSNumber numberWithInt:0] forKey:@"scene"];
            break;
        case C2DXPlatTypeWeixiTimeline:
        case C2DXPlatTypeYiXinTimeline:
            [configDict setObject:[NSNumber numberWithInt:1] forKey:@"scene"];
            break;
        case C2DXPlatTypeWeixiFav:
            [configDict setObject:[NSNumber numberWithInt:2] forKey:@"scene"];
            break;
        default:
            break;
    }
    
    [ShareSDK connectPlatformWithType:(ShareType)platType
                             platform:nil
                              appInfo:configDict];
}

void C2DXiOSShareSDK::authorize(C2DXPlatType platType, C2DXAuthResultEvent callback)
{
    [ShareSDK authWithType:(ShareType)platType
                   options:nil
                    result:^(SSAuthState state, id<ICMErrorInfo> error) {
                        
                        CCDictionary *errorInfo = NULL;
                        
                        if (error)
                        {
                            errorInfo = CCDictionary::create();
                            errorInfo -> setObject(CCInteger::create([error errorCode]), "error_code");
                            errorInfo -> setObject(CCString::create([[error errorDescription] UTF8String]), "error_msg");
                        }
                        
                        if (callback)
                        {
                            callback ((C2DXResponseState)state, platType, errorInfo);
                        }
                        
                    }];
}

void C2DXiOSShareSDK::cancelAuthorize(C2DXPlatType platType)
{
    [ShareSDK cancelAuthWithType:(ShareType)platType];
}

bool C2DXiOSShareSDK::hasAutorized(C2DXPlatType platType)
{
    return [ShareSDK hasAuthorizedWithType:(ShareType)platType] ? true : false;
}

void C2DXiOSShareSDK::getUserInfo(C2DXPlatType platType, C2DXGetUserInfoResultEvent callback)
{
    [ShareSDK getUserInfoWithType:(ShareType)platType
                      authOptions:nil
                           result:^(BOOL result, id<ISSPlatformUser> userInfo, id<ICMErrorInfo> error) {
                               
                               CCDictionary *userInfoDict = NULL;
                               CCDictionary *errorInfo = NULL;
                               
                               if (result)
                               {
                                   userInfoDict = convertNSDictToCCDict([userInfo sourceData]);
                               }
                               
                               if (error)
                               {
                                   errorInfo = CCDictionary::create();
                                   errorInfo -> setObject(CCInteger::create([error errorCode]), "error_code");
                                   errorInfo -> setObject(CCString::create([[error errorDescription] UTF8String]), "error_msg");
                               }
                               
                               if (callback)
                               {
                                   callback (result ? C2DXResponseStateSuccess : C2DXResponseStateFail, platType, userInfoDict, errorInfo);
                               }
                               
                           }];
}

void C2DXiOSShareSDK::shareContent(C2DXPlatType platType, CCDictionary *content, C2DXShareResultEvent callback)
{
    id<ISSContent> publishContent = convertPublishContent(content);
    [ShareSDK shareContent:publishContent
                      type:(ShareType)platType
               authOptions:nil
              shareOptions:nil
             statusBarTips:NO
                    result:^(ShareType type, SSResponseState state, id<ISSPlatformShareInfo> statusInfo, id<ICMErrorInfo> error, BOOL end) {
                        
                        CCDictionary *shareInfo = NULL;
                        CCDictionary *errorInfo = NULL;
                        
                        if (state == SSResponseStateSuccess)
                        {
                            shareInfo = convertNSDictToCCDict([statusInfo sourceData]);
                        }
                        
                        if (error)
                        {
                            errorInfo = CCDictionary::create();
                            errorInfo -> setObject(CCInteger::create([error errorCode]), "error_code");
                            errorInfo -> setObject(CCString::create([[error errorDescription] UTF8String]), "error_msg");
                        }
                        
                        if (callback)
                        {
                            callback ((C2DXResponseState)state, (C2DXPlatType)type, shareInfo, errorInfo);
                        }
                        
                    }];
}

void C2DXiOSShareSDK::oneKeyShareContent(CCArray *platTypes, CCDictionary *content, C2DXShareResultEvent callback)
{
    id<ISSContent> publishContent = convertPublishContent(content);
    
    NSMutableArray *shareList = nil;
    if (platTypes -> count() > 0)
    {
        shareList = [NSMutableArray array];
        for (int i = 0; i < platTypes -> count(); i++)
        {
            CCInteger *type = (CCInteger *)platTypes -> objectAtIndex(i);
            [shareList addObject:[NSNumber numberWithInteger:type -> getValue()]];
        }
    }
    
    [ShareSDK oneKeyShareContent:publishContent
                       shareList:shareList
                     authOptions:nil
                    shareOptions:nil
                   statusBarTips:NO
                          result:^(ShareType type, SSResponseState state, id<ISSPlatformShareInfo> statusInfo, id<ICMErrorInfo> error, BOOL end) {
                              
                              CCDictionary *shareInfo = NULL;
                              CCDictionary *errorInfo = NULL;
                              
                              if (state == SSResponseStateSuccess)
                              {
                                  shareInfo = convertNSDictToCCDict([statusInfo sourceData]);
                              }
                              
                              if (error)
                              {
                                  errorInfo = CCDictionary::create();
                                  errorInfo -> setObject(CCInteger::create([error errorCode]), "error_code");
                                  errorInfo -> setObject(CCString::create([[error errorDescription] UTF8String]), "error_msg");
                              }
                              
                              if (callback)
                              {
                                  callback ((C2DXResponseState)state, (C2DXPlatType)type, shareInfo, errorInfo);
                              }
                              
                          }];
}

void C2DXiOSShareSDK::showShareMenu(CCArray *platTypes, CCDictionary *content, C2DXShareResultEvent callback)
{
    id<ISSContent> publishContent = convertPublishContent(content);
    
    NSMutableArray *shareList = nil;
    if (platTypes && platTypes -> count() > 0)
    {
        shareList = [NSMutableArray array];
        for (int i = 0; i < platTypes -> count(); i++)
        {
            CCInteger *type = (CCInteger *)platTypes -> objectAtIndex(i);
            [shareList addObject:[NSNumber numberWithInteger:type -> getValue()]];
        }
    }
    
    [ShareSDK showShareActionSheet:nil
                         shareList:shareList
                           content:publishContent
                     statusBarTips:NO
                       authOptions:nil
                      shareOptions:nil
                            result:^(ShareType type, SSResponseState state, id<ISSPlatformShareInfo> statusInfo, id<ICMErrorInfo> error, BOOL end) {
                                
                                CCDictionary *shareInfo = NULL;
                                CCDictionary *errorInfo = NULL;
                                
                                if (state == SSResponseStateSuccess)
                                {
                                    shareInfo = convertNSDictToCCDict([statusInfo sourceData]);
                                }
                                
                                if (error)
                                {
                                    errorInfo = CCDictionary::create();
                                    errorInfo -> setObject(CCInteger::create([error errorCode]), "error_code");
                                    errorInfo -> setObject(CCString::create([[error errorDescription] UTF8String]), "error_msg");
                                }
                                
                                if (callback)
                                {
                                    callback ((C2DXResponseState)state, (C2DXPlatType)type, shareInfo, errorInfo);
                                }
                                
                            }];
}