//
//  I8MapRectsOperational.h
//  ZombieHero
//
//  Created by haxpor on 12/23/14.
//
//

#ifndef ZombieHero_I8MapRectsOperational_h
#define ZombieHero_I8MapRectsOperational_h

#include "cocos2d.h"
#include <vector>

NS_S_HARPENGINE

/**
 A class that needs to be able to call on those 8 map rectangles related function has to define all the following interfaces.
 */
class I8MapRectsOperational
{
public:
    virtual ~I8MapRectsOperational() {}
    
    /**
     Get 8 map-rects vector that is pre-constructed in an order of upper to lower row, and left to right column.
     */
    virtual const std::vector<cocos2d::CCRect>& get8MapRects() = 0;
};

NS_E_HARPENGINE

#endif
