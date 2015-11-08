//
//  HelperAIFunctionTemplate.h
//  ZombieHero
//
//  Created by haxpor on 3/11/14.
//
//

#ifndef ZombieHero_HelperAIFunctionTemplate_h
#define ZombieHero_HelperAIFunctionTemplate_h

#include "CCGeometry.h"
#include "CCPointExtension.h"
#include "CCArray.h"
#include "Core.h"

NS_S_HARPENGINE

/*
 ===============================================================================
 
 TagNeighbors
 
 Tags any agents in agents-containment that are within the radius of the single
 entity parameter.
 
 ===============================================================================
 */
template <class T>
void TagNeighbors(const T* entity, const cocos2d::CCArray* agents, float radius)
{
    // do dirty cast (but still ensure non-modification)
    cocos2d::CCArray* cagents = (cocos2d::CCArray*)agents;
    
    // iterate through all entities checking for range
    for(unsigned int i=0; i<cagents->count(); i++)
    {
        // get the element object via casting
        T* curEntity = dynamic_cast<T*>(cagents->objectAtIndex(i));
        
        CCAssert(curEntity != NULL, "curEntity should be able to cast, and not NULL.");
        
        // first clear any current tag
        curEntity->unTag();
        
        cocos2d::CCPoint to = ccpSub(curEntity->getPosition(), ((T*)entity)->getPosition());
        
        // if entity within range, tag for further consideration (working in distance-squared space to avoid sqrts)
        if( (curEntity != entity) && (ccpLengthSQ(to) < radius*radius))
        {
            curEntity->tag();
        }
    }
}

/*
 ===============================================================================
 
 enforceNonPenetrationConstraint
 
 Ensure that entities are not overlapped.
 
 ===============================================================================
 */
template <class T>
void enforceNonPenetrationConstraint(T* entity, const cocos2d::CCArray* allEntities, bool (*pBoundingFunc)(T*))
{
    // only when meet the conditions
    if(entity->isAlive() &&
       !entity->isInMidAir() &&
       !entity->isAttacking() &&
       entity->getIsEnabledAI()
       )
    {
        // do dirty cast (but still ensure non-modification)
        cocos2d::CCArray* cAllEntities = (cocos2d::CCArray*)allEntities;
        
        // iterate through all
        for (unsigned int i=0; i<cAllEntities->count(); i++) {
            // get the element object via casting
            T* curEntity = dynamic_cast<T*>(cAllEntities->objectAtIndex(i));
            
            CCAssert(curEntity != NULL, "curEntity should be able to cast, and not NULL.");
            
            // make sure we don't check against this entity
            if(curEntity == entity)
                continue;
            
            // calculate the distance between the positions of the entities
            cocos2d::CCPoint toEntity = ccpSub(entity->getPosition(), curEntity->getPosition());
            
            // calculate lengthSq to avoid dealing with sqrt
            float distFromEachOtherSQ = ccpLengthSQ(toEntity);
            if(distFromEachOtherSQ < HarpConvertLength(100) * HarpConvertLength(100))
            {
                // calculate sqrt
                float distFromEachOther = sqrtf(distFromEachOtherSQ);
            
                // tweaking factor, higher value will make entities more pack together
                const float closeProximityFactor = 6.0f;
                
                // if this distance is smaller than the sum of their radii then this entity must be moved away
                // in the direction parallel to the toEntity vector
                float amountOfOverlap = curEntity->getBoundingRadius()/closeProximityFactor + entity->getBoundingRadius()/closeProximityFactor - distFromEachOther;
                if(amountOfOverlap >= 0)
                {
                    // move the entity a distance away equivalent to the amount of overlap
                    entity->setPosition(ccpAdd(entity->getPosition(), ccpMult(toEntity, amountOfOverlap/distFromEachOther*ENFORCER_NONPENETRATION_EFFICIENT_MULTIPLIER)));
                    
                    // bound this entity
                    (*pBoundingFunc)(entity);
                }
            }
        }
    }
}

NS_E_HARPENGINE

#endif
