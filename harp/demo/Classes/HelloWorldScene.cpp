#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "CCDeviceLocale.h"
#include "CCURLOpener.h"
#include "DummyScene.hpp"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace harp;

HelloWorld::~HelloWorld()
{
    CCLOGINFO("Destroying DummyScene");
    
    CC_SAFE_RELEASE_NULL(m_rewardIdLabel);
    CC_SAFE_RELEASE_NULL(m_rewardValueLabel);
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

void HelloWorld::handleReward(intptr_t rewardKey, int rewardValue, bool isLastReward)
{
    if(rewardKey == 1)
    {
        m_rewardIdLabel->setString("Coin");
        m_rewardValueLabel->setString(CCString::createWithFormat("%d", rewardValue)->getCString());
    }
    
    if(isLastReward)
    {
        CCLOG("This is a last reward");
    }
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
    
    // labels to show value of remote notification we got
    m_rewardIdLabel = new CCLabelTTF();
    m_rewardIdLabel->initWithString("", "Arial", 30);
    m_rewardIdLabel->setPosition(ccp(origin.x + visibleSize.width / 2.5 - m_rewardIdLabel->getContentSize().width/2, 70));
    
    m_rewardValueLabel = new CCLabelTTF();
    m_rewardValueLabel->initWithString("", "Arial", 30);
    m_rewardValueLabel->setPosition(ccp(origin.x + visibleSize.width / 1.5 - m_rewardIdLabel->getContentSize().width/2, 70));
    
    this->addChild(m_rewardIdLabel, 1);
    this->addChild(m_rewardValueLabel, 1);
    
    // flush push reward
    PushRewardManager::sharedInstance()->flush(this);

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
    
    // add a facebook button
    CCMenuItemFont *fbItem = CCMenuItemFont::create("Facebook", this, menu_selector(HelloWorld::openFacebookURL));
    fbItem->setPosition(ccp(origin.x + visibleSize.width/1.5 - fbItem->getContentSize().width/2,
                            origin.y + visibleSize.height/2 - fbItem->getContentSize().height/2));
    
    // add a twitter button
    CCMenuItemFont *twitterItem = CCMenuItemFont::create("Twitter", this, menu_selector(HelloWorld::openTwitterURL));
    twitterItem->setPosition(ccp(origin.x + visibleSize.width/2.5 - twitterItem->getContentSize().width/2,
                            origin.y + visibleSize.height/2 - twitterItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, fbItem, twitterItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", TITLE_FONT_SIZE);
    
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
    
    // test device's locale
    CCLOG("Device language (raw): %s", ccdeviceLocale_getLanguage());
    CCLOG("Device language (code): %d", ccdeviceLocale_getLanguageK());
    
    // reachability test
    // hostname
    ReachabilityCpp *hostnameReachability = ReachabilityCpp::createReachabilityWithHostName("zombie-hero.com");
    hostnameReachability->startNotifier();
    handlePrintResultFromReachability("hostname", hostnameReachability);
    
    // internet connection
    ReachabilityCpp *internetReachability = ReachabilityCpp::createReachabilityForInternetConnection();
    internetReachability->startNotifier();
    handlePrintResultFromReachability("internet", internetReachability);
    
    // local wifi
    ReachabilityCpp *localWifiReachability = ReachabilityCpp::createReachabilityForInternetConnection();
    localWifiReachability->startNotifier();
    handlePrintResultFromReachability("local WiFi", localWifiReachability);
    
    return true;
}

void HelloWorld::handlePrintResultFromReachability(const char *name, ReachabilityCpp *reachability)
{
    switch (reachability->currentReachabilityStatus())
    {
        case ErrorCpp:
            printf("%s - Error occurs\n", name);
            break;
        case NotReachableCpp:
            printf("%s - Not reachable\n", name);
            break;
        case ReachableViaWiFiCpp:
            printf("%s - Reachable via WiFi\n", name);
            break;
        case ReachableViaWWANCpp:
            printf("%s - Reachable vi WWAN\n", name);
            break;
    }
    
    printf("\n");
}

void HelloWorld::openFacebookURL(cocos2d::CCObject *pSender)
{
    CCURLOpener *urlOpener = CCURLOpener::create();
    
    // try to open app-link of facebook
    if(!urlOpener->openURL("fb://profile/374701732647919"))
    {
        // open it with browser
        urlOpener->openURL("https://www.facebook.com/secretcharsg");
    }
}

void HelloWorld::openTwitterURL(cocos2d::CCObject *pSender)
{
    CCURLOpener *urlOpener = CCURLOpener::create();
    
    // try to open app-link of facebook
    if(!urlOpener->openURL("twitter://user?screen_name=secretcharsg"))
    {
        // open it with browser
        urlOpener->openURL("https://twitter.com/secretcharsg");
    }
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // go to dummy scene
    CCDirector::sharedDirector()->replaceScene(DummyScene::scene());
}
