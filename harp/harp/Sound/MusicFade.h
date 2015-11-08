//
//  MusicFade.h
//
//  Created by Duncan Holby on 1/30/14.
//  http://fuguelike.com
//

#ifndef __MusicFade__
#define __MusicFade__


#include "cocos2d.h"
#include "CoreMacros.h"

NS_S_HARPENGINE


class MusicFade : public cocos2d::CCActionInterval
{
public:
    MusicFade();
    
    static MusicFade* create(float d, float volume, bool pauseOnComplete = false );
    bool initWithDuration(float d, float volume, bool pauseOnComplete );
    
    virtual void startWithTarget(cocos2d::CCNode *pTarget);
    virtual void update(float time);
    virtual void stop(void);
    
protected:
    float m_targetVal;
    float m_initialVal;
    bool m_bPauseOnComplete;
};

NS_E_HARPENGINE

#endif /* defined(__MusicFade__) */
