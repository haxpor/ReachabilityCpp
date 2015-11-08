//
//  SteeringBehavior.h
//  ZombieHero
//
//  Created by haxpor on 3/6/14.
//
//

#ifndef __ZombieHero__SteeringBehavior__
#define __ZombieHero__SteeringBehavior__

#include "cocos2d.h"
#include "CoreMacros.h"
#include "Wall2D.h"

NS_S_HARPENGINE

class CharacterAI;

class SteeringBehavior
{
public:
    enum summing_method
    {
        weight_average,
        prioritized
    };
    
    enum behavior_type
    {
        none                = 0x00000,
        seek                = 0x00002,
        arrive              = 0x00008,
        flee                = 0x00010,
        wall_avoidance      = 0x00020,
        separation          = 0x00040,
        alignment           = 0x00080,
        cohesion            = 0x00100,
        wander              = 0x00200,
        pursuit             = 0x00400
    };

public:
    SteeringBehavior(CharacterAI* agent);
    virtual ~SteeringBehavior();
    
    // calulates and sums the steering forces from any active behaviors
    cocos2d::CCPoint calculate();
    
    // test whether the give bit of m_flags is set
    bool on(behavior_type bt) {
        return (m_flags & bt) == bt;
    }
    
    // get the feelers for wall avoidance
    const std::vector<cocos2d::CCPoint>& getFeelers();
    
    // get/set summing method
    summing_method getSummingMethod();
    void setSummingMethod(summing_method method);
    
    // getter/setter for attributes that can be set
    const cocos2d::CCPoint& getWanderTarget();
    float getWanderRadius();
    float getWanderDistance();
    float getWanderJitterPerSec();
    
    const harp::CharacterAI* getTargetAgent1();
    void setTargetAgent1(harp::CharacterAI* agent);
    
    // related to m_flags | on/off and check
    void seekOn();
    void arriveOn();
    void fleeOn();
    void wallAvoidanceOn();
    void separationOn();
    void alignmentOn();
    void cohesionOn();
    void wanderOn();
    void pursuitOn();
    
    void seekOff();
    void arriveOff();
    void fleeOff();
    void wallAvoidanceOff();
    void separationOff();
    void alignmentOff();
    void cohesionOff();
    void wanderOff();
    void pursuitOff();
    
    bool isSeekOn();
    bool isArriveOn();
    bool isFleeOn();
    bool isWallAvoidanceOn();
    bool isSeparationOn();
    bool isAlignmentOn();
    bool isCohesionOn();
    bool isWanderOn();
    bool isPursuitOn();
    
private:
    // deceleration determines how quick it should decelerate to its target
    enum Deceleration { slow = 3, normal = 2, fast = 1};
    
private:
    bool _accumulateForce(cocos2d::CCPoint& runningTotal, const cocos2d::CCPoint& forceToAdd);
    
    cocos2d::CCPoint _calculateWeightedSum();
    cocos2d::CCPoint _calculatePrioritized();
    
    // create the antenna utilized by the wall avoidance behavior
    void _createFeelers();
    
    // -- behavior declarations -- //
    cocos2d::CCPoint _seek(const cocos2d::CCPoint& targetPos);
    cocos2d::CCPoint _arrive(const cocos2d::CCPoint& targetPos, Deceleration deceleration);
    cocos2d::CCPoint _flee(const cocos2d::CCPoint& targetPos);
    cocos2d::CCPoint _wallAvoidance(const std::vector<Wall2D>& walls);
    
    cocos2d::CCPoint _separation(const cocos2d::CCArray* neighbors);
    cocos2d::CCPoint _alignment(const cocos2d::CCArray* neighbors);
    cocos2d::CCPoint _cohesion(const cocos2d::CCArray* neighbors);
    
    cocos2d::CCPoint _wander();
    cocos2d::CCPoint _pursuit(const harp::CharacterAI* agent);
    // -- end of behavior declarations -- //
    
protected:
    // a pointer to the owner of this instance
    CharacterAI* m_pVehicle;
    
    // steering force created by the combined effect of all the selected behaviors
    cocos2d::CCPoint m_steeringForce;
               
    // flags indicating behaviors currently on/off
    int m_flags;
                             
    Deceleration m_deceleration;
    summing_method m_summingMethod;
    
    // a vertex buffer to contain the feelers for wall avoidance
    std::vector<cocos2d::CCPoint> m_feelers;
    // length of feeler used in wall avoidance
    float m_wallDetectionFeelerLength;
    
    // wander attributes
    cocos2d::CCPoint m_wanderTarget;
    float m_wanderRadius;
    float m_wanderJitterPerSec;
    float m_wanderDistance;
    
    // pursuit
    harp::CharacterAI* m_targetAgent1;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__SteeringBehavior__) */
