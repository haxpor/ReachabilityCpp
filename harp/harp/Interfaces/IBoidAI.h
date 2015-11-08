//
//  IBoidAI.h
//  ZombieHero
//
//  Created by haxpor on 4/17/13.
//
//

#ifndef __ZombieHero__IBoidAI__
#define __ZombieHero__IBoidAI__

#include "Core.h"
#include "BoidAIController.h"

NS_S_HARPENGINE

class IBoidAI
{
public:
    virtual ~IBoidAI() {}
    
    virtual harp::BoidAIController* getBoidAIController() = 0;             // strict to use BoidAIController class
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__IBoidAI__) */
