//
//  CharacterAI.cpp
//  ZombieHero
//
//  Created by haxpor on 1/31/13.
//
//

#include "CharacterAI.h"

#if IS_DEBUG == 1 && IS_STEERINGAI_SHOW_FLASHTAG_DEBUG == 1
// must not be included in release version, harp should not have any knowledge about zh namespace
#include "ObjectSpawner.h"
#endif

#if IS_DEBUG == 1 && IS_STEERINGAI_SHOW_WANDER_DEBUG == 1
#include "HelperAIFunctions.h"
#endif

NS_S_HARPENGINE

using namespace cocos2d;
using namespace harp;

CharacterAI::CharacterAI():
    m_targetPos(CCPointZero),
    m_currentState(-1),
    m_isEnabledAI(true),
    m_gameWorld(NULL),
    m_isSteeringBehaviorEnabled(false),
    m_steering(NULL),
    m_velocity(CCPointZero),
    m_heading(ccp(0.0f, 1.0f)),
    m_side(ccpPerp(m_heading)),
    m_mass(0.0f),
    m_maxForce(0.0f),
    m_maxTurnRate(0.0f),
    m_tag(false)
{
    
}

CharacterAI::~CharacterAI()
{
    CCLOGINFO("Destroying CharacterAI");
    
    delete m_steering;
}

bool CharacterAI::initWithSpriteFrameName(const char *spriteFrameName, const CCPoint &hdCharCenterPos, int hp, float maxSpeed, int defPoint)
{
    // inc-attack and def-percent will be zero as always
    if(Character::initWithSpriteFrameName(spriteFrameName, hdCharCenterPos, hp, maxSpeed, 0, defPoint))
    {
        // if init via this function, then disable steering behavior AI
        m_isSteeringBehaviorEnabled = false;
        
        // - mover (update at 60 Hz)
        this->scheduleUpdate();
        
        return true;
    }
    return false;
}

bool CharacterAI::initWithAttributes(const char *spriteFrameName, const cocos2d::CCPoint &hdCharCenterPos, int hp,
                                     const IGameWorldAIProtocol* gameWorld,
                                     const cocos2d::CCPoint& velocity,
                                     float maxSpeed,
                                     int defPoint,
                                     const cocos2d::CCPoint& heading,
                                     float mass,
                                     float maxTurnRate,
                                     float maxForce)
{
    // do normal init first
    if(CharacterAI::initWithSpriteFrameName(spriteFrameName, hdCharCenterPos, hp, maxSpeed, defPoint))
    {
        CCAssert(gameWorld != NULL, "gameWorld cannot be NULL.");
        
        // set attributes
        m_gameWorld = (IGameWorldAIProtocol*)gameWorld;
        m_velocity = velocity;
        m_heading = heading;
        m_mass = mass;
        m_maxTurnRate = maxTurnRate;
        m_maxForce = maxForce;
        
        // if init via this function, then default to enable steering behavior AI
        m_isSteeringBehaviorEnabled = true;
        
        // create steering behavior attached to this instance
        m_steering = new SteeringBehavior(this);
        
        return true;
    }
    return false;
}

void CharacterAI::update(float dt)
{
    // only move when enabled
    if(this->getIsEnabledAI() && this->_isMoveEnabled)
    {
        // if for steering AI, use its maintaining velocity
        if(this->getIsSteeringBehaviorEnabled())
        {
            this->moveWithVelocity(this->m_velocity, dt);
        }
        // if for normal AI, use its given moving dir
        else
        {
            this->moveWithDirection(this->_moveDir, dt);
        }
    }
}

void CharacterAI::updateAI(float dt)
{
    // only update when AI is enabled
    if(m_isEnabledAI)
    {
        // set the elapsed time
        m_elapsedTime = dt;
        
        // only update steering behavior if it's enabled, otherwise update normal FSM AI
        if(m_isSteeringBehaviorEnabled)
        {
            // update steering ai
            updateSteeringAI(dt);
            
            // to stop translation when in these states
            if(this->isUnderAttackOrDown() ||
               this->isDead() ||
               this->isInMidAir() ||
               this->isWakeup() ||
               this->isAttacking())
            {
                this->setMaxSpeed(0.0f);
            }
            
#if IS_DEBUG == 1 && IS_STEERINGAI_SHOW_FLASHTAG_DEBUG == 1
            // flash tag
            /*if(this->isTagged())
                this->setColor(ccc3(100, 100, 100));
            else
                this->setColor(ccc3(255, 255, 255));*/
            
            // just show temporary
            if(this->isTagged())
                ObjectSpawner::sharedInstance()->spawnDamageIndicator(DamageIndicatorType::WHITE, 1, this->getPosition(), TOPMOST_LAYER);
#endif
            
            CCPoint steeringForce;
            
            // calculate combined force from each steering behavior in the vehicle's list
            steeringForce = m_steering->calculate();
            
            // acceleration = Force / Mass
            CCPoint acceleration = ccpMult(steeringForce, 1/m_mass);
            
            // update velocity
            m_velocity = ccpAdd(m_velocity, ccpMult(acceleration, dt));
            
            // make sure vehicle does not exceed maximum velocity
            m_velocity = ccpTruncate(m_velocity, m_maxSpeed);
            
            // update the position
            //setPosition(ccpAdd(getPosition(), ccpMult(m_velocity, dt)));
            // begin to update position, this will update its position in update() loop
            this->beginMoveWithVelocity(m_velocity);
            
            // update the heading if vehicle has a non zero velocity
            if(ccpLengthSQ(m_velocity) > 0.00000001f)
            {
                m_heading = ccpNormalize(m_velocity);
                m_side = ccpPerp(m_heading);
            }
            
            // flip accordingly
            if(this->isReadyToTurnFacingDirection())
            {
                if(m_heading.x < 0.0f)
                    this->setFlipX(true);
                else
                    this->setFlipX(false);
            }
        }
        else
        {
            // update non-steering AI
            updateNonSteeringAI(dt);
        }
    }
}

void CharacterAI::updateSteeringAI(float dt)
{
    // do nothing here, derived class should override this to provide detail on steering AI update
}

void CharacterAI::updateNonSteeringAI(float dt)
{
    // do nothing here, derived class should override this to provide detail on non-steering AI update
}

const CCPoint& CharacterAI::getTargetPos()
{
    return m_targetPos;
}

void CharacterAI::setTargetPos(const CCPoint& targetPos)
{
    if(targetPos.x != m_targetPos.x &&
       targetPos.y != m_targetPos.y)
    {
        m_targetPos.setPoint(targetPos.x, targetPos.y);
    }
}

void CharacterAI::setState(int state)
{
    onStateChanged(state, m_currentState);
    
    // update states
    m_currentState = state;
}

int CharacterAI::getCurrentState() const
{
    return m_currentState;
}

bool CharacterAI::getIsSteeringBehaviorEnabled()
{
    return m_isSteeringBehaviorEnabled;
}

void CharacterAI::setSteeringBehaviorEnabled(bool b)
{
    m_isSteeringBehaviorEnabled = b;
}

SteeringBehavior* CharacterAI::getSteeringBehavior()
{
    return m_steering;
}

void CharacterAI::setEnabledAI(bool b)
{
    m_isEnabledAI = b;
}

bool CharacterAI::getIsEnabledAI()
{
    return m_isEnabledAI;
}

void CharacterAI::onStateChanged(int setState, int oldState)
{
    // do nothing here
    // specific class overwrites this
}

// -- moving entity's attributes related functions -- //
const CCPoint& CharacterAI::getVelocity()
{
    return m_velocity;
}
void CharacterAI::setVelocity(const CCPoint &pos)
{
    m_velocity = pos;
}

const CCPoint& CharacterAI::getHeading()
{
    return m_heading;
}

/**
 * setHeading
 *
 * First check if given heading is not a vector of zero length, if not then set heading accordingly.
 *
 * Return true if it is facing the desired direction.
 *
 */
void CharacterAI::setHeading(const CCPoint& heading)
{
    CCAssert( (ccpLengthSQ(heading) - 1.0) < 0.00001, "heading is a vector of zero length");
    
    m_heading = heading;
    
    // the side vector must always be perpendicular to the heading
    m_side = ccpPerp(m_heading);
}

/*
 ============
 
 rotateHeadingToFacePosition
 
 Rotates heading and side vectors by an amount not greater than m_maxTurnRate until it directly faces the target.
 
 Return true when heading is facing in desired direction
 
 ============
 */
bool CharacterAI::rotateHeadingToFacePosition(const CCPoint &target)
{
    CCPoint toTarget = ccpNormalize(ccpSub(target, getPosition()));
    
    double dot = ccpDot(m_heading, toTarget);
    clampf(dot, -1, 1);
    
    // determine the angle between the heading vector and the target
    double angle = acos(dot);
    
    // return true if it is facing the target
    if(angle < 0.00001)
        return true;
    
    // the amount to turn to the max turn rate
    if(angle > m_maxTurnRate)
        angle = m_maxTurnRate;
    
    // rotate the heading
    kmMat3 rotMatrix;
    
    // : we need to use kmVec2 as a buffer to calculate transformation
    kmVec2 heading;
    kmVec2 velocity;
    
    kmMat3Rotation(&rotMatrix, angle * ccpSign(m_heading, toTarget));
    kmVec2Transform(&heading, kmVec2Fill(&heading, m_heading.x, m_heading.y), &rotMatrix);
    kmVec2Transform(&velocity, kmVec2Fill(&velocity, m_velocity.x, m_velocity.y), &rotMatrix);
    
    m_heading.setPoint(heading.x, heading.y);
    m_velocity.setPoint(velocity.x, velocity.y);
    
    // finally recreate m_side
    m_side = ccpPerp(m_heading);
    
    return false;
}

const CCPoint& CharacterAI::getSide()
{
    return m_side;
}

float CharacterAI::getMass()
{
    return m_mass;
}

float CharacterAI::getMaxForce()
{
    return m_maxForce;
}
void CharacterAI::setMaxForce(float maxForce)
{
    m_maxForce = maxForce;
}

bool CharacterAI::getIsSpeedMaxedOut()
{
    return m_maxSpeed * m_maxSpeed >= ccpLengthSQ(m_velocity);
}
float CharacterAI::getSpeed()
{
    return ccpLength(m_velocity);
}
float CharacterAI::getSpeedSq()
{
    return ccpLengthSQ(m_velocity);
}

float CharacterAI::getMaxTurnRate()
{
    return m_maxTurnRate;
}
void CharacterAI::setMaxTurnRate(float maxTurnRate)
{
    m_maxTurnRate = maxTurnRate;
}

#if IS_DEBUG == 1 && (IS_STEERINGAI_SHOW_GISMO_DEBUG == 1 || IS_STEERINGAI_SHOW_WANDER_DEBUG == 1)
void CharacterAI::visit()
{
    Character::visit();
    
#if IS_DEBUG == 1 && IS_STEERINGAI_SHOW_GIZMO_DEBUG == 1
    // debug section for steering ai behavior
    CCPoint rectSize = ccp(HarpConvertSize(6),HarpConvertSize(6));
    
    // draw heading (red)
    ccDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);
    CCPoint headingHeadPos = ccpAdd(getPosition(), ccpMult(m_heading, HarpConvertLength(140)));
    ccDrawLine(getPosition(), headingHeadPos);
    // draw circle at the head pos of heading vector
    ccDrawSolidRect(ccpSub(headingHeadPos, rectSize), ccpAdd(headingHeadPos, rectSize), ccc4f(1.0f, 0.0f, 0.0f, 1.0f));
    
    // draw side (yellow)
    ccDrawColor4F(1.0f, 1.0f, 0.0f, 1.0f);
    CCPoint sideHeadPos = ccpAdd(getPosition(), ccpMult(m_side, HarpConvertLength(140)));
    ccDrawLine(getPosition(), sideHeadPos);
    // draw circle at the head pos of side vector
    ccDrawSolidRect(ccpSub(sideHeadPos, rectSize), ccpAdd(sideHeadPos, rectSize), ccc4f(1.0f, 1.0f, 0.0f, 1.0f));
#endif
    
#if IS_DEBUG == 1 && IS_STEERINGAI_SHOW_WANDER_DEBUG == 1
    if(m_steering != NULL && m_steering->isWallAvoidanceOn())
    {
        // draw the wander circle
        ccDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);
        CCPoint pos = pointToWorldSpace(ccp(m_steering->getWanderDistance(), 0), this->getHeading(), this->getSide(), this->getPosition());
        ccDrawCircle(pos, m_steering->getWanderRadius(), 0.0f, 20, false, 1.0f, 1.0f);
        
        // draw target circle
        ccDrawColor4F(1.0f, 1.0f, 0.0f, 1.0f);
        pos = pointToWorldSpace(ccpAdd(m_steering->getWanderTarget(), ccp(m_steering->getWanderDistance(), 0)), this->getHeading(), this->getSide(), this->getPosition());
        ccDrawCircle(pos, HarpConvertLength(5), 0.0f, 10, false, 1.0f, 1.0f);
    }
#endif
}
#endif

IGameWorldAIProtocol* const CharacterAI::getGameWorld() const
{
    return m_gameWorld;
}

bool CharacterAI::isReadyToTurnFacingDirection()
{
    return true;
}

bool CharacterAI::isTagged() const
{
    return m_tag;
}

void CharacterAI::tag()
{
    m_tag = true;
}

void CharacterAI::unTag()
{
    m_tag = false;
}

float CharacterAI::getElapsedTime() const
{
    return m_elapsedTime;
}

void CharacterAI::beginMoveWithDirection(const CCPoint &dir)
{
    // enable moving flag
    this->_isMoveEnabled = true;
    // set moving direction
    this->_moveDir = dir;
}

void CharacterAI::beginMoveWithVelocity(const CCPoint &vel)
{
    // enable moving flag
    this->_isMoveEnabled = true;
    // (safely) set moving dir to zero as we have velocity from steering AI already
    this->_moveDir = CCPointZero;
}

void CharacterAI::stopMoving()
{
    // disable moving flag
    this->_isMoveEnabled = false;
    // set moving direction to zero
    this->_moveDir = CCPointZero;
}

const CCPoint& CharacterAI::getNonSteeringMovingDirection()
{
    return _moveDir;
}

NS_E_HARPENGINE
