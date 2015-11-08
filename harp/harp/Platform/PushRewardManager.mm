//
//  PushRewardManager.h
//  HelloCpp
//
//  Created by haxpor on 11/5/15.
//
//

#include "PushRewardManager.h"

using namespace harp;
using namespace cocos2d;

static PushRewardManager *s_sharedPushRewardManager;

PushRewardManager::PushRewardManager():
m_rewardDict(NULL)
{
    
}

PushRewardManager::~PushRewardManager()
{
    CCLOGINFO("Destroying PushRewardManager");
    
    CC_SAFE_RELEASE_NULL(m_rewardDict);
}

bool PushRewardManager::init()
{
    m_rewardDict = new CCDictionary();
    
    return true;
}

PushRewardManager* PushRewardManager::sharedInstance()
{
    if (!s_sharedPushRewardManager)
    {
        s_sharedPushRewardManager = new PushRewardManager();
        s_sharedPushRewardManager->init();
    }
    
    return s_sharedPushRewardManager;
}

void PushRewardManager::registerAsReward(intptr_t key, int value)
{
    m_rewardDict->setObject(CCInteger::create(value), key);
}

int PushRewardManager::valueForReward(intptr_t key)
{
    return ((CCInteger*)m_rewardDict->objectForKey(key))->getValue();
}

void PushRewardManager::flush(IPushRewardDelegate *delegate)
{
    CCAssert(delegate != NULL, "Delegate cannot be NULL");
    
    CCArray *keys = m_rewardDict->allKeys();
    if(keys != NULL)
    {
        for(int i=0; i<keys->count(); i++)
        {
            intptr_t key = ((CCInteger*)keys->objectAtIndex(i))->getValue();
            int value = ((CCInteger*)m_rewardDict->objectForKey(key))->getValue();
            delegate->handleReward(key, value, i == keys->count()-1);
        }
    }
    
    m_rewardDict->removeAllObjects();
}

void PushRewardManager::purgeManager()
{
    // delete PushRewardManager
    release();
}