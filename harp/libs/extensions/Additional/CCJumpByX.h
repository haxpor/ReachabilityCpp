//
// CCJumpByXX.h
// Author @haxpor
//
// Zombie Hero : Revenge of Kiki

// This is a modified version of CCJumpByX class of cocos2dx's original source file (v.2.0.3).
// Modified version support jumping along x-direction which regards positive x as a ground.

#include "cocos2d.h"

NS_CC_BEGIN

/** @brief Moves a CCNode object simulating a parabolic jump movement by modifying it's position attribute.
*/
class CC_DLL CCJumpByX : public CCActionInterval
{
public:
    /** initializes the action */
    bool initWithDuration(float duration, const CCPoint& position, float height, unsigned int jumps);

    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);
    virtual CCActionInterval* reverse(void);

public:
    /** creates the action 
    @deprecated: This interface will be deprecated sooner or later.
    */
    CC_DEPRECATED_ATTRIBUTE static CCJumpByX* actionWithDuration(float duration, const CCPoint& position, float height, unsigned int jumps);
    /** creates the action */
    static CCJumpByX* create(float duration, const CCPoint& position, float height, unsigned int jumps);
protected:
    CCPoint            m_startPosition;
    CCPoint            m_delta;
    float            m_height;
    unsigned int    m_nJumps;
};

NS_CC_END
