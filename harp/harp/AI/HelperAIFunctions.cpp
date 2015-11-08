#include "Character.h"

//
//  HelperAIFunctions.cpp
//  ZombieHero
//
//  Created by haxpor on 3/19/14.
//
//

#include "HelperAIFunctions.h"
#include "MathUtil.h"

using namespace cocos2d;
using namespace harp;

NS_S_HARPENGINE

bool _ai_checkFullHit(harp::Character* base, harp::Character *against, float yLayerAttackRangeCheck, bool isMidAirCheck)
{
    CCSize baseDynamicSize = base->getDynamicCollisionSize();
    CCSize againstDynamicSize = against->getDynamicCollisionSize();
    
    // check roughly first
    if(((!isMidAirCheck && !against->isInMidAir()) || isMidAirCheck) &&
       !against->isDead() &&
       fabsf(against->getPositionX() - base->getPositionX()) <= baseDynamicSize.width*2 &&
       fabsf(against->getPositionY() - base->getPositionY()) <= yLayerAttackRangeCheck)
    {
        // finer check
        CCRect baseRect = CCRectMake(base->getPositionX() - baseDynamicSize.width*0.5f, base->getPositionY() - baseDynamicSize.height*0.5f, baseDynamicSize.width, baseDynamicSize.height);
        CCRect againstRect = CCRectMake(against->getPositionX() - againstDynamicSize.width*0.5f, against->getPositionY() - againstDynamicSize.height*0.5f, againstDynamicSize.width, againstDynamicSize.height);
        
        if(baseRect.intersectsRect(againstRect))
        {
            // collided!
            return true;
        }
        else
        {
            // not collide
            return false;
        }
    }
    else
    {
        // not collide
        return false;
    }
}

bool _ai_checkFrontHit(harp::Character *base, harp::Character *against, float yLayerAttackRangeCheck, float bPercent, bool isMidAirCheck)
{
    CCSize baseDynamicSize = base->getDynamicCollisionSize();
    CCSize againstDynamicSize = against->getDynamicCollisionSize();
    
    // to be created CCRect
    CCRect baseRect, againstRect;
    
    // percent for back, and front
    // compared against half width of dynamic collision size
    const float backPercent = bPercent;
    
    // check roughly first
    if(((!isMidAirCheck && !against->isInMidAir()) || isMidAirCheck) &&
       !against->isDead() &&
       fabsf(against->getPositionX() - base->getPositionX()) <= baseDynamicSize.width*2 &&
       fabsf(against->getPositionY() - base->getPositionY()) <= yLayerAttackRangeCheck)
    {
        // facing right
        if(base->getFacingDirection() == RIGHT)
        {
            // get the pivot point
            float xPivot = base->getAnchorPoint().x;
            
            // form rects
            baseRect = CCRectMake(base->getPositionX() - baseDynamicSize.width*xPivot*backPercent,
                                  base->getPositionY() - baseDynamicSize.height*0.5f,
                                  baseDynamicSize.width*(1.0f-xPivot) + baseDynamicSize.width*xPivot*backPercent,
                                  baseDynamicSize.height);
            
            againstRect = CCRectMake(against->getPositionX() - againstDynamicSize.width*0.5f,
                                     against->getPositionY() - againstDynamicSize.height*0.5f,
                                     againstDynamicSize.width,
                                     againstDynamicSize.height);
            
            // check the collision
            if(baseRect.intersectsRect(againstRect))
                // collided
                return true;
            else
                // not collide
                return false;
        }
        // face left
        else
        {
            // get the pivot point (already converted, so we use as it is)
            float xPivot = base->getAnchorPoint().x;
            
            // form rects
            baseRect = CCRectMake(base->getPositionX() - baseDynamicSize.width*(xPivot) - baseDynamicSize.width*(1.0f-xPivot)*backPercent,
                                  base->getPositionY() - baseDynamicSize.height*0.5f,
                                  baseDynamicSize.width*xPivot + baseDynamicSize.width*(1.0f-xPivot)*backPercent,
                                  baseDynamicSize.height);
            
            againstRect = CCRectMake(against->getPositionX() - againstDynamicSize.width*0.5f,
                                     against->getPositionY() - againstDynamicSize.height*0.5f,
                                     againstDynamicSize.width,
                                     againstDynamicSize.height);
            
            // check the collision
            if(baseRect.intersectsRect(againstRect))
                // collided
                return true;
            else
                // not collide
                return false;
        }
    }
    else
        // not collide
        return false;
}

/**
 pointToWorldSpace
 
 Transforms a point from the agent's local space into world space.
 */
CCPoint pointToWorldSpace(const CCPoint& point,
                          const CCPoint& agentHeading,
                          const CCPoint& agentSide,
                          const CCPoint& agentPosition)
{
    // make a copy of the point
    kmVec2 transVec;
    kmVec2Fill(&transVec, point.x, point.y);
    
    // create a transformation matrix
    kmMat3 transMat;
    kmMat3Identity(&transMat);
    
    // rotate (from forward and side vector)
    {
        kmMat3 localMat;
        localMat.mat[0] = agentHeading.x;
        localMat.mat[1] = agentHeading.y;
        localMat.mat[2] = 0;
        localMat.mat[3] = agentSide.x;
        localMat.mat[4] = agentSide.y;
        localMat.mat[5] = 0;
        localMat.mat[6] = 0;
        localMat.mat[7] = 0;
        localMat.mat[8] = 1;
        
        // multiplay
        kmMat3Multiply(&transMat, &transMat, &localMat);
    }
    
    // translate
    kmMat3Translation(&transMat, agentPosition.x, agentPosition.y);
    
    // apply transformation matrix to point
    kmVec2Transform(&transVec, &transVec, &transMat);
    
    // return point
    return ccp(transVec.x, transVec.y);
}

bool ai_checkFullHit(harp::Character *base, harp::Character *against, float yLayerAttackRangeCheck)
{
    return _ai_checkFullHit(base, against, yLayerAttackRangeCheck, false);
}

bool ai_checkFullHitMidAir(harp::Character *base, harp::Character *against, float yLayerAttackRangeCheck)
{
    return _ai_checkFullHit(base, against, yLayerAttackRangeCheck, true);
}

bool ai_checkFrontHit(harp::Character *base, harp::Character *against, float yLayerAttackRangeCheck, float backPercent)
{
    return _ai_checkFrontHit(base, against, yLayerAttackRangeCheck, backPercent, false);
}

bool ai_checkFrontHitMidAir(harp::Character *base, harp::Character *against, float yLayerAttackRangeCheck, float backPercent)
{
    return _ai_checkFrontHit(base, against, yLayerAttackRangeCheck, backPercent, true);
}

void ai_faceToTarget(harp::CharacterAI *facer, harp::Character *target, bool isForceToFace)
{
    // if it's forced or it's the right time to face
    if(isForceToFace || facer->isReadyToTurnFacingDirection())
    {
        if(target->getPositionX() > facer->getPositionX() && facer->getFacingDirection() == LEFT)
            facer->setFlipX(false);
        else if(target->getPositionX() < facer->getPositionX() && facer->getFacingDirection() == RIGHT)
            facer->setFlipX(true);
    }
}

void ai_faceAwayFromTarget(harp::CharacterAI *facer, harp::Character *target, bool isForceToFace)
{
    // if it's forced or it's the right time to face
    if(isForceToFace || facer->isReadyToTurnFacingDirection())
    {
        // face away from target
        if(target->getPositionX() > facer->getPositionX() && facer->getFacingDirection() == RIGHT)
            facer->setFlipX(true);
        else if(target->getPositionX() < facer->getPositionX() && facer->getFacingDirection() == LEFT)
            facer->setFlipX(false);
    }
}

void ai_faceToPos(harp::CharacterAI *facer, const cocos2d::CCPoint& pos, bool isForceToFace)
{
    // if it's forced or it's the right time to face
    if(isForceToFace || facer->isReadyToTurnFacingDirection())
    {
        if(pos.x > facer->getPositionX() && facer->getFacingDirection() == LEFT)
            facer->setFlipX(false);
        else if(pos.x < facer->getPositionX() && facer->getFacingDirection() == RIGHT)
            facer->setFlipX(true);
    }
}

void ai_updateCountingTimeWithDeltaTime(float& ctime, float dt)
{
    ctime += dt;
}

bool ai_isTimeout(float ctime, float checkAgainst)
{
    if(ctime >= checkAgainst)
        return true;
    else
        return false;
}

void ai_resetCountingTime(float& ctime)
{
    ctime = 0.0f;
}

void ai_resetCountingTime(float& ctime, float checkAgainst)
{
    ctime -= checkAgainst;
}

cocos2d::CCPoint ai_getDirectionToTarget(harp::Character *target, harp::Character *base)
{
    return ccpNormalize(ccpSub(target->getPosition(), base->getPosition()));
}

cocos2d::CCPoint ai_getOppositeDirectionToTarget(harp::Character *target, harp::Character *base)
{
    return ccpNormalize(ccpSub(base->getPosition(), target->getPosition()));
}

float ai_getDistanceToTarget(harp::Character *target, harp::Character *base)
{
    return ccpDistance(target->getPosition(), base->getPosition());
}

float ai_getDistanceSQToTarget(harp::Character *target, harp::Character *base)
{
    return ccpDistanceSQ(target->getPosition(), base->getPosition());
}

float ai_getAbsYPosDistanceBetween(harp::Character *char1, harp::Character *char2)
{
    return fabsf(char1->getPositionY() - char2->getPositionY());
}

CCPoint ai_getRandomPositionWithinMap(harp::Character *c, const cocos2d::CCRect& mapRect, float error)
{
    // get anchor point of character
    CCPoint cAnchorPoint = c->getAnchorPoint();
    
    // create a bounding rectangle to randomize position within
    float x = mapRect.origin.x + error + (c->getStaticCollisionSize().width * cAnchorPoint.x);
    float y = mapRect.origin.y + error + (c->getStaticCollisionSize().height * cAnchorPoint.y);
    float width = (mapRect.origin.x + mapRect.size.width - error - c->getStaticCollisionSize().width * (1.0f-cAnchorPoint.x)) - x;
    // make it 95% of what can be as it's not accurate perfect as displayFrame() but with this we get less expensive call
    float height = ((mapRect.origin.y + mapRect.size.height - error + c->getStaticCollisionSize().height * cAnchorPoint.y) - y) * MAXIMUM_Y_POS_CHARACTER_ADJUSTMENT_PERCENT;
    
    // random the position from the bounding rectangle
    return MathUtil::sharedInstance()->randomPointFrom(CCRectMake(x, y, width, height));
}

CCPoint ai_getRandomPositionWithinMaps(harp::Character *c, const std::vector<CCRect> mapRects, float error)
{
    CCAssert(mapRects.size() > 0, "Check the size of vector, must be more than 0.");
    
    // random which rectangle to be random position within
    int rectIndex = MathUtil::sharedInstance()->randomIntegerBetween(0, mapRects.size());
    
    // get selected rectangle
    CCRect mapRect = mapRects[rectIndex];
    
    // random the position from the bounding rectangle
    return ai_getRandomPositionWithinMap(c, mapRect, error);
}

int ai_getMapRectNumberOf(Character *c, I8MapRectsOperational* eightMapInstance)
{
    CCAssert(eightMapInstance != NULL, "Check if eightMapInstance is not null");
    
    // get vector of 8 map-rects
    std::vector<CCRect> mapRects = eightMapInstance->get8MapRects();
    
    // get character's position
    CCPoint cPos = c->getPosition();
    // take into account the y-anchor point
    // for some characters esp if they're tall, their position might appear above the limit position
    // this prevents below CCAssert() to pass
    cPos.y -= c->getStaticCollisionSize().height * c->getAnchorPoint().y;
    
    // loop through all the map-rects then
    for(int i=0; i<mapRects.size(); i++)
    {
        // get the map rect
        CCRect rect = mapRects[i];
        
        // check if position is in the current rectangle or not
        if(rect.containsPoint(cPos))
        {
            return i+1;
        }
    }
    
    CCAssert(false, "Map rect position must be found.");
}

int ai_getMapRectNumberButAvoidOf(Character* c, I8MapRectsOperational* eightMapInstance)
{
    // get the current map-rect number from input character
    int mapRectNumber = ai_getMapRectNumberOf(c, eightMapInstance);
    
    // create an array of 1-8 but not include mapRectNumber
    int rNumArray[7];
    int arrayIndex = 0;
    for(int n=1; n<=8; n++)
    {
        // if not match, then add into an array
        if(n != mapRectNumber)
        {
            rNumArray[arrayIndex++] = n;
        }
    }
    
    // random index, then use it to get map-rect number from an array
    return rNumArray[MathUtil::sharedInstance()->randomIntegerBetween(0, 7)];
}

void ai_getSmoothKnockbackParameterToNearestStageEdge(harp::Character *attacker, harp::Character *against, float stageLimitMinX, float stageLimitMaxX, float &outDistance, float &outDuration)
{
    // find the distance of knockback
    // note: we can find it via comparing the x-position of both instances
    // - find distance to the right edge of the screen
    if(attacker->getPositionX() < against->getPositionX())
    {
        outDistance = stageLimitMaxX - against->getPositionX();
    }
        // - find distance to the left edge of the screen
    else
    {
        outDistance = against->getPositionX() - stageLimitMinX;
    }

    // add a little noise to knockback distance
    outDistance *= MathUtil::sharedInstance()->randomFloatBetween(0.60f, 1.0f);

    // calculate the duration of the knockback
    outDuration = 1.15f / HarpConvertLength(900.0f) * outDistance;
}

float ai_getSmoothKnockbackDurationForDistance(float distance)
{
    // calculate the duration of the knockback
    return 1.15f / HarpConvertLength(900.0f) * distance;
}

NS_E_HARPENGINE