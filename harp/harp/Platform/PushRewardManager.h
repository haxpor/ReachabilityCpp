//
//  PushRewardManager.h
//  HelloCpp
//
//  Created by haxpor on 11/5/15.
//
//

#ifndef PushRewardManager_h
#define PushRewardManager_h

#include "CoreMacros.h"
#include "cocos2d.h"
#include "IPushRewardDelegate.h"

NS_S_HARPENGINE

class PushRewardManager : cocos2d::CCObject
{
public:
    PushRewardManager();
    virtual ~PushRewardManager();
    
    virtual bool init();
    
    /**
     Register reward with corresponding value.
     */
    void registerAsReward(intptr_t key, int value);
    
    /**
     Get value for reward key.
     */
    int valueForReward(intptr_t key);
    
    /**
     Flush all the registered rewards and let user handle it via specified delegate.
     */
    void flush(IPushRewardDelegate *delegate);
    
    void purgeManager();
    
    static PushRewardManager* sharedInstance();
    
protected:
    cocos2d::CCDictionary *m_rewardDict;
};

NS_E_HARPENGINE

#endif /* PushRewardManager_h */
