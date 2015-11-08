//
//  HelperAIFunctions.h
//  ZombieHero
//
//  Created by haxpor on 3/19/14.
//
//

#ifndef __ZombieHero__HelperAIFunctions__
#define __ZombieHero__HelperAIFunctions__

#include "Core.h"
#include "kazmath.h"
#include "CCPointExtension.h"
#include "Character.h"
#include "CharacterAI.h"
#include "I8MapRectsOperational.h"

NS_S_HARPENGINE

#define MAXIMUM_Y_POS_CHARACTER_ADJUSTMENT_PERCENT 0.95f

/**
 Internal functions
 */
// Internal function wrap core functionality for checking hit functions
bool _ai_checkFullHit(harp::Character* base, harp::Character *against, float yLayerAttackRangeCheck, bool isMidAirCheck);
bool _ai_checkFrontHit(harp::Character *base, harp::Character *against, float yLayerAttackRangeCheck, bool isMidAirCheck);
/**
 End of Internal functions
 */

/**
 pointToWorldSpace
 
 Transforms a point from the agent's local space into world space.
 */
cocos2d::CCPoint pointToWorldSpace(const cocos2d::CCPoint& point,
                                   const cocos2d::CCPoint& agentHeading,
                                   const cocos2d::CCPoint& agentSide,
                                   const cocos2d::CCPoint& agentPosition);

/**
 ai_checkFullHit
 
 Check collision between two characters in full size of character's sprite.
 */
bool ai_checkFullHit(harp::Character *base, harp::Character *against, float yLayerAttackRangeCheck=HarpConvertLength(30));

/**
 ai_checkFullHitMidAir
 
 Check collision similar to ai_checkFullHit() but it's in mid-air.
 */
bool ai_checkFullHitMidAir(harp::Character *base, harp::Character *against, float yLayerAttackRangeCheck=HarpConvertLength(30));

/**
 ai_checkFrontHit
 
 Check collision between two characters in front-based of character's sprite.
 */
bool ai_checkFrontHit(harp::Character *base, harp::Character *against, float yLayerAttackRangeCheck=HarpConvertLength(30), float backPercent=0.010f);

/**
 ai_checkFrontHitMidAir
 
 Check collision between two characters in front-based of character's sprite but in mid-air.
 */
bool ai_checkFrontHitMidAir(harp::Character *base, harp::Character *against, float yLayerAttackRangeCheck=HarpConvertLength(30), float backPercent=0.010f);

/**
 ai_faceToTarget()
 
 Face facer to target. If isForceToFace is true, then it won't consider isReadyToTurnFacingDirection().
 */
void ai_faceToTarget(harp::CharacterAI *facer, harp::Character *target, bool isForceToFace=false);

/**
 ai_faceAwayFromTarget()
 
 Face facer away from target. If isForceToFace is true, then it won't consider isReadyToTurnFacingDirection().
 */
void ai_faceAwayFromTarget(harp::CharacterAI *facer, harp::Character *target, bool isForceToFace=false);

/**
 ai_faceToPos()
 
 Face facer to pos.
 */
void ai_faceToPos(harp::CharacterAI *facer, const cocos2d::CCPoint& pos, bool isForceToFace=false);

/**
 Update counting time with dt.
 */
void ai_updateCountingTimeWithDeltaTime(float& ctime, float dt);

/**
 Check if counting time is timed-out.
 */
bool ai_isTimeout(float ctime, float checkAgainst);

/**
 Reset counting with zero.
 */
void ai_resetCountingTime(float& ctime);

/**
 Reset counting accurately with checkAgainst time.
 */
void ai_resetCountingTime(float &ctime, float checkAgainst);

/**
 Get normalized direction to target.
 */
cocos2d::CCPoint ai_getDirectionToTarget(harp::Character *target, harp::Character *base);

/**
 Get normalized opposite direction to target.
 */
cocos2d::CCPoint ai_getOppositeDirectionToTarget(harp::Character *target, harp::Character* base);

/**
 Get distance from base to target.
 */
float ai_getDistanceToTarget(harp::Character *target, harp::Character *base);

/**
 Get squared distance from base to target.
 */
float ai_getDistanceSQToTarget(harp::Character *target, harp::Character *base);

/**
 Get distance of y-position between two characters.
 */
float ai_getAbsYPosDistanceBetween(harp::Character *char1, harp::Character *char2);

/**
 Get random position within the map.
 */
cocos2d::CCPoint ai_getRandomPositionWithinMap(harp::Character *c, const cocos2d::CCRect& mapRect, float error=0.0f);

/**
 Get random position within the maps.
 */
cocos2d::CCPoint ai_getRandomPositionWithinMaps(harp::Character *c, const std::vector<cocos2d::CCRect> mapRects, float error=0.0f);

/**
 8 map-rects
 Functions that work in context of 8 map areas.
 See interface class of I8MapRectsOperational.h to define them in user-defined class.
 */

/**
 Get a map-rect number indicating the position of its rectangle in context of 8 map-rects from the input character.
 Return number of map-rect position (1 to 8).
 */
int ai_getMapRectNumberOf(harp::Character *c, I8MapRectsOperational* eightMapInstance);

/**
 Get a map-rect number indicating the position of its rectangle in context of 8 map-rects randomizing from the map-rect number of input character that currently resides..
 */
int ai_getMapRectNumberButAvoidOf(harp::Character *c, I8MapRectsOperational* eightMapInstance);

/**
 Get smooth knockback parameter back for knockback distance, and duration.
 */
void ai_getSmoothKnockbackParameterToNearestStageEdge(harp::Character *attacker, harp::Character *against, float stageLimitMinX, float stageLimitMaxX, float& outDistance, float& outDuration);

/**
 Get smooth knockback parameter back for knockback duration.
 */
float ai_getSmoothKnockbackDurationForDistance(float distance);

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__HelperAIFunctions__) */
