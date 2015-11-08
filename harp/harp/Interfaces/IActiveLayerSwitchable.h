//
//  IActiveLayerSwitchable.h
//  ZombieHero
//
//  Created by haxpor on 4/20/13.
//
//

#ifndef __ZombieHero__IActiveLayerSwitchable__
#define __ZombieHero__IActiveLayerSwitchable__

#include "Core.h"

NS_S_HARPENGINE

class IActiveLayerSwitchable
{
public:
    virtual ~IActiveLayerSwitchable() {}
    
    virtual void prepareToSwitchActiveLayerToAnother(void* data) = 0;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__IActiveLayerSwitchable__) */
