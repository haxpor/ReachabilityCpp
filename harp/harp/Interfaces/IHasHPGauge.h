//
//  IHasHPGauge.h
//  ZombieHero
//
//  Created by haxpor on 2/17/14.
//
//

#ifndef ZombieHero_IHasHPGauge_h
#define ZombieHero_IHasHPGauge_h

#include "Core.h"

NS_S_HARPENGINE

class IHasHPGauge
{
public:
    virtual ~IHasHPGauge() {};
    
    virtual void updateHPGauge(int hp) = 0;
    virtual void showHPGauge() = 0;
    virtual void hideHPGauge() = 0;
};

NS_E_HARPENGINE

#endif
