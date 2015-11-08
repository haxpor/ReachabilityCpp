//
//  Wall2D.h
//  ZombieHero
//
//  Created by haxpor on 3/10/14.
//
//

#ifndef __ZombieHero__Wall2D__
#define __ZombieHero__Wall2D__

#include "Core.h"

NS_S_HARPENGINE

class Wall2D
{
public:
    Wall2D();
    Wall2D(const cocos2d::CCPoint& a, const cocos2d::CCPoint& b);
    Wall2D(const cocos2d::CCPoint& a, const cocos2d::CCPoint& b, const cocos2d::CCPoint& n);
    
    const cocos2d::CCPoint& from() const;
    void setFrom(const cocos2d::CCPoint& v);
    
    const cocos2d::CCPoint& to() const;
    void setTo(const cocos2d::CCPoint& v);
    
    const cocos2d::CCPoint& normal() const;
    void setNormal(const cocos2d::CCPoint& normal);
    
    cocos2d::CCPoint center() const;
    
protected:
    cocos2d::CCPoint m_A;
    cocos2d::CCPoint m_B;
    cocos2d::CCPoint m_N;
    
    void calculateNormal();
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__Wall2D__) */
