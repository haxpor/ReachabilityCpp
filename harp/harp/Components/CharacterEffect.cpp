//
//  CharacterEffect.cpp
//  ZombieHero
//
//  Created by haxpor on 9/1/13.
//
//

#include "CharacterEffect.h"

using namespace harp;
using namespace cocos2d;

CharacterEffect::CharacterEffect():
    _parent(NULL),
    m_animate(NULL),
    _cached_firstSpriteFrame(NULL),
    _cached_fullCycle_action(NULL),
    _cached_loopAnimation_action(NULL)
{
    
}

CharacterEffect::~CharacterEffect()
{
    CCLOGINFO("Destroy CharacterEffect");
}

void CharacterEffect::dealloc()
{
    stopAllActions();
    
    CC_SAFE_RELEASE_NULL(m_animate);
    CC_SAFE_RELEASE_NULL(_cached_fullCycle_action);
    CC_SAFE_RELEASE_NULL(_cached_loopAnimation_action);
    
    CCSprite::dealloc();
}

bool CharacterEffect::init(Character *c, bool isBackgroundEffect, bool isLoopAnimation, float delayBeforeFadeOut, float fadeIn, float fadeOut)
{
    if(initFirstFrameAndAnimation(&m_animate))
    {
        m_animate->retain();
        
        // half-size asset
        setScale(CHARACTER_SCALE_FACTOR);
        
        // cache first sprite frame
        // note: used to reset back to particular frame after animation finishes
        CCAnimationFrame* firstAnimationFrame = (CCAnimationFrame*) m_animate->getAnimation()->getFrames()->objectAtIndex(0);
        _cached_firstSpriteFrame = firstAnimationFrame->getSpriteFrame();
        
        // set parent
        _parent = c;
        
        // set attributes
        _delayBeforeFadeOut = delayBeforeFadeOut;
        _fadeIn = fadeIn;
        _fadeOut = fadeOut;
        _isBackgroundEffect = isBackgroundEffect;
        _isLoopAnimation = isLoopAnimation;
        
        // cache action
        if(isLoopAnimation)
        {
            // always and immediately play forever action
            _cached_loopAnimation_action = CCRepeatForever::create(m_animate);
            _cached_loopAnimation_action->retain();
            
            // delay to fade out
            CCCallFunc *start = CCCallFunc::create(this, callfunc_selector(CharacterEffect::setStatesBeforeBegin));
            CCFadeIn *fadeIn = CCFadeIn::create(_fadeIn);
            CCDelayTime *delayTime = CCDelayTime::create(_delayBeforeFadeOut);
            CCFadeOut *fadeOut = CCFadeOut::create(_fadeOut);
            CCCallFunc *callFunc = CCCallFunc::create(this, callfunc_selector(CharacterEffect::removeFromParent));
            
            _cached_fullCycle_action = CCSequence::create(start, fadeIn, delayTime, fadeOut, callFunc, NULL);
            _cached_fullCycle_action->retain();
            
        }
        else
        {
            CCCallFunc *start = CCCallFunc::create(this, callfunc_selector(CharacterEffect::setStatesBeforeBegin));
            CCFadeIn *fadeIn = CCFadeIn::create(_fadeIn);
            CCRepeat *repeat = CCRepeat::create(m_animate, 1);
            CCDelayTime *delayTime = CCDelayTime::create(_delayBeforeFadeOut);
            CCFadeOut *fadeOut = CCFadeOut::create(_fadeOut);
            CCCallFunc *callFunc = CCCallFunc::create(this, callfunc_selector(CharacterEffect::removeFromParent));
            
            _cached_fullCycle_action = CCSequence::create(start, fadeIn, repeat, delayTime, fadeOut, callFunc, NULL);
            _cached_fullCycle_action->retain();
        }
        
        return true;
    }
    else
        return false;
}

void CharacterEffect::addSelfToParent(CCPoint adjustPos)
{
    this->addSelfToParent(adjustPos, false, true, 0);
}

void CharacterEffect::addSelfToParent(cocos2d::CCPoint adjustPos, bool isFollowParent)
{
    this->addSelfToParent(adjustPos, isFollowParent, true, 0);
}

void CharacterEffect::addSelfToParent(CCPoint adjustPos, bool isFollowParent, bool isFlipXAheringParent)
{
    this->addSelfToParent(adjustPos, isFollowParent, isFlipXAheringParent, 0);
}

void CharacterEffect::addSelfToParent(CCPoint adjustPos, bool isFollowParent, bool isFlipXAdheringParent, int adjustZOrder)
{
    if((_parent != NULL && _parent->getParent() != NULL && isFollowParent) ||
       (_parent != NULL && !isFollowParent))
    {
        if(isFollowParent)
        {
            // ignore prior setting of scale, as it aheres the parent now
            setScale(1.0f);
            // ignore anchor point
            ignoreAnchorPointForPosition(true);
        }
        else
        {
            // ignore anchor point
            ignoreAnchorPointForPosition(false);
        }
        
        // if already added to the parent then remove it first
        if(CCNode::getParent() != NULL)
        {
            removeFromParentAndCleanup(false);
            stopAllActions();
        }
        
        // set back to first frame
        setDisplayFrame(_cached_firstSpriteFrame);
        
        // flipX if adhere to parent
        if(isFlipXAdheringParent)
        {
            // flip the x-position if sprite is flipped
            if(_parent->isFlipX())
            {
                setFlipX(true);
                
                if(isFollowParent)
                    setPosition(ccp(-adjustPos.x, adjustPos.y));
                else
                    setPosition(ccp(_parent->getPositionX() - adjustPos.x, _parent->getPositionY() + adjustPos.y));
            }
            else
            {
                setFlipX(false);
                
                if(isFollowParent)
                    setPosition(ccp(adjustPos.x, adjustPos.y));
                else
                    setPosition(ccp(_parent->getPositionX() + adjustPos.x, _parent->getPositionY() + adjustPos.y));
            }
        }
        // not to flipX adhering to the parent
        else
        {
            if(isFollowParent)
                setPosition(ccp(adjustPos.x, adjustPos.y));
            else
                setPosition(ccp(_parent->getPositionX() + adjustPos.x, _parent->getPositionY() + adjustPos.y));
        }
        
        if(isFollowParent)
            _parent->addChild(this, _isBackgroundEffect ? BACKGROUND_LAYER + adjustZOrder: _parent->getZOrder() + adjustZOrder);
        else
            _parent->getParent()->addChild(this, _isBackgroundEffect ? BACKGROUND_LAYER + adjustZOrder: _parent->getZOrder() + adjustZOrder);
        
        // run action
        if(this->getIsLoopAnimation())
            this->runAction(_cached_loopAnimation_action);
        this->runAction(_cached_fullCycle_action);
    }
}

Character* CharacterEffect::getParent()
{
    return _parent;
}

float CharacterEffect::getDelayBeforeFadeOut()
{
    return _delayBeforeFadeOut;
}

float CharacterEffect::getFadeInDuration()
{
    return _fadeIn;
}

float CharacterEffect::getFadeOutDuration()
{
    return _fadeOut;
}

bool CharacterEffect::isBackgroundEffect()
{
    return _isBackgroundEffect;
}

bool CharacterEffect::getIsLoopAnimation()
{
    return _isLoopAnimation;
}

void CharacterEffect::removeFromParent()
{
    if(_parent != NULL && CCNode::getParent() != NULL)
    {
        // reset states
        this->resetStates();
        
        this->removeFromParentAndCleanup(true);
    }
}

void CharacterEffect::setStatesBeforeBegin()
{
    // do nothing here
}

void CharacterEffect::resetStates()
{
    // do nothing here.
}
