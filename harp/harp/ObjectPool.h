//
//  ObjectPool.h
//  ZombieHero
//
//  Created by haxpor on 2/17/13.
//
//

#ifndef __ZombieHero__ObjectPool__
#define __ZombieHero__ObjectPool__

#include "CoreMacros.h"
#include "cocos2d.h"
#include "Interfaces/IRecyclableObject.h"

NS_S_HARPENGINE

/**
 * Abstract ObjectPool class 
 *
 * This ObjectPool class works best with small number of objects in the pool.
 * Around 50 - 100 should be fine with no noticeable performance hit.
 * 
 * V.0.1 : Users need to allocate number of objects higher than being used in the game as always as the class has no ability to grow in run-time.
 */
class ObjectPool
{
public:
    ObjectPool();
    virtual ~ObjectPool();
    
    virtual bool init(unsigned int n, bool allowToGrow=false, int growPackCount=0);
    
    virtual void initPool() = 0;
    virtual void releasePool();
    /* Overwrite this function to define how object pool can grow itself. Even if object pool is allowed to grow, but growObjectByOne() is not defined, then it has no effect. */
    virtual IRecyclableObject* growObjectByPacks(int recycleTypeID);
    
    bool isAllowToGrow();
    unsigned int getBaseNumberOfAllocatedObjects();
    unsigned int getCurrentNumberOfAllocatedObjects();
    int getGrowPackCount();
    void setGrowPackCount(int growPackCount);
    bool isAnyFreeObjects();
    
    cocos2d::CCArray* getFreeList();
    cocos2d::CCArray* getActiveList();
    
    void recycle(IRecyclableObject* obj);
    IRecyclableObject* popFreeObjectAndAddToActiveListForType(int recycleTypeID);
    
protected:
    cocos2d::CCArray *m_freeList;
    cocos2d::CCArray *m_activeList;
    int m_growPackCount;
    
    unsigned int m_baseNumberOfObjects;
    bool m_isAllowToGrow;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__ObjectPool__) */
