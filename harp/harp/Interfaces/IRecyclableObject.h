//
//  IRecyclableObject.h
//  ZombieHero
//
//  Created by haxpor on 2/17/13.
//
//

#ifndef ZombieHero_IRecyclable_h
#define ZombieHero_IRecyclable_h

#include "Core.h"

NS_S_HARPENGINE

class IRecyclableObject
{
public:
    virtual ~IRecyclableObject() {}
    
    virtual void recycle() = 0;             // not being called by users manually, just provide the system the way to recycle (reset states) the object
    virtual void dump() = 0;                // not being called by users manually, just provide the system the way to dump (release resource) the object
    virtual int getRecycleTypeID() = 0;
};

NS_E_HARPENGINE

#endif
