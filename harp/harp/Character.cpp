//
//  Character.cpp
//  ZombieHero
//
//  Created by haxpor on 10/22/12.
//
//

#include "Character.h"
#include "Core.h"
#include "MathUtil.h"

NS_S_HARPENGINE

using namespace cocos2d;

Character::Character():
    m_hp(100),
    m_incAttackPoint(10),
    m_defPoint(0),
    m_offsetDivFactor(1),
    m_facingDirection(RIGHT),    // always default to RIGHT as of asset created is in that way
    m_charOffset(CCPointZero),   // from the lower left of the sprite, FIXME: May not need this anymore, check back later
    m_staticCollisionSize(CCSizeZero),
    m_shadow(NULL),
    m_isInMidAir(false),
    m_isJumpAlongXY(false),
    m_managedDisplayedFrameCMDs(NULL),
    m_isRemainInStageWhenRecycled(false),
    _preCached_nonFlipX_anchorPoint(CCPointZero),
    _preCached_flipX_anchorPoint(CCPointZero),
    _preCached_nonFlipX_relShadowPos(CCPointZero),
    _preCached_flipX_relShadowPos(CCPointZero),
    _cached_shadow_scaleLoop(NULL),
    _cached_shadow_scaleToSmallerThenNormalOneTime(NULL),
    _cached_shadow_scaleToBiggerThenNormalOneTime(NULL),
    _cached_shadow_scaleToNormalOneTime(NULL),
    _cached_shadow_scaleForMidAirThenNormalOneTime(NULL),
    _shadow_savedSetScale(CHARACTER_SCALE_FACTOR)
{
    // set the offset-div-factor
    if(Core::sharedCore()->getIsDeviceTypeiPad())
    {
        m_offsetDivFactor = 2;
    }
    else
    {
        m_offsetDivFactor = 4;
    }
}

Character::~Character()
{
    CCLOGINFO("Destroying Character");
}

void Character::dealloc()
{
    CC_SAFE_RELEASE_NULL(_cached_shadow_scaleLoop);
    CC_SAFE_RELEASE_NULL(_cached_shadow_scaleToSmallerThenNormalOneTime);
    CC_SAFE_RELEASE_NULL(_cached_shadow_scaleToBiggerThenNormalOneTime);
    CC_SAFE_RELEASE_NULL(_cached_shadow_scaleToNormalOneTime);
    CC_SAFE_RELEASE_NULL(_cached_shadow_scaleForMidAirThenNormalOneTime);
    
    // clear out displayed-frame-cmds
    if(m_managedDisplayedFrameCMDs)
    {
        for(int i=0; i<m_managedDisplayedFrameCMDs->count(); i++)
        {
            CCAnimationFrame *frame = static_cast<CCAnimationFrame*>(m_managedDisplayedFrameCMDs->objectAtIndex(i));
            
            // clear out all data in dict
            frame->getUserInfo()->removeAllObjects();
            
            // set null to user info
            frame->setUserInfo(NULL);
        }
        
        m_managedDisplayedFrameCMDs->removeAllObjects();
        CC_SAFE_RELEASE_NULL(m_managedDisplayedFrameCMDs);
    }
    
    CC_SAFE_RELEASE_NULL(m_shadow);
}

bool Character::initWithSpriteFrameName(const char *spriteFrameName, const cocos2d::CCPoint &hdCharCenterPos, int hp, float maxSpeed, int incAttackPoint, int defPoint)
{
    if(CCSprite::initWithSpriteFrameName(spriteFrameName))
    {
        // every character object is half-size asset
        setScale(CHARACTER_SCALE_FACTOR);
        
        // set maxSpeed
        this->setMaxSpeed(maxSpeed);
        
        // set the center position of character
        // calculate the sprite offset and use in new anchor point (offset position is based on HD position)
        // note: also divide by 2 as all enemies asset are in half-size
        m_charOffset = ccp(hdCharCenterPos.x / CHARACTER_ASSET_SCALE_FACTOR / m_offsetDivFactor, hdCharCenterPos.y / CHARACTER_ASSET_SCALE_FACTOR / m_offsetDivFactor);
        // pre-calculate and caching anchor point when flip-x
        _preCached_nonFlipX_anchorPoint = ccp(m_charOffset.x / this->getContentSize().width, m_charOffset.y / this->getContentSize().height);
        _preCached_flipX_anchorPoint = ccp(1.0f-m_charOffset.x / this->getContentSize().width, m_charOffset.y / this->getContentSize().height);
        // set a new anchor point based on character's center position
        this->setAnchorPoint(_preCached_nonFlipX_anchorPoint);
        
        // set new collision size using the specified frame name as static size
        this->setStaticCollisionSize(CCSizeMake(getTextureRect().size.width * this->getScaleX(), getTextureRect().size.height * this->getScaleY()));
        
        // pre-calculate position x of shadow for case of flip-x and non flip-x
        _preCached_nonFlipX_relShadowPos = ccp(0, -this->getTextureRect().size.height * 0.5f);
        _preCached_flipX_relShadowPos = _preCached_nonFlipX_relShadowPos;

        // create shadow
        m_shadow = CCSprite::createWithSpriteFrameName("Shadow-cropped.png");
        m_shadow->retain();
        m_shadow->setScale(_shadow_savedSetScale);
        m_shadow->setPosition(ccp(_preCached_nonFlipX_relShadowPos.x + getPositionX(), _preCached_nonFlipX_relShadowPos.y + getPositionY()));
        
        // pre-init to receieve data in derived class
        m_managedDisplayedFrameCMDs = CCArray::create();
        m_managedDisplayedFrameCMDs->retain();
        
        // init with character's basic attributes
        m_hp = hp;
        m_incAttackPoint = incAttackPoint;
        m_defPoint = defPoint;
        
        // cache shadow's actions
        this->_cacheScalingShadowActions(_shadow_savedSetScale);
        
        return true;
    }
    return false;
}

void Character::setFlipX(bool bFlipX)
{
    // set proper anchor point, only when things change
    if(bFlipX != this->isFlipX())
    {
        if(bFlipX)
        {
            this->setAnchorPoint(_preCached_flipX_anchorPoint);
            m_shadow->setPositionX(getPositionX() + _preCached_flipX_relShadowPos.x);
            if(!isInMidAir())
                m_shadow->setPositionY(getPositionY() + _preCached_flipX_relShadowPos.y);
            this->setFacingDirection(LEFT);
        }
        else
        {
            this->setAnchorPoint(_preCached_nonFlipX_anchorPoint);
            m_shadow->setPositionX(getPositionX() + _preCached_nonFlipX_relShadowPos.x);
            if(!isInMidAir())
                m_shadow->setPositionY(getPositionY() + _preCached_nonFlipX_relShadowPos.y);
            this->setFacingDirection(RIGHT);
        }
    }
    
    // call the original one
    CCSprite::setFlipX(bFlipX);
}

void Character::moveWithDirection(const kmVec2& vecDir, float dt)
{
    CCPoint pos = this->getPosition();
    // check timeScale to avoid multiply with 1.0 all the time
    float timeScale = CCDirector::sharedDirector()->getScheduler()->getTimeScale();
    if(timeScale != 1.0f)
    {
        pos.x += m_maxSpeed * timeScale * vecDir.x * dt;
        pos.y += m_maxSpeed * timeScale * vecDir.y * dt;
    }
    else
    {
        pos.x += m_maxSpeed * vecDir.x * dt;
        pos.y += m_maxSpeed * vecDir.y * dt;
    }
    this->setPosition(pos);
}

void Character::moveWithDirection(const CCPoint& vecDir, float dt)
{
    CCPoint pos = this->getPosition();
    // check timeScale to avoid multiply with 1.0 all the time
    float timeScale = CCDirector::sharedDirector()->getScheduler()->getTimeScale();
    if(timeScale != 1.0f)
    {
        pos.x += m_maxSpeed * timeScale * vecDir.x * dt;
        pos.y += m_maxSpeed * timeScale * vecDir.y * dt;
    }
    else
    {
        pos.x += m_maxSpeed * vecDir.x * dt;
        pos.y += m_maxSpeed * vecDir.y * dt;
    }
    this->setPosition(pos);
}

void Character::moveWithVelocity(const CCPoint& velocity, float dt)
{
    CCPoint pos = this->getPosition();
    // check timeScale to avoid multiply with 1.0 all the time
    float timeScale = CCDirector::sharedDirector()->getScheduler()->getTimeScale();
    if(timeScale != 1.0f)
    {
        pos.x += velocity.x * timeScale * dt;
        pos.y += velocity.y * timeScale * dt;
    }
    else
    {
        pos.x += velocity.x * dt;
        pos.y += velocity.y * dt;
    }
    this->setPosition(pos);
}

int Character::getHP()
{
    return this->m_hp;
}

void Character::setHP(int hp)
{
    if(hp != this->m_hp)
    {
        this->m_hp = hp;
    }
}

int Character::getIncAttackPoint()
{
    return this->m_incAttackPoint;
}

void Character::setIncAttackPoint(int attack)
{
    if(attack != this->m_incAttackPoint)
    {
        this->m_incAttackPoint = attack;
    }
}

int Character::getDefensePoint()
{
    return m_defPoint;
}

void Character::setDefensePoint(int defPoint)
{
    m_defPoint = defPoint;
}

Direction Character::getFacingDirection()
{
    return m_facingDirection;
}

void Character::setFacingDirection(Direction dir)
{
    if(dir != m_facingDirection)
        m_facingDirection = dir;
}

const CCPoint& Character::getCharacterOffset()
{
    return m_charOffset;
}

void Character::setCharacterOffset(const CCPoint& offset)
{
    if(m_charOffset.x != offset.x &&
       m_charOffset.y != offset.y)
    {
        m_charOffset.setPoint(offset.x, offset.y);
    }
}

const CCSize& Character::getStaticCollisionSize()
{
    return m_staticCollisionSize;
}
void Character::setStaticCollisionSize(const CCSize& size)
{
    m_staticCollisionSize = size;
}

// Beware : This call is expensive. Try to cache the result and use it from there.
CCSize Character::getDynamicCollisionSize()
{
    CCSize size = this->displayFrame()->getRect().size;
    return CCSizeMake(size.width * getScaleX(), size.height * getScaleY());
}

bool Character::isDead()
{
    if(m_hp <= 0)
        return true;
    else
        return false;
}

bool Character::isAlive()
{
    if(m_hp > 0)
        return true;
    else
        return false;
}

float Character::getMaxSpeed()
{
    return m_maxSpeed;
}
void Character::setMaxSpeed(float maxSpeed)
{
    m_maxSpeed = maxSpeed;
}

bool Character::isAttacking()
{
    // sub-class overwrites this
    return false;
}

bool Character::isUnderAttackOrDown()
{
    // sub-class overwrites this
    return false;
}

bool Character::isUnderAttack()
{
    // sub-class overwrites this
    return false;
}

bool Character::isDown()
{
    // sub-class overwrites this
    return false;
}

bool Character::isWakeup()
{
    // sub-class overwrites this
    return false;
}

void Character::onBeingHit(int decHP, CCSprite* attacker, void* data)
{
    // do nothing here
    // sub-class overwrites this
}

void Character::adjustInitialShadowPosition(const CCPoint& offset)
{
    // re-calculate pre-cached shadow position
    _preCached_nonFlipX_relShadowPos = ccp(Core::sharedCore()->convertPointX(offset.x),
                                  -this->getTextureRect().size.height * 0.5f * CHARACTER_SCALE_FACTOR + Core::sharedCore()->convertPointY(offset.y));
    _preCached_flipX_relShadowPos = ccp(Core::sharedCore()->convertPointX(-offset.x),
                                  -this->getTextureRect().size.height * 0.5f * CHARACTER_SCALE_FACTOR + Core::sharedCore()->convertPointY(offset.y));
}

void Character::adjustInitialShadowScale(float scale)
{
    if(m_shadow)
    {
        // save the set scale of shadow
        // we will reset to it later inside recycle() method
        _shadow_savedSetScale = scale;
        m_shadow->setScale(scale);
        
        // recache shadow's scaling actions
        this->_cacheScalingShadowActions(_shadow_savedSetScale);
    }
}

float Character::getBoundingRadius()
{
    // return the maximum value of dynamic collision size, return value get applied with scale for free
    CCSize size = this->getDynamicCollisionSize();
    return MAX(size.width, size.height);
}

void Character::reorderSelf()
{
    float winHeight = CCDirector::sharedDirector()->getWinSize().height;
    
    this->getParent()->reorderChild(this, winHeight - getPositionY() + CHARACTER_LAYER);
}

void Character::addAsChildTo(CCNode *node, CCSpriteBatchNode* shadowBatchNode, int zOrder)
{
    // add self
    node->addChild(this, zOrder, getTag());
    
    // add shadow
    if(m_shadow)
        shadowBatchNode->addChild(m_shadow, CHARACTER_SHADOW_LAYER);
}

void Character::setPositionX(float x)
{
    CCSprite::setPositionX(x);
    
    // set shadow's position x
    if(m_shadow)
    {
        if(isFlipX())
            m_shadow->setPositionX(x + _preCached_flipX_relShadowPos.x);
        else
            m_shadow->setPositionX(x + _preCached_nonFlipX_relShadowPos.x);
    }
}

void Character::setPositionY(float y)
{
    CCSprite::setPositionY(y);
    
    // set shadow's position y
    if(m_shadow)
    {
        if(!isInMidAir() && !isJumpAlongXY())
        {
            if(isFlipX())
                m_shadow->setPositionY(y + _preCached_flipX_relShadowPos.y);
            else
                m_shadow->setPositionY(y + _preCached_nonFlipX_relShadowPos.y);
        }
    }
}

void Character::setPosition(float x, float y)
{
    CCNode::setPosition(x,y);
    
    // set shadow's position x
    if(m_shadow)
    {
        if(isFlipX())
        {
            m_shadow->cocos2d::CCNode::setPositionX(x + _preCached_flipX_relShadowPos.x);
            if(!isInMidAir() && !isJumpAlongXY())
                m_shadow->cocos2d::CCNode::setPositionY(y + _preCached_flipX_relShadowPos.y);
        }
        else
        {
            m_shadow->cocos2d::CCNode::setPositionX(x + _preCached_nonFlipX_relShadowPos.x);
            if(!isInMidAir() && !isJumpAlongXY())
                m_shadow->cocos2d::CCNode::setPositionY(y + _preCached_nonFlipX_relShadowPos.y);
        }
    }
}

void Character::setPosition(const cocos2d::CCPoint& pos)
{
    CCSprite::setPosition(pos);
    
    // set shadow's position x
    if(m_shadow)
    {
        if(isFlipX())
        {
            CCPoint setPos = ccp(pos.x + _preCached_flipX_relShadowPos.x, pos.y + _preCached_flipX_relShadowPos.y);
            
            m_shadow->setPositionX(setPos.x);
            if(!isInMidAir() && !isJumpAlongXY())
                m_shadow->setPositionY(setPos.y);
        }
        else
        {
            CCPoint setPos = ccp(pos.x + _preCached_nonFlipX_relShadowPos.x, pos.y + _preCached_nonFlipX_relShadowPos.y);
            
            m_shadow->setPositionX(setPos.x);
            if(!isInMidAir() && !isJumpAlongXY())
                m_shadow->setPositionY(setPos.y);
        }
    }
}

void Character::removeFromParentAndCleanup(bool cleanup)
{
    // also remove out shadow
    if(m_shadow != NULL)
        m_shadow->removeFromParentAndCleanup(cleanup);
    
    // remove and clean up self
    CCSprite::removeFromParentAndCleanup(cleanup);
}

bool Character::isInMidAir()
{
    return m_isInMidAir;
}

void Character::setIsInMidAir(bool b)
{
    m_isInMidAir = b;
    
    
    // apply new y-position
    if(!m_isInMidAir)
    {
        if(isFlipX())
        {
            m_shadow->setPositionY(getPositionY() + _preCached_flipX_relShadowPos.y);
        }
        else
        {
            m_shadow->setPositionY(getPositionY() + _preCached_nonFlipX_relShadowPos.y);
        }
    }
}

bool Character::isJumpAlongXY()
{
    return m_isJumpAlongXY;
}

void Character::setIsJumpAlongXY(bool b)
{
    m_isJumpAlongXY = b;
}

CCSprite* Character::getShadow()
{
    return m_shadow;
}

void Character::addToManageDisplayedFrameCMDsAnimationFrame(CCAnimationFrame* animationFrame)
{
    m_managedDisplayedFrameCMDs->addObject(animationFrame);
}

bool Character::isRemainInStageWhenRecycled()
{
    return m_isRemainInStageWhenRecycled;
}

void Character::setIsRemainInStageWhenRecycled(bool b)
{
    if(m_isRemainInStageWhenRecycled != b)
    {
        m_isRemainInStageWhenRecycled = b;
    }
}

void Character::recycle()
{
    // stop all actions
    stopAllActions();
    stopScalingShadow();
    
    // remove from parent (if need)
    if(!m_isRemainInStageWhenRecycled)
        Character::removeFromParentAndCleanup(false);
    
    // set shadow's scale back to normal
    m_shadow->setScale(_shadow_savedSetScale);
    
    // derived class should overwirte or add more specific operations to this dump function (if need)
    // but always call this base function
}

void Character::dump()
{
    stopAllActions();
    stopScalingShadow();
    Character::removeFromParentAndCleanup(true);
    dealloc();
    
    // derived class should overwirte or add more specific operations to this dump function (if need)
    // but always call this base function
}

int Character::getRecycleTypeID()
{
    return getTag();
}

void Character::stopScalingShadow()
{
    // stop all actions
    m_shadow->stopAllActions();
}

void Character::startLoopScalingShadow()
{
    m_shadow->runAction(_cached_shadow_scaleLoop);
}

void Character::startToBiggerThenNormalScalingShadow()
{
    m_shadow->runAction(_cached_shadow_scaleToBiggerThenNormalOneTime);
}

void Character::startToSmallerThenNormalScalingShadow()
{
    m_shadow->runAction(_cached_shadow_scaleToSmallerThenNormalOneTime);
}

void Character::startToNormalScalingShadow()
{
    m_shadow->runAction(_cached_shadow_scaleToNormalOneTime);
}

void Character::startMidAirScalingShadow()
{
    m_shadow->runAction(_cached_shadow_scaleForMidAirThenNormalOneTime);
}

void Character::resumeSchedulerAndActions()
{
    CCSprite::resumeSchedulerAndActions();
    
    // resume shadow is not null, and shadow's action if added into a node
    if(m_shadow != NULL && m_shadow->getParent() != NULL)
        m_shadow->resumeSchedulerAndActions();
}

void Character::pauseSchedulerAndActions()
{
    CCSprite::pauseSchedulerAndActions();
    
    // resume shadow is not null, and shadow's action if added into a node
    if(m_shadow != NULL && m_shadow->getParent() != NULL)
        m_shadow->pauseSchedulerAndActions();
}

void Character::_cacheScalingShadowActions(float normalShadowScale)
{
    // to normal
    {
        // stop action first before doing a cache
        if(_cached_shadow_scaleToNormalOneTime != NULL)
            this->stopAction(_cached_shadow_scaleToNormalOneTime);
        
        CCFiniteTimeAction *toNormal = CCEaseBounceOut::create(CCScaleTo::create(0.15f, 1.0f * normalShadowScale));
        _cached_shadow_scaleToNormalOneTime = toNormal;
        _cached_shadow_scaleToNormalOneTime->retain();
    }
    
    // loop
    {
        // stop action first before doing a cache
        if(_cached_shadow_scaleLoop != NULL)
            this->stopAction(_cached_shadow_scaleLoop);
        
        // also taken into account the character scale factor
        CCFiniteTimeAction *toSmall = CCEaseBounceOut::create(CCScaleTo::create(0.25f, 0.7f * normalShadowScale));
        CCFiniteTimeAction *toNormal = CCEaseBounceOut::create(CCScaleTo::create(0.25f, 1.0f * normalShadowScale));
        CCSequence *seq = CCSequence::createWithTwoActions(toSmall, toNormal);
        _cached_shadow_scaleLoop = CCRepeatForever::create(seq);
        _cached_shadow_scaleLoop->retain();
    }
    
    // to small
    {
        // stop action first before doing a cache
        if(_cached_shadow_scaleToSmallerThenNormalOneTime != NULL)
            this->stopAction(_cached_shadow_scaleToSmallerThenNormalOneTime);
        
        CCFiniteTimeAction *toSmall = CCEaseBounceIn::create(CCScaleTo::create(0.2f, 0.7f * normalShadowScale));
        CCFiniteTimeAction *toNormal = CCEaseBounceOut::create(CCScaleTo::create(0.2f, 1.0f * normalShadowScale));
        _cached_shadow_scaleToSmallerThenNormalOneTime = CCSequence::createWithTwoActions(toSmall, toNormal);
        _cached_shadow_scaleToSmallerThenNormalOneTime->retain();
    }
    
    // to bigger
    {
        // stop action first before doing a cache
        if(_cached_shadow_scaleToBiggerThenNormalOneTime != NULL)
            this->stopAction(_cached_shadow_scaleToBiggerThenNormalOneTime);
        
        CCFiniteTimeAction *toBigger = CCEaseBounceOut::create(CCScaleTo::create(0.2f, 1.2f * normalShadowScale));
        CCFiniteTimeAction *toNormal = CCEaseBounceOut::create(CCScaleTo::create(0.2f, 1.0f * normalShadowScale));
        _cached_shadow_scaleToBiggerThenNormalOneTime = CCSequence::createWithTwoActions(toBigger, toNormal);
        _cached_shadow_scaleToBiggerThenNormalOneTime->retain();
    }
    
    // mid-air
    {
        // stop action first before doing a cache
        if(_cached_shadow_scaleForMidAirThenNormalOneTime != NULL)
            this->stopAction(_cached_shadow_scaleForMidAirThenNormalOneTime);
        
        CCFiniteTimeAction *toSmall = CCEaseBounceOut::create(CCScaleTo::create(0.4f, 0.6f * normalShadowScale));
        CCFiniteTimeAction *toNormal = CCEaseBounceOut::create(CCScaleTo::create(0.4f, 1.0f * normalShadowScale));
        _cached_shadow_scaleForMidAirThenNormalOneTime = CCSequence::createWithTwoActions(toSmall, toNormal);
        _cached_shadow_scaleForMidAirThenNormalOneTime->retain();
    }
}

NS_E_HARPENGINE