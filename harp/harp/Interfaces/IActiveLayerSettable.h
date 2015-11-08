//
//  IActiveLayerSettable.h
//  ZombieHero
//
//  Created by haxpor on 6/24/13.
//
//

#ifndef ZombieHero_IActiveLayerSettable_h
#define ZombieHero_IActiveLayerSettable_h

#include "Core.h"

NS_S_HARPENGINE

class IActiveLayerSettable
{
public:
    virtual ~IActiveLayerSettable() {}
    
    virtual void disableLayer() = 0;
    virtual void enableLayer() = 0;
};

NS_E_HARPENGINE

#endif
