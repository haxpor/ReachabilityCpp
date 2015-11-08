#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PushRewardManager.h"
#include "ReachabilityCpp.h"

class HelloWorld : public cocos2d::CCLayer, private harp::IPushRewardDelegate
{
public:
    virtual ~HelloWorld();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void openFacebookURL(CCObject* pSender);
    void openTwitterURL(CCObject* pSender);
    
    void handlePrintResultFromReachability(const char* name, ReachabilityCpp* reachability);
    
    // handle rewards as registered via PushRewardManager
    virtual void handleReward(intptr_t rewardKey, int rewardValue, bool isLastReward);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
protected:
    cocos2d::CCLabelTTF *m_rewardIdLabel;
    cocos2d::CCLabelTTF *m_rewardValueLabel;
};

#endif // __HELLOWORLD_SCENE_H__
