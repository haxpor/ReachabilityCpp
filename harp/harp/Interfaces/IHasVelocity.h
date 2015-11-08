//
//  IHasVelocity.h
//  ZombieHero
//
//  Created by haxpor on 4/17/13.
//
//

#ifndef ZombieHero_IHasVelocity_h
#define ZombieHero_IHasVelocity_h

#include "cocos2d.h"
#include "Core.h"

NS_S_HARPENGINE

class IHasVelocity
{
public:
    virtual ~IHasVelocity() {}
    
    virtual const cocos2d::CCPoint& getVelocity() = 0;
    virtual float getVelocityX() = 0;
    virtual float getVelocityY() = 0;
    virtual void setVelocity(const cocos2d::CCPoint& velocity) = 0;
    virtual void setVelocity(float velX, float velY) = 0;
    virtual void setVelocityX(float velX) = 0;
    virtual void setVelocityY(float velY) = 0;
};

NS_E_HARPENGINE

#endif
