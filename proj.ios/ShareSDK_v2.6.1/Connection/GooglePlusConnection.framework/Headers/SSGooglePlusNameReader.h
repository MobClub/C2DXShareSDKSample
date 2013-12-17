//
//  SSGooglePlusNameReader.h
//  GooglePlusConnection
//
//  Created by 冯 鸿杰 on 13-10-25.
//  Copyright (c) 2013年 掌淘科技. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 *	@brief	名字信息读取器
 */
@interface SSGooglePlusNameReader : NSObject
{
@private
    NSDictionary *_sourceData;
}

/**
 *	@brief	源数据
 */
@property (nonatomic,readonly) NSDictionary *sourceData;

/**
 *	@brief	The full name of this person, including middle names, suffixes, etc.
 */
@property (nonatomic,readonly) NSString *formatted;

/**
 *	@brief	The family name (last name) of this person.
 */
@property (nonatomic,readonly) NSString *familyName;

/**
 *	@brief	The given name (first name) of this person.
 */
@property (nonatomic,readonly) NSString *givenName;

/**
 *	@brief	The middle name of this person.
 */
@property (nonatomic,readonly) NSString *middleName;

/**
 *	@brief	The honorific prefixes (such as "Dr." or "Mrs.") for this person.
 */
@property (nonatomic,readonly) NSString *honorificPrefix;

/**
 *	@brief	The honorific suffixes (such as "Jr.") for this person.
 */
@property (nonatomic,readonly) NSString *honorificSuffix;

/**
 *	@brief	初始化读取器
 *
 *	@param 	sourceData 	原数据
 *
 *	@return	读取器实例对象
 */
- (id)initWithSourceData:(NSDictionary *)sourceData;


/**
 *	@brief	创建名字信息读取器
 *
 *	@param 	sourceData 	原数据
 *
 *	@return	读取器实例对象
 */
+ (SSGooglePlusNameReader *)readerWithSourceData:(NSDictionary *)sourceData;

@end
