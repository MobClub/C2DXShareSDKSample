#include "HelloWorldScene.h"
#include "C2DXShareSDK.h"


USING_NS_CC;
using namespace cn::sharesdk;

void authResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *error)
{
    switch (state) {
        case C2DXResponseStateSuccess:
            CCLog("授权成功");
            break;
        case C2DXResponseStateFail:
            CCLog("授权失败");
            break;
        default:
            break;
    }
}

void getUserResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *userInfo, CCDictionary *error)
{
    if (state == C2DXResponseStateSuccess)
    {
        //输出用户信息
        try
        {
            CCArray *allKeys = userInfo -> allKeys();
            for (int i = 0; i < allKeys -> count(); i++)
            {
                CCString *key = (CCString *)allKeys -> objectAtIndex(i);
                CCObject *obj = userInfo -> objectForKey(key -> getCString());
                
                CCLog("key = %s", key -> getCString());
                if (dynamic_cast<CCString *>(obj))
                {
                    CCLog("value = %s", dynamic_cast<CCString *>(obj) -> getCString());
                }
                else if (dynamic_cast<CCInteger *>(obj))
                {
                    CCLog("value = %d", dynamic_cast<CCInteger *>(obj) -> getValue());
                }
                else if (dynamic_cast<CCDouble *>(obj))
                {
                    CCLog("value = %f", dynamic_cast<CCDouble *>(obj) -> getValue());
                }
            }
        }
        catch(...)
        {
            CCLog("==============error");
        }
        
    }
}

void shareResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error)
{
    switch (state) {
        case C2DXResponseStateSuccess:
            CCLog("分享成功");
            break;
        case C2DXResponseStateFail:
            CCLog("分享失败");
            break;
        default:
            break;
    }
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCMenuItemLabel *authMenuItem = CCMenuItemLabel::create(CCLabelTTF::create("授权", "Arial", 40),
                                                            this,
                                                            menu_selector(HelloWorld::authMenuItemClick));
    CCMenuItemLabel *cancelAuthMenuItem = CCMenuItemLabel::create(CCLabelTTF::create("取消授权", "Arial", 40),
                                                                  this,
                                                                  menu_selector(HelloWorld::cancelAuthMenuItemClick));
    CCMenuItemLabel *hasAuthMenuItem = CCMenuItemLabel::create(CCLabelTTF::create("是否授权", "Arial", 40),
                                                               this,
                                                               menu_selector(HelloWorld::hasAuthMenuItemClick));
    CCMenuItemLabel *getUserMenuItem = CCMenuItemLabel::create(CCLabelTTF::create("用户信息", "Arial", 40),
                                                               this,
                                                               menu_selector(HelloWorld::getUserInfoMenuItemClick));
    CCMenuItemLabel *shareMenuItem = CCMenuItemLabel::create(CCLabelTTF::create("分享", "Arial", 40),
                                                             this,
                                                             menu_selector(HelloWorld::shareMenuItemClick));

    CCMenu *itemsMenu = CCMenu::create(authMenuItem, cancelAuthMenuItem, hasAuthMenuItem, getUserMenuItem, shareMenuItem, NULL);
    itemsMenu -> alignItemsHorizontallyWithPadding(20);
    itemsMenu -> setPosition(ccp(CCDirector::sharedDirector() -> getWinSize().width / 2, 100));
    this -> addChild(itemsMenu);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void HelloWorld::authMenuItemClick(CCObject* pSender)
{
//    C2DXShareSDK::authorize(C2DXPlatTypeFacebook, authResultHandler);
}

void HelloWorld::cancelAuthMenuItemClick(CCObject* pSender)
{
//    C2DXShareSDK::cancelAuthorize(C2DXPlatTypeSinaWeibo);
}

void HelloWorld::hasAuthMenuItemClick(CCObject* pSender)
{
    if (C2DXShareSDK::hasAutorized(C2DXPlatTypeSinaWeibo))
    {
        CCLog("用户已授权");
    }
    else
    {
        CCLog("用户尚未授权");
    }
}

void HelloWorld::getUserInfoMenuItemClick(CCObject* pSender)
{
    C2DXShareSDK::getUserInfo(C2DXPlatTypeSinaWeibo, getUserResultHandler);
}

void HelloWorld::shareMenuItemClick(CCObject* pSender)
{
    CCDictionary *content = CCDictionary::create();
    content -> setObject(CCString::create("这是一条测试内容"), "content");
    content -> setObject(CCString::create(C2DXShareSDK::pathString()), "image");
    content -> setObject(CCString::create("测试标题"), "title");
    content -> setObject(CCString::create("测试描述"), "description");
    content -> setObject(CCString::create("http://sharesdk.cn"), "url");
    content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
    content -> setObject(CCString::create("http://sharesdk.cn"), "siteUrl");
    content -> setObject(CCString::create("ShareSDK"), "site");
    content -> setObject(CCString::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
    content -> setObject(CCString::create("extInfo"), "extInfo");
    
    C2DXShareSDK::showShareMenu(NULL, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
//    C2DXShareSDK::showShareView(C2DXPlatTypeSinaWeibo, content, shareResultHandler);
}
