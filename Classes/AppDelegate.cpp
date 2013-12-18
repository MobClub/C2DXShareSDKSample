#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace cn::sharesdk;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    
    //初始化ShareSDK
    C2DXShareSDK::open(CCString::create("api20"), false);
    
    CCDictionary *sinaConfigDict = CCDictionary::create();
    sinaConfigDict -> setObject(CCString::create("568898243"), "app_key");
    sinaConfigDict -> setObject(CCString::create("38a4f8204cc784f81f9f0daaf31e02e3"), "app_secret");
    sinaConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSinaWeibo, sinaConfigDict);
    
    CCDictionary *tcConfigDict = CCDictionary::create();
    tcConfigDict -> setObject(CCString::create("801307650"), "app_key");
    tcConfigDict -> setObject(CCString::create("ae36f4ee3946e1cbb98d6965b0b2ff5c"), "app_secret");
    tcConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTencentWeibo, tcConfigDict);
    
    
    
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
