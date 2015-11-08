//
//  IGameWorldAIProtocol.h
//  ZombieHero
//
//  Created by haxpor on 3/11/14.
//
//

#ifndef ZombieHero_IGameWorldAIProtocol_h
#define ZombieHero_IGameWorldAIProtocol_h

#include "Core.h"
#include <vector>
#include "Wall2D.h"
#include "CCArray.h"

NS_S_HARPENGINE

class CharacterAI;

/*
 ===============================================================================
 
 IGameWorldAIProtocol
 
 Object that will act as a Game world provoding information for AI should adhere
 to this interface.
 
 ===============================================================================
 */
class IGameWorldAIProtocol
{
public:
    virtual ~IGameWorldAIProtocol() {}
    
    /**
     getWalls
     
     Get wall information to be used in steering AI for agents in the game.
     */
    virtual const std::vector<harp::Wall2D>& getWalls() const = 0;
    
    /**
     getAgents
     
     Get all agents in the game world.
     */
    virtual const cocos2d::CCArray* getAgents() const = 0;
    
    /**
     tagVehiclesWithinViewRange
     
     Tag vehicles within given vehicle's range.
     */
    virtual void tagVehiclesWithinViewRange(CharacterAI *vehicle, float range) = 0;
};

NS_E_HARPENGINE

#endif
