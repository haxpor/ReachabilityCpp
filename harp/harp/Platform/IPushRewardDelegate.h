//
//  IPushRewardDelegate.h
//  HelloCpp
//
//  Created by haxpor on 11/5/15.
//
//

#ifndef IPushRewardDelegate_h
#define IPushRewardDelegate_h

NS_S_HARPENGINE

class IPushRewardDelegate
{
public:
    virtual ~IPushRewardDelegate() {}
    
    /**
     User handle reward one by one.
     */
    virtual void handleReward(intptr_t rewardKey, int rewardValue, bool isLastReward) = 0;
};

NS_E_HARPENGINE

#endif /* IPushRewardDelegate_h */
