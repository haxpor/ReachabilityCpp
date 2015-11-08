//
//  BoidAIController.h
//  ZombieHero
//
//  Created by haxpor on 4/17/13.
//
//

#ifndef __ZombieHero__BoidAIController__
#define __ZombieHero__BoidAIController__

#include "Core.h"
#include "cocos2d.h"
#include "CharacterAI.h"
#include "CoreMacros.h"
#include "IHasVelocity.h"

NS_S_HARPENGINE

class BoidAIController
{
public:
    float SEPARATION_WEIGHT = 1.0f;
    float ALIGNMENT_WEIGHT = 0.6f;
    float COHESION_WEIGHT = 0.2f;
    float NEIGHBOURS_RADIUS = harp::Core::sharedCore()->convertPointX(100.0f);
    float DESIRED_SEPARATION = harp::Core::sharedCore()->convertPointX(90.0f);
    
public:
    BoidAIController(harp::CharacterAI* ptrCharacterAI, float maximumSpeed, float maximumForce);
    ~BoidAIController();
    
    void update(float dt);

    float getMaximumSpeed();
    void setMaximumSpeed(float maxSpeed);
    float getMaximumForce();
    void setMaximumForce(float maxForce);
    
    /* Bind with external neighbours. */
    void bindWithNeighbours(cocos2d::CCArray* ptrNeighbours);
    const cocos2d::CCArray* getNeighbours();
    
private:
    cocos2d::CCPoint _flock(cocos2d::CCArray *ptrNeighbours);
    cocos2d::CCPoint _separate(cocos2d::CCArray *ptrNeighbours);
    cocos2d::CCPoint _align(cocos2d::CCArray *ptrNeighbours);
    cocos2d::CCPoint _cohere(cocos2d::CCArray *ptrNeighbours);
    cocos2d::CCPoint _steerTo(const cocos2d::CCPoint& target);
    
    cocos2d::CCPoint _limitWith(const cocos2d::CCPoint& vec, float limit);
    
protected:
    cocos2d::CCArray *m_refNeighbours;

    harp::CharacterAI *m_refAttachedCharacterAI;
    harp::IHasVelocity *m_refPrecastedIHasVelocity;

    float m_maxSpeed;
    float m_maxForce;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__BoidAIController__) */
