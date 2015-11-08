//
//  CharacterEffect.h
//  ZombieHero
//
//  Created by haxpor on 9/1/13.
//
//

#ifndef __ZombieHero__CharacterEffect__
#define __ZombieHero__CharacterEffect__

#include "cocos2d.h"
#include "Character.h"

NS_S_HARPENGINE

class CharacterEffect : public cocos2d::CCSprite
{
public:
    CharacterEffect();
    virtual ~CharacterEffect();
    virtual void dealloc();
    
    /* Init */
    bool init(harp::Character* c, bool isBackgroundEffect=false, bool isLoopAnimation=false, float delayBeforeFadeOut=0.0f, float fadeIn=0.3f, float fadeOut=0.5f);
    
    /* Implement this function to add animation and return true */
    virtual bool initFirstFrameAndAnimation(cocos2d::CCAnimate** ptr_out_animate) = 0;
    
    /* Add self into parent character node. */
    void addSelfToParent(cocos2d::CCPoint adjustPos);
    void addSelfToParent(cocos2d::CCPoint adjustPos, bool isFollowParent);
    void addSelfToParent(cocos2d::CCPoint adjustPos, bool isFollowParent, bool isFlipXAdheringParent);
    void addSelfToParent(cocos2d::CCPoint adjustPos, bool isFollowParent, bool isFlipXAdheringParent, int adjustZOrder);
    
    /**
     If you override this, you should no what you're doing.
     Note: The main reason is to override it to hook up with recyclable object to be recycled by object pool.
     */
    virtual void removeFromParent();
    
    /**
     Set any necessary setup states before begin the chain of action.
     Sub-class implement this for specific preparation before getting started.
     */
    virtual void setStatesBeforeBegin();
    
    /**
     Sub-class implement this to define more specific to reset states before being removed from parent.
     */
    virtual void resetStates();
    
    /* Get parent node */
    harp::Character* getParent();
    /* Get attributes */
    float getDelayBeforeFadeOut();
    float getFadeInDuration();
    float getFadeOutDuration();
    cocos2d::CCPoint getAdjustPosition();
    bool isBackgroundEffect();
    bool getIsLoopAnimation();

protected:
    cocos2d::CCAnimate* m_animate;

private:
    harp::Character* _parent;
    float _delayBeforeFadeOut;
    float _fadeIn;
    float _fadeOut;
    bool _isBackgroundEffect;
    bool _isLoopAnimation;
    cocos2d::CCSpriteFrame* _cached_firstSpriteFrame;
    cocos2d::CCAction* _cached_fullCycle_action;
    cocos2d::CCAction* _cached_loopAnimation_action;    // only for when 'isLoopAnimation' is set
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__CharacterEffect__) */
