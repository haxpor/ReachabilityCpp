//
//  SteeringBehavior.cpp
//  ZombieHero
//
//  Created by haxpor on 3/6/14.
//
//

#include "SteeringBehavior.h"
#include "CharacterAI.h"
#include "MathUtil.h"
#include "HelperAIFunctions.h"

using namespace cocos2d;
using namespace harp;

SteeringBehavior::SteeringBehavior(CharacterAI* agent):
    m_flags(none),
    m_summingMethod(prioritized),
    m_deceleration(normal),
    m_wallDetectionFeelerLength(HarpConvertLength(60.0f)),
    m_feelers(3),
    m_wanderRadius(HarpConvertLength(60.0f)),
    m_wanderJitterPerSec(80.0f),
    m_wanderDistance(HarpConvertLength(50.0f))
{
    m_pVehicle = agent;
    
    // random and set initial wander target
    float theta = MathUtil::sharedInstance()->randomFloat() * M_PI * 2.0f;
    m_wanderTarget = ccp(m_wanderRadius * cosf(theta), m_wanderRadius * sinf(theta));
}

SteeringBehavior::~SteeringBehavior()
{
    
}

CCPoint SteeringBehavior::calculate()
{
    // reset the steering force
    m_steeringForce.setPoint(0.0f, 0.0f);
    
    // for group behaviors, tag entities first
    if(on(separation) || on(alignment) || on(cohesion))
    {
        // TODO: Add settable group distance here ...
        m_pVehicle->getGameWorld()->tagVehiclesWithinViewRange(m_pVehicle, HarpConvertLength(200.0f));
    }
    
    // calculate steering force based on the summing method
    if(m_summingMethod == weight_average)
        m_steeringForce = this->_calculateWeightedSum();
    else if(m_summingMethod == prioritized)
        m_steeringForce = this->_calculatePrioritized();
    
    return m_steeringForce;
}

/**
 _accumulateForce
 
 Calculates how much of its max steering force the vehicle has left to apply and then applies that amount of the force to add.
 */
bool SteeringBehavior::_accumulateForce(CCPoint &runningTotal, const CCPoint &forceToAdd)
{
    // calculate how much steering force the vehicle has used so far
    float magnitudeSoFar = ccpLength(runningTotal);
    
    // calculate how much steering force remains to be used by this vehicle
    float magnitudeRemaining = m_pVehicle->getMaxForce() - magnitudeSoFar;
    
    // return false if there is no more force left to use
    if(magnitudeRemaining <= 0.0f)
        return false;
    
    // calculate the magnitude of the force we want to add
    float magnitudeToAdd = ccpLength(forceToAdd);
    
    // if the magnitude of the sum of forceToAdd and the running total
    // does not exceed the maximum force available to this vehicle, just
    // add together. Otherwise, add as much of the forceToAdd vector is possible
    // without going over the max
    if(magnitudeToAdd < magnitudeRemaining)
    {
        runningTotal = ccpAdd(runningTotal, forceToAdd);
    }
    else
    {
        // add to the steering force
        runningTotal = ccpAdd(runningTotal, ccpMult(ccpNormalize(forceToAdd), magnitudeRemaining));
    }
    
    return true;
}

CCPoint SteeringBehavior::_calculateWeightedSum()
{
    // first priority
    if(on(wall_avoidance))
        m_steeringForce = ccpMult(ccpAdd(m_steeringForce, _wallAvoidance(m_pVehicle->getGameWorld()->getWalls())), 0.1f);
    
    // group behaviors
    if(on(separation))
        m_steeringForce = ccpMult(ccpAdd(m_steeringForce, _separation(m_pVehicle->getGameWorld()->getAgents())), 30.0f);
    if(on(alignment))
        m_steeringForce = ccpAdd(m_steeringForce, _alignment(m_pVehicle->getGameWorld()->getAgents()));
    if(on(cohesion))
        m_steeringForce = ccpAdd(m_steeringForce, _cohesion(m_pVehicle->getGameWorld()->getAgents()));
    
    if(on(wander))
        m_steeringForce = ccpMult(ccpAdd(m_steeringForce, _wander()), 0.4f);
    
    if(on(seek))
    {
        if(m_targetAgent1)
            m_steeringForce = ccpAdd(m_steeringForce, _seek(m_targetAgent1->getPosition()));
    }
    
    if(on(arrive))
    {
        if(m_targetAgent1)
            m_steeringForce = ccpAdd(m_steeringForce, _arrive(m_targetAgent1->getPosition(), m_deceleration));
    }
    
    if(on(flee))
    {
        if(m_targetAgent1)
            m_steeringForce = ccpAdd(m_steeringForce, _flee(m_targetAgent1->getPosition()));
    }
    
    if(on(pursuit))
    {
        CCAssert(m_targetAgent1 != NULL, "Target agent 1 must be set, and must not be NULL.");
        
        m_steeringForce = ccpAdd(m_steeringForce, _pursuit(m_targetAgent1));
    }
    
    // truncate
    m_steeringForce = ccpTruncate(m_steeringForce, m_pVehicle->getMaxForce());
    
    return m_steeringForce;
}

CCPoint SteeringBehavior::_calculatePrioritized()
{
    CCPoint force;
    
    if(on(wall_avoidance))
    {
        force = ccpMult(_wallAvoidance(m_pVehicle->getGameWorld()->getWalls()), 100.0f);
        
        if(!_accumulateForce(m_steeringForce, force))
            return m_steeringForce;
    }
    
    if(on(flee))
    {
        if(m_targetAgent1)
            force = ccpMult(_flee(m_targetAgent1->getPosition()), 1.0f);
        
        if(!_accumulateForce(m_steeringForce, force))
            return m_steeringForce;
    }
    
    // group behaviors
    if(on(separation))
    {
        force = ccpMult(_separation(m_pVehicle->getGameWorld()->getAgents()), 60.0f);
        
        if(!_accumulateForce(m_steeringForce, force))
            return m_steeringForce;
    }
    if(on(alignment))
    {
        force = ccpMult(_alignment(m_pVehicle->getGameWorld()->getAgents()), 1.0f);
        
        if(!_accumulateForce(m_steeringForce, force))
            return m_steeringForce;
    }
    if(on(cohesion))
    {
        force = ccpMult(_cohesion(m_pVehicle->getGameWorld()->getAgents()), 1.0f);
        
        if(!_accumulateForce(m_steeringForce, force))
            return m_steeringForce;
    }
    
    if(on(seek))
    {
        if(m_targetAgent1)
            force = _seek(m_targetAgent1->getPosition());
        
        if(!_accumulateForce(m_steeringForce, force))
            return m_steeringForce;
    }
    
    if(on(arrive))
    {
        if(m_targetAgent1)
            force = _arrive(m_targetAgent1->getPosition(), m_deceleration);
        
        if(!_accumulateForce(m_steeringForce, force))
            return m_steeringForce;
    }
    
    if(on(wander))
    {
        force = ccpMult(_wander(), 1.0f);
        
        if(!_accumulateForce(m_steeringForce, force))
            return m_steeringForce;
    }
    
    if(on(pursuit))
    {
        CCAssert(m_targetAgent1 != NULL, "Target agent 1 must be set, and must not be NULL.");
        
        force = _pursuit(m_targetAgent1);
        
        if(!_accumulateForce(m_steeringForce, force))
            return m_steeringForce;
    }
    
    return m_steeringForce;
}

/*
 ============
 
 _createFeelers
 
 Creates the antenna utilized by Wall Avoidance
 
 ============
 */
void SteeringBehavior::_createFeelers()
{
    // feeler pointing straight in front
    m_feelers[0] = ccpAdd(m_pVehicle->getPosition(), ccpMult(m_pVehicle->getHeading(), m_wallDetectionFeelerLength));
    
    // temp variables used for both feeler of the left and right
    kmVec2 temp;
    kmMat3 rotMatrix;
    
    // - feeler to the right
    // set kmVec2 with heading
    kmVec2Fill(&temp, m_pVehicle->getHeading().x, m_pVehicle->getHeading().y);
    // set rotMatrix
    kmMat3Rotation(&rotMatrix, M_PI_2 * 3.5f);
    // do vector transform
    kmVec2Transform(&temp, &temp, &rotMatrix);
    // create new CCPoint from temp, and set to m_feelers
    m_feelers[1] = ccpAdd(m_pVehicle->getPosition(), ccpMult(ccp(temp.x, temp.y), m_wallDetectionFeelerLength/2.0f));
    
    // - feeler to the left
    // set kmVec2 with heading
    kmVec2Fill(&temp, m_pVehicle->getHeading().x, m_pVehicle->getHeading().y);
    // set rotMatrix
    kmMat3Rotation(&rotMatrix, M_PI_2 * 0.5f);
    // do vector transform
    kmVec2Transform(&temp, &temp, &rotMatrix);
    // create new CCPoint from temp, and set to m_feelers
    m_feelers[2] = ccpAdd(m_pVehicle->getPosition(), ccpMult(ccp(temp.x, temp.y), m_wallDetectionFeelerLength/2.0f));
}

CCPoint SteeringBehavior::_seek(const CCPoint& targetPos)
{
    CCPoint desiredVelocity = ccpMult( ccpNormalize(ccpSub(targetPos, m_pVehicle->getPosition())), m_pVehicle->getMaxSpeed());
    
    return ccpSub(desiredVelocity, m_pVehicle->getVelocity());
}

CCPoint SteeringBehavior::_arrive(const CCPoint& targetPos, Deceleration deceleration)
{
    CCPoint toTarget = ccpSub(targetPos, m_pVehicle->getPosition());
    
    // calculate the distance to the target
    float dist = ccpLength(toTarget);
    
    if(dist > 0.0f)
    {
        // TODO: Make this value settable ...
        // increase this value to make it slower
        const double DecelerationTweaker = 0.15f;
        
        // calculate the speed required to reach the target given the desired deceleration
        float speed = dist / ((float)deceleration * DecelerationTweaker);
        
        // make sure the velocity does not exceed the max
        speed = fminf(speed, m_pVehicle->getMaxSpeed());
        
        CCPoint desiredVelocity = ccpMult(toTarget, speed / dist);
        
        return ccpSub(desiredVelocity, m_pVehicle->getVelocity());
    }
    
    return CCPointZero;
}

CCPoint SteeringBehavior::_flee(const CCPoint& targetPos)
{
    CCPoint desiredVelocity = ccpMult( ccpNormalize(ccpSub(m_pVehicle->getPosition(), targetPos)), m_pVehicle->getMaxSpeed());
    
    return ccpSub(desiredVelocity, m_pVehicle->getVelocity());
}

CCPoint SteeringBehavior::_wallAvoidance(const std::vector<Wall2D>& walls)
{
    // the feelers are contained in a std::vector, m_feelers
    this->_createFeelers();
    
    float distToThisIP = 0.0f;
    float distToClosestTP = FLT_MAX;
    
    // this will hold an index into the vector of walls
    int closestWall = -1;
    
    CCPoint steeringForce,
            point,          // used for storing temporaray info
            closestPoint;   // holds the closest intersection point
    
    // examine each feeler in turn
    for (unsigned int flr=0; flr<m_feelers.size(); flr++)
    {
        // run through each wall checking for any intersection points
        for(unsigned int w=0; w<walls.size(); w++)
        {
            if(ccpLineIntersect(m_pVehicle->getPosition(),
                             m_feelers[flr],
                             walls[w].from(),
                             walls[w].to(),
                             distToThisIP,
                             point))
            {
                // if this the closest found so far? then keep a record
                if(distToThisIP < distToClosestTP)
                {
                    distToClosestTP = distToThisIP;
                    closestWall = w;
                    closestPoint = point;
                }
            }
        }
        
        // if an intersection point has been detected, calculate a force that will direct the agent away
        if(closestWall >= 0)
        {
            // calculate by what distance the projected position of the agent will overshoot the wall
            CCPoint overShoot = ccpSub(m_feelers[flr], closestPoint);
            
            // create a force in the direction of the wall normal, with a magnitude of the overshoot
            steeringForce = ccpMult(walls[closestWall].normal(), ccpLength(overShoot));
        }
    }
    
    return steeringForce;
}

CCPoint SteeringBehavior::_separation(const CCArray* neighbors)
{
    // do a dirty cast
    CCArray* cneighbors = (CCArray*)neighbors;
    
    CCPoint steeringForce;
    
    for(unsigned int i=0; i<cneighbors->count(); i++)
    {
        // get the casted entity
        CharacterAI* entity = static_cast<CharacterAI*>(cneighbors->objectAtIndex(i));
        
        CCAssert(entity != NULL, "entity should be able to cast, and not NULL.");
        
        // make sure this agent is not included in calculation, and that the agent being examined is close enough
        // also make sure it doesn't include evade target
        if(entity != m_pVehicle && entity->isTagged())
        {
            CCPoint toAgent = ccpSub(m_pVehicle->getPosition(), entity->getPosition());
            
            steeringForce = ccpAdd(steeringForce, ccpMult(ccpNormalize(toAgent), 1/ccpLength(toAgent)));
        }
    }
    
    return steeringForce;
}

CCPoint SteeringBehavior::_alignment(const CCArray* neighbors)
{
    // record average heading of neighbors
    CCPoint averageHeading;
    
    // count number of vehicles in the neighborhood
    int neighborCount = 0;
    
    // do a dirty cast
    CCArray* cneighbors = (CCArray*)neighbors;
    
    for(unsigned int i=0; i<cneighbors->count(); i++)
    {
        // get the casted entity
        CharacterAI* entity = static_cast<CharacterAI*>(cneighbors->objectAtIndex(i));
        
        CCAssert(entity != NULL, "entity should be able to cast, and not NULL.");
        
        // make sure this agent is not included in calculation, and that agent being examined is close enough
        // also make sure it doesn't include evade target
        if(entity != m_pVehicle && entity->isTagged())
        {
            averageHeading = ccpAdd(averageHeading, entity->getHeading());
            
            neighborCount++;
        }
    }
    
    // if the neighborhood contained one or more vehicles, average their heading vectors
    if(neighborCount > 0)
    {
        averageHeading = ccpMult(averageHeading, 1.0f/neighborCount);
        averageHeading = ccpSub(averageHeading, m_pVehicle->getHeading());
    }
    
    return averageHeading;
}

CCPoint SteeringBehavior::_cohesion(const CCArray *neighbors)
{
    CCPoint centerOfMass;
    CCPoint steeringForce;
    
    int neighborCount = 0;
    
    // do a dirty cast
    CCArray* cneighbors = (CCArray*)neighbors;
    
    for(unsigned int i=0; i<cneighbors->count(); i++)
    {
        // get the casted entity
        CharacterAI* entity = static_cast<CharacterAI*>(cneighbors->objectAtIndex(i));
        
        CCAssert(entity != NULL, "entity should be able to cast, and not NULL.");
        
        if(entity != m_pVehicle && entity->isTagged())
        {
            centerOfMass = ccpAdd(centerOfMass, entity->getPosition());
            neighborCount++;
        }
    }
    
    if(neighborCount > 0)
    {
        centerOfMass = ccpMult(centerOfMass, 1.0f/neighborCount);
        steeringForce = _seek(centerOfMass);
        
        // the magnitude of cohesion is much larger than separation and alignment, so normalize it
        steeringForce = ccpNormalize(steeringForce);
        if(isnan(steeringForce.x) || isnan(steeringForce.y))
        {
            CCLOG("nan");
            return CCPointZero;
        }
        else
        {
            return ccpNormalize(steeringForce);
        }
    }
    else
    {
        return CCPointZero;
    }
}

CCPoint SteeringBehavior::_wander()
{
    // this behavior depends on the update rate, thus apply some jitter
    float jitterTimeSlice = m_wanderJitterPerSec * m_pVehicle->getElapsedTime();
    
    // first add a small random vector to the target's position
    m_wanderTarget = ccpAdd(m_wanderTarget, ccp(MathUtil::sharedInstance()->randomClamped() * jitterTimeSlice,
                            MathUtil::sharedInstance()->randomClamped() * jitterTimeSlice));
    
    // reproject this new vector back on to a unit circle
    m_wanderTarget = ccpNormalize(m_wanderTarget);
    
    // increase the length of the vector to the same as the radius of the wander circle
    m_wanderTarget = ccpMult(m_wanderTarget, m_wanderRadius);
    
    // move the target into a position wander dist in front of the agent
    CCPoint target = ccpAdd(m_wanderTarget, ccp(m_wanderDistance, 0));
    
    // project the target into world space
    CCPoint ret = pointToWorldSpace(target, m_pVehicle->getHeading(), m_pVehicle->getSide(), m_pVehicle->getPosition());
    
    // and steer towards it
    return ccpSub(ret, m_pVehicle->getPosition());;
}

CCPoint SteeringBehavior::_pursuit(const CharacterAI *agent)
{
    // dirty cast
    CharacterAI* evader = (CharacterAI*)agent;
    
    // if the evader is ahead and facing the agent then we can just seek for the evaders current position
    CCPoint toEvader = ccpSub(evader->getPosition(), m_pVehicle->getPosition());
    
    float relativeHeading = ccpDot(m_pVehicle->getHeading(), evader->getHeading());
    
    if( (ccpDot(toEvader, m_pVehicle->getHeading()) > 0) &&
        (relativeHeading < -0.95f))
        return _seek(evader->getPosition());
    
    // the lookahead time is proportional to the distance between the evader and the pursuer; and is inversely
    // proportional to the sum of the agent's velocities
    float lookAheadTime = ccpLength(toEvader) / (m_pVehicle->getMaxSpeed() + evader->getSpeed());
    
    // now seek to the predicted future position of the evader
    return _seek(ccpAdd(evader->getPosition(), ccpMult(evader->getVelocity(), lookAheadTime)));
}

const std::vector<CCPoint>& SteeringBehavior::getFeelers()
{
    return m_feelers;
}

SteeringBehavior::summing_method SteeringBehavior::getSummingMethod()
{
    return m_summingMethod;
}

void SteeringBehavior::setSummingMethod(SteeringBehavior::summing_method method)
{
    m_summingMethod = method;
}

const CCPoint& SteeringBehavior::getWanderTarget()
{
    return m_wanderTarget;
}
float SteeringBehavior::getWanderRadius()
{
    return m_wanderRadius;
}
float SteeringBehavior::getWanderDistance()
{
    return m_wanderDistance;
}
float SteeringBehavior::getWanderJitterPerSec()
{
    return m_wanderJitterPerSec;
}

const CharacterAI* SteeringBehavior::getTargetAgent1()
{
    return m_targetAgent1;
}
void SteeringBehavior::setTargetAgent1(CharacterAI* agent)
{
    m_targetAgent1 = agent;
}

// ------------------
// -- On-functions --
// ------------------
void SteeringBehavior::seekOn()
{
    m_flags |= seek;
}

void SteeringBehavior::arriveOn()
{
    m_flags |= arrive;
}

void SteeringBehavior::fleeOn()
{
    m_flags |= flee;
}

void SteeringBehavior::wallAvoidanceOn()
{
    m_flags |= wall_avoidance;
}

void SteeringBehavior::separationOn()
{
    m_flags |= separation;
}

void SteeringBehavior::alignmentOn()
{
    m_flags |= alignment;
}

void SteeringBehavior::cohesionOn()
{
    m_flags |= cohesion;
}

void SteeringBehavior::wanderOn()
{
    m_flags |= wander;
}

void SteeringBehavior::pursuitOn()
{
    m_flags |= pursuit;
}

// ------------------
// -- Off-function --
// ------------------
void SteeringBehavior::seekOff()
{
    if(on(seek))
        m_flags ^= seek;
}

void SteeringBehavior::arriveOff()
{
    if(on(arrive))
        m_flags ^= arrive;
}

void SteeringBehavior::fleeOff()
{
    if(on(flee))
        m_flags ^= flee;
}

void SteeringBehavior::wallAvoidanceOff()
{
    if(on(wall_avoidance))
        m_flags ^= wall_avoidance;
}

void SteeringBehavior::separationOff()
{
    if(on(separation))
        m_flags ^= separation;
}

void SteeringBehavior::alignmentOff()
{
    if(on(alignment))
        m_flags ^= alignment;
}

void SteeringBehavior::wanderOff()
{
    if(on(wander))
        m_flags ^= wander;
}

void SteeringBehavior::pursuitOff()
{
    if(on(pursuit))
        m_flags ^= pursuit;
}

// -----------------------------
// -- Is-On checker functions --
// -----------------------------
bool SteeringBehavior::isSeekOn()
{
    return on(seek);
}

bool SteeringBehavior::isArriveOn()
{
    return on(arrive);
}

bool SteeringBehavior::isFleeOn()
{
    return on(flee);
}

bool SteeringBehavior::isWallAvoidanceOn()
{
    return on(wall_avoidance);
}

bool SteeringBehavior::isSeparationOn()
{
    return on(separation);
}

bool SteeringBehavior::isAlignmentOn()
{
    return on(alignment);
}

bool SteeringBehavior::isWanderOn()
{
    return on(wander);
}

bool SteeringBehavior::isPursuitOn()
{
    return on(pursuit);
}