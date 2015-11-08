//
//  Character.h
//  ZombieHero
//
//  Created by haxpor on 10/22/12.
//
//

#ifndef __ZombieHero__Character__
#define __ZombieHero__Character__

#define ALIVE_ASSERT if(isDead())\
    return;
#define IS_IN_MIDAIR_ASSERT if(isInMidAir())\
    return;

#include "cocos2d.h"
#include "vec2.h"
#include "IRecyclableObject.h"

NS_S_HARPENGINE

/**
 * Base class for any character in game.
 */
class Character : public cocos2d::CCSprite, public harp::IRecyclableObject
{
public:
    Character();
    virtual ~Character();
    virtual void dealloc();
    
    virtual bool initWithSpriteFrameName(const char *spriteFrameName, const cocos2d::CCPoint &hdCharCenterPos, int hp, float maxSpeed, int incAttackPoint, int defPoint);
    void setFlipX(bool bFlipX); /* overwirtten the original method.*/
    void moveWithDirection(const kmVec2& vecDir, float dt);
    void moveWithDirection(const cocos2d::CCPoint& vecDir, float dt);
    void moveWithVelocity(const cocos2d::CCPoint& velocity, float dt);
    
    // -- overwrite CCSprite's removeFromParentAndCleanup()
    virtual void setPositionX(float x);
    virtual void setPositionY(float y);
    virtual void setPosition(float x, float y); // from CCNode
    virtual void setPosition(const cocos2d::CCPoint& pos);  // from CCSprite
    virtual void removeFromParentAndCleanup(bool cleanup);
    
    // -- Specific classes need to overwrite these. -- //
    virtual void onBeingHit(int decHP, cocos2d::CCSprite* attacker, void* data = NULL);
    virtual bool isAttacking();
    virtual bool isUnderAttackOrDown();
    virtual bool isUnderAttack();
    virtual bool isDown();
    virtual bool isWakeup();
    
    // -- recyclable object -- //
    /* If need to add specific instruction on recycling this object, then overwrite this function. */
    virtual void recycle();
    virtual void dump();
    virtual int getRecycleTypeID();
    
    bool isDead();
    bool isAlive();
    
    /**
     Max speed for this instance to move.
     Usually non-steering AI instance will use max value, but steering AI will gradually use from [0, getMaxSpeed()].
     But custom code can make non-steering AI instance uses in similar way as steering AI but we normally don't do
     that.
     */
    float getMaxSpeed();
    void setMaxSpeed(float maxSpeed);
    
    // HP
    int getHP();
    void setHP(int hp);
    
    // Inc Attack point.
    int getIncAttackPoint();
    void setIncAttackPoint(int attack);
    
    // Defense point
    int getDefensePoint();
    void setDefensePoint(int defPoint);
    
    // Facing direction.
    // Use these getter/setter if you know what you're doing. Normally its setter method will be
    // used internally.
    // You don't have to set it manually.
    Direction getFacingDirection();
    void setFacingDirection(Direction dir);
    
    // Collision size.
    const cocos2d::CCSize& getStaticCollisionSize();
    void setStaticCollisionSize(const cocos2d::CCSize& size);
    
    // Beware : This call is expensive. Try to cache the result and use it from there.
    // It doesn't return const but making change to return value doesn't affect anything
    cocos2d::CCSize getDynamicCollisionSize();
    
    // Character's offset position.
    const cocos2d::CCPoint& getCharacterOffset();
    void setCharacterOffset(const cocos2d::CCPoint& offset);
    //## End of Get/Set Methods. ##//
    
    /* 
        Add this character as a child to the specified node.
        Tag will be set automatically from the information known in the creation time of this character.
     */
    virtual void addAsChildTo(cocos2d::CCNode *node, cocos2d::CCSpriteBatchNode *shadowBatchNode, int zOrder);
    /* Reorder self among others objects from the current parent child. */
    void reorderSelf();
    /*
        Adjust position of shadow attached as a child of this character. If visual results in undesire, then call this functions to adjust accordingly.
     */
    void adjustInitialShadowPosition(const cocos2d::CCPoint& offset);
    
    /* 
        Set scale of shadow.
        Note: Must be based on 2.0f as it's half-size asset.
     */
    void adjustInitialShadowScale(float scale);
    
    /**
     Get bounding radius of this character.
     */
    float getBoundingRadius();
    
    virtual bool isInMidAir();
    void setIsInMidAir(bool b);
    bool isJumpAlongXY();
    void setIsJumpAlongXY(bool b);
    
    /* Get shadow sprite */
    cocos2d::CCSprite* getShadow();
    
    void addToManageDisplayedFrameCMDsAnimationFrame(cocos2d::CCAnimationFrame* animationFrame);
    
    /* Set this character to be remained in the stage after get recycled. If you set it to remain in the stage, then you have to manually remove it after that. Especially this will be used with boss enemy to let the story progress while having seen its dead body. But that's totally optional. */
    bool isRemainInStageWhenRecycled();
    void setIsRemainInStageWhenRecycled(bool b);
    
    /** Shadow's scaling action related functions */
    void stopScalingShadow();
    void startLoopScalingShadow();
    void startToBiggerThenNormalScalingShadow();
    void startToSmallerThenNormalScalingShadow();
    void startToNormalScalingShadow();
    void startMidAirScalingShadow();
    
    virtual void resumeSchedulerAndActions(void);
    virtual void pauseSchedulerAndActions(void);
    
private:
    // cache scaling action for shadow and based on specified normal shadow scale
    void _cacheScalingShadowActions(float normalShadowScale);
    
protected:
    int m_hp;
    int m_incAttackPoint;
    int m_defPoint;
    int m_offsetDivFactor;
    bool m_isInMidAir;
    bool m_isJumpAlongXY;
    bool m_isRemainInStageWhenRecycled;
    float m_maxSpeed;
    
    Direction m_facingDirection;
    cocos2d::CCPoint m_charOffset;
    cocos2d::CCSize m_staticCollisionSize;
    cocos2d::CCSprite *m_shadow;
    cocos2d::CCArray *m_managedDisplayedFrameCMDs;
    
private:
    cocos2d::CCPoint _preCached_nonFlipX_anchorPoint;
    cocos2d::CCPoint _preCached_flipX_anchorPoint;
    
    // relative position from character's position
    cocos2d::CCPoint _preCached_nonFlipX_relShadowPos;
    cocos2d::CCPoint _preCached_flipX_relShadowPos;
    
    // pre-cached of shadow's scaling action
    cocos2d::CCAction* _cached_shadow_scaleLoop;
    cocos2d::CCAction* _cached_shadow_scaleToSmallerThenNormalOneTime;
    cocos2d::CCAction* _cached_shadow_scaleToBiggerThenNormalOneTime;
    cocos2d::CCAction* _cached_shadow_scaleToNormalOneTime;
    cocos2d::CCAction* _cached_shadow_scaleForMidAirThenNormalOneTime;
    
    float _shadow_savedSetScale;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__Character__) */
