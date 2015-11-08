//
//  BoidAIController.cpp
//  ZombieHero
//
//  Created by haxpor on 4/17/13.
//
//

#include "BoidAIController.h"
#include "MathUtil.h"

using namespace cocos2d;
using namespace harp;

BoidAIController::BoidAIController(CharacterAI* ptrCharacterAI, float maximumSpeed, float maximumForce)
{
    CCAssert(ptrCharacterAI != NULL, "Input Character must not be NULL.");
    m_refAttachedCharacterAI = ptrCharacterAI;
    m_refPrecastedIHasVelocity = dynamic_cast<IHasVelocity*>(ptrCharacterAI);
    CCAssert(m_refPrecastedIHasVelocity != NULL, "Pre-casted must not equal NULL");
    
    m_maxSpeed = maximumSpeed;
    m_maxForce = maximumForce;
    
    // random velocity
    m_refPrecastedIHasVelocity->setVelocity(ccp(MathUtil::sharedInstance()->randomFloat() * 2.0f - 1.0f, MathUtil::sharedInstance()->randomFloat() * 2.0f - 1.0f));
}

BoidAIController::~BoidAIController()
{
    // null out all reference variables
    m_refNeighbours->release();
    m_refNeighbours = NULL;
    
    m_refAttachedCharacterAI = NULL;
    m_refPrecastedIHasVelocity = NULL;
}

void BoidAIController::update(float dt)
{
    // only when number of instances is greater than 1
    if(m_refNeighbours->count() > 1)
    {
        CCPoint acceleration = _flock(m_refNeighbours);
        
        CCPoint velocity = m_refPrecastedIHasVelocity->getVelocity();
        velocity = ccpAdd(velocity, acceleration);
        // limit with maximum speed
        velocity = _limitWith(velocity, m_maxSpeed);
        
        // set velocity to pre-casted object
        m_refPrecastedIHasVelocity->setVelocity(velocity);
        
        // move
        m_refAttachedCharacterAI->moveWithVelocity(velocity, dt);
    }
}

float BoidAIController::getMaximumSpeed()
{
    return m_maxSpeed;
}
void BoidAIController::setMaximumSpeed(float maxSpeed)
{
    if(m_maxSpeed != maxSpeed)
        m_maxSpeed = maxSpeed;
}

float BoidAIController::getMaximumForce()
{
    return m_maxForce;
}
void BoidAIController::setMaximumForce(float maxForce)
{
    if(m_maxForce != maxForce)
        m_maxForce = maxForce;
}

void BoidAIController::bindWithNeighbours(CCArray* ptrNeighbours)
{
    // just change the pointer
    m_refNeighbours = ptrNeighbours;
    m_refNeighbours->retain();
}
const CCArray* BoidAIController::getNeighbours()
{
    return m_refNeighbours;
}

CCPoint BoidAIController::_flock(CCArray *ptrNeighbours)
{
    CCPoint separation = ccpMult(_separate(ptrNeighbours), SEPARATION_WEIGHT);
    CCPoint alignment = ccpMult(_align(ptrNeighbours), ALIGNMENT_WEIGHT);
    CCPoint cohesion = ccpMult(_cohere(ptrNeighbours), COHESION_WEIGHT);
    
    return ccpAdd(ccpAdd(separation, alignment), cohesion);
}

CCPoint BoidAIController::_separate(CCArray *ptrNeighbours)
{
    CCPoint mean;
    int count = 0;
    
    for(int i=0; i<ptrNeighbours->count(); i++)
    {
        CharacterAI *boid = static_cast<CharacterAI*>(ptrNeighbours->objectAtIndex(i));
        CCAssert(boid != NULL, "boid must be able to cast to harp::CharacterAI");
        
        // if boid is still alive in the current layer (majorly not get recycled yet)
        if(boid->getParent() != NULL)
        {
            float distance = ccpDistance(m_refAttachedCharacterAI->getPosition(), boid->getPosition());
            if(distance > 0 && distance < DESIRED_SEPARATION)
            {
                mean = ccpAdd( mean, ccpMult( ccpNormalize( ccpSub(m_refAttachedCharacterAI->getPosition(), boid->getPosition()) ), 1.0f/distance ) );
                count++;
            }
        }
    }
    
    if(count > 0)
        mean = ccpMult(mean, 1.0f / count);
    return mean;
}

CCPoint BoidAIController::_align(CCArray *ptrNeighbours)
{
    CCPoint mean;
    int count = 0;
    
    for(int i=0; i<ptrNeighbours->count(); i++)
    {
        CharacterAI *boid = static_cast<CharacterAI*>(ptrNeighbours->objectAtIndex(i));
        CCAssert(boid != NULL, "boid must be able to cast to harp::CharacterAI");
        IHasVelocity *boidVel = dynamic_cast<IHasVelocity*>(boid);
        CCAssert(boidVel != NULL, "boid must be able to cast to IHasVelocity.");
        
        // if boid is still alive in the current layer (majorly not get recycled yet)
        if(boid->getParent() != NULL)
        {
            float distance = ccpDistance(m_refAttachedCharacterAI->getPosition(), boid->getPosition());
            if(distance > 0 && distance < NEIGHBOURS_RADIUS)
            {
                mean = ccpAdd(mean, boidVel->getVelocity());
                count++;
            }
        }
    }
    
    if(count > 0)
        mean = ccpMult(mean, 1.0f / count);
    _limitWith(mean, m_maxForce);
    
    return mean;
}

CCPoint BoidAIController::_cohere(cocos2d::CCArray *ptrNeighbours)
{
    CCPoint sum;
    int count = 0;
    
    for(int i=0; i<ptrNeighbours->count(); i++)
    {
        CharacterAI *boid = static_cast<CharacterAI*>(ptrNeighbours->objectAtIndex(i));
        CCAssert(boid != NULL, "boid must be able to cast to harp::CharacterAI");
        
        // if boid is still alive in the current layer (majorly not get recycled yet)
        if(boid->getParent() != NULL)
        {
            float distance = ccpDistance(m_refAttachedCharacterAI->getPosition(), boid->getPosition());
            if(distance > 0 && distance < NEIGHBOURS_RADIUS)
            {
                sum = ccpAdd(m_refAttachedCharacterAI->getPosition(), boid->getPosition());
                count++;
            }
        }
    }
    
    if(count > 0)
        return _steerTo(ccpMult(sum, 1.0f/count));
    else
        return sum;
}

CCPoint BoidAIController::_steerTo(const CCPoint& target)
{
    CCPoint desired = ccpSub(target, m_refAttachedCharacterAI->getPosition());
    float distance = ccpLength(desired);
    
    if(distance > 0)
    {
        desired = ccpNormalize(desired);
        
        float convertedLimitLength = Core::sharedCore()->convertPointX(50.0f);
        if(distance < convertedLimitLength)
            desired = ccpMult(desired, m_maxSpeed * (distance / convertedLimitLength));
        else
            desired = ccpMult(desired, m_maxSpeed);
        
        // steer = desired minus veclocity
        return _limitWith(ccpSub(desired, m_refPrecastedIHasVelocity->getVelocity()), m_maxForce);
    }
    else
        return CCPointZero;
}

CCPoint BoidAIController::_limitWith(const CCPoint &vec, float limit)
{
    // copy from input vec
    CCPoint ret(vec);
    
    if(ccpLength(vec) > limit)
    {
        ret = ccpNormalize(vec);
        ret = ccpMult(ret, limit);
        
        return ret;
    }
    else
        return ret;
}


