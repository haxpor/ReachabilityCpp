//
//  ObjectPool.cpp
//  ZombieHero
//
//  Created by haxpor on 2/17/13.
//
//

#include "ObjectPool.h"
#include "Core.h"
#include "CharacterAI.h"

using namespace harp;
using namespace cocos2d;

ObjectPool::ObjectPool() :
    m_freeList(NULL),
    m_activeList(NULL),
    m_isAllowToGrow(false),
    m_baseNumberOfObjects(20)
{
}

ObjectPool::~ObjectPool()
{
    if(m_freeList)
    {
        m_freeList->removeAllObjects();
        CC_SAFE_RELEASE_NULL(m_freeList);
    }
    
    if(m_activeList)
    {
        m_activeList->removeAllObjects();
        CC_SAFE_RELEASE_NULL(m_activeList);
    }
}

/**
 * Recycle the specified object by reset its states, add into free-list and remove from active-list.
 *
 */
void ObjectPool::recycle(IRecyclableObject* obj)
{
    if(obj == NULL)
        return;
    
    // check if obj is currently in active-list
    for(unsigned int i=0; i<m_activeList->count(); i++)
    {
        IRecyclableObject* objInterface = dynamic_cast<IRecyclableObject*>(m_activeList->objectAtIndex(i));
        if(obj == objInterface)
        {
            // recycle (reset states and value)
            objInterface->recycle();
            // add into free-list (add first before removing to prevent from fully release)
            m_freeList->addObject(m_activeList->objectAtIndex(i));
            // remove it from active-list
            m_activeList->removeObjectAtIndex(i);

            return;
        }
    }
}

/**
 * Pop first found free object that has "recycleTypeID" the same as specified and add to active-list immediately, otherwise return NULL if not found or not allowed to grow.
 *
 */
IRecyclableObject* ObjectPool::popFreeObjectAndAddToActiveListForType(int recycleTypeID)
{
    // search from free-list
    for(unsigned int i=0; i<m_freeList->count(); i++)
    {
        IRecyclableObject* obj = dynamic_cast<IRecyclableObject*>(m_freeList->objectAtIndex(i));
        if(obj->getRecycleTypeID() == recycleTypeID)
        {
            CCObject* objCasted = m_freeList->objectAtIndex(i);
            // add to active-list
            m_activeList->addObject(objCasted);
            
            // remove from free-list
            m_freeList->removeObjectAtIndex(i);
            
            return obj;
        }
    }
    
    // if no available object, then create new one if allowed
    if(m_isAllowToGrow)
    {
        return growObjectByPacks(recycleTypeID);
    }
    else
        return NULL;
}

void ObjectPool::releasePool()
{
    // dump all list
    unsigned int i=0;
    for(i=0 ; i<m_freeList->count(); i++)
    {
        IRecyclableObject* obj = dynamic_cast<IRecyclableObject*>(m_freeList->objectAtIndex(i));
        obj->dump();
        m_freeList->removeObjectAtIndex(i--);
    }
    for(i=0 ; i<m_activeList->count(); i++)
    {
        IRecyclableObject* obj = dynamic_cast<IRecyclableObject*>(m_activeList->objectAtIndex(i));
        obj->dump();
        m_activeList->removeObjectAtIndex(i--);
    }
    
    // remove all objects
    m_freeList->removeAllObjects();
    m_activeList->removeAllObjects();
}

IRecyclableObject* ObjectPool::growObjectByPacks(int recycleTypeID)
{
    // do nothing in base as default is not allowed to grow
    return NULL;
}

bool ObjectPool::init(unsigned int n, bool allowToGrow, int growPackCount)
{
    m_baseNumberOfObjects = n;
    m_isAllowToGrow = allowToGrow;
    m_growPackCount = growPackCount;
    
    m_freeList = CCArray::createWithCapacity(n);
    m_freeList->retain();
    m_activeList = CCArray::createWithCapacity(n);
    m_activeList->retain();
    
    return true;
}

/**
 * Create initial pool.
 *
 */
void ObjectPool::initPool()
{
    // Derived classs must override this function.
}

bool ObjectPool::isAllowToGrow()
{
    return m_isAllowToGrow;
}

/**
 * Get the first time allocation number of objects in the pool.
 *
 */
unsigned int ObjectPool::getBaseNumberOfAllocatedObjects()
{
    return m_baseNumberOfObjects;
}

/* Beware: it needs to recalculate everytime this function is called. */
unsigned int ObjectPool::getCurrentNumberOfAllocatedObjects()
{
    return m_freeList->count() + m_activeList->count();
}

bool ObjectPool::isAnyFreeObjects()
{
    if(m_freeList->count() > 0)
        return true;
    else
        return false;
}

cocos2d::CCArray* ObjectPool::getFreeList()
{
    return m_freeList;
}

cocos2d::CCArray* ObjectPool::getActiveList()
{
    return m_activeList;
}

int ObjectPool::getGrowPackCount()
{
    return m_growPackCount;
}

void ObjectPool::setGrowPackCount(int growPackCount)
{
    m_growPackCount = growPackCount;
}
