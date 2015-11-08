//
//  CharacterAI.h
//  ZombieHero
//
//  Created by haxpor on 1/31/13.
//
//

#ifndef __ZombieHero__CharacterAI__
#define __ZombieHero__CharacterAI__

#include "Core.h"
#include "Character.h"
#include "SteeringBehavior.h"
#include "IGameWorldAIProtocol.h"

NS_S_HARPENGINE

/**
 *  Base class for character that is controlled by AI.
 */
class CharacterAI : public harp::Character
{
public:
    CharacterAI();
    virtual ~CharacterAI();
    
    virtual bool initWithSpriteFrameName(const char *spriteFrameName, const cocos2d::CCPoint &hdCharCenterPos, int hp, float maxSpeed, int defPoint);
    virtual bool initWithAttributes(const char *spriteFrameName, const cocos2d::CCPoint &hdCharCenterPos, int hp,
                                    const harp::IGameWorldAIProtocol* gameWorld,
                                    const cocos2d::CCPoint& velocity,
                                    float maxSpeed,
                                    int defPoint,
                                    const cocos2d::CCPoint& heading,
                                    float mass,
                                    float maxTurnRate,
                                    float maxForce);
    
    virtual void update(float dt);
    virtual void updateAI(float dt);
    virtual void updateNonSteeringAI(float dt);
    virtual void updateSteeringAI(float dt);

#if IS_DEBUG == 1 && (IS_STEERINGAI_SHOW_GISMO_DEBUG == 1 || IS_STEERINGAI_SHOW_WANDER_DEBUG == 1)
    /**
     overwrite visit() to draw visual debug line on top, if enabled.
     Otherwise not overwrite.
     */
    virtual void visit();
#endif
    
    /**
     If need to add specific instruction up on state being changed, then overwrite this function.
     
     Derived class must call its super function at the beginning.
     */
    virtual void onStateChanged(int setState, int oldState);
    
    /* Use this information to properly turn facing direction when time is right. */
    virtual bool isReadyToTurnFacingDirection();
    
    /** Get/Set target position. **/
    const cocos2d::CCPoint& getTargetPos();
    void setTargetPos(const cocos2d::CCPoint& targetPos);
    
    void setState(int state);
    int getCurrentState() const;
    
    bool getIsSteeringBehaviorEnabled();
    void setSteeringBehaviorEnabled(bool b);
    harp::SteeringBehavior* getSteeringBehavior();
    
    /* Control whether AI code will be executed. */
    void setEnabledAI(bool b);
    bool getIsEnabledAI();
    
    // -- moving entity's attributes -- //
    harp::IGameWorldAIProtocol* const getGameWorld() const;
    
    const cocos2d::CCPoint& getVelocity();
    void setVelocity(const cocos2d::CCPoint& pos);
    
    const cocos2d::CCPoint& getHeading();
    void setHeading(const cocos2d::CCPoint& heading);
    bool rotateHeadingToFacePosition(const cocos2d::CCPoint& target);
    
    const cocos2d::CCPoint& getSide();
    
    float getMass();
    
    float getMaxForce();
    void setMaxForce(float maxForce);
    
    bool getIsSpeedMaxedOut();
    float getSpeed();
    float getSpeedSq();
    
    float getMaxTurnRate();
    void setMaxTurnRate(float maxTurnRate);
    
    bool isTagged() const;
    void tag();
    void unTag();
    
    float getElapsedTime() const;
    // -- end of moving entity's attributes -- //
    
    /**
     Begin to move with the given direction.
     This will enable moving of this object, then it will smoothly moves each frame.
     */
    void beginMoveWithDirection(const cocos2d::CCPoint& dir);
    
    /**
     Begin to move with the given velocity.
     This will enable moving of this object, then it will smoothly moves each frame.
     */
    void beginMoveWithVelocity(const cocos2d::CCPoint& vel);
    
    /**
     Stop moving.
     This will disable moving of this object, its mover won't update the position anymore until
     it's enabled again.
     */
    void stopMoving();
    
    /**
     Get non-steering moving direction at the moment.
     */
    const cocos2d::CCPoint& getNonSteeringMovingDirection();

protected:
    cocos2d::CCPoint m_targetPos;
    int m_currentState;
    bool m_isEnabledAI;
    
    // -- moving entity's attributes -- //
    harp::IGameWorldAIProtocol* m_gameWorld;
    
    bool m_isSteeringBehaviorEnabled;
    harp::SteeringBehavior* m_steering;
    
    cocos2d::CCPoint m_velocity;
    cocos2d::CCPoint m_heading;
    cocos2d::CCPoint m_side;
    float m_mass;
    float m_maxForce;
    float m_maxTurnRate;
    
    // generic flag
    bool m_tag;
    
    // elpased time
    // note: will be set each frame
    float m_elapsedTime;
    
private:
    /**
     Move information. These will be used in mover update when this instance decides to move.
     */
    bool _isMoveEnabled;
    cocos2d::CCPoint _moveDir;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__CharacterAI__) */
