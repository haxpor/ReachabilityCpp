//
// CCJumpByXX.cpp
// Author @haxpor
//
// Zombie Hero : Revenge of Kiki

// This is a modified version of CCJumpByX class of cocos2dx's original source file (v.2.0.3).
// Modified version support jumping along x-direction which regards positive x as a ground.


#include "CCJumpByX.h"

NS_CC_BEGIN

//
// JumpBy
//
CCJumpByX* CCJumpByX::actionWithDuration(float duration, const CCPoint& position, float height, unsigned int jumps)
{
    return CCJumpByX::create(duration, position, height, jumps);
}

CCJumpByX* CCJumpByX::create(float duration, const CCPoint& position, float height, unsigned int jumps)
{
    CCJumpByX *pJumpBy = new CCJumpByX();
    pJumpBy->initWithDuration(duration, position, height, jumps);
    pJumpBy->autorelease();

    return pJumpBy;
}

bool CCJumpByX::initWithDuration(float duration, const CCPoint& position, float height, unsigned int jumps)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_delta = position;
        m_height = height;
        m_nJumps = jumps;

        return true;
    }

    return false;
}

CCObject* CCJumpByX::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    CCJumpByX* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCJumpByX*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCJumpByX();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_delta, m_height, m_nJumps);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCJumpByX::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_startPosition = pTarget->getPosition();
}

void CCJumpByX::update(float time)
{
    // parabolic jump (since v0.8.2)
    if (m_pTarget)
    {
        float frac = fmodf(time * m_nJumps, 1.0f);
        float y = m_height * 4 * frac * (1 - frac);
        y += m_delta.y * time;
        float x = m_delta.x * time;
        m_pTarget->setPosition(ccp(m_startPosition.x - y, m_startPosition.y + x));
    }
}

CCActionInterval* CCJumpByX::reverse(void)
{
    return CCJumpByX::create(m_fDuration, ccp(-m_delta.y, -m_delta.x),
        m_height, m_nJumps);
}

NS_CC_END
