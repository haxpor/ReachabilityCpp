//
//  Gauge.h
//  ZombieHero
//
//  Created by haxpor on 2/5/14.
//
//

#ifndef __ZombieHero__Gauge__
#define __ZombieHero__Gauge__

#include "Core.h"

NS_S_HARPENGINE

class Gauge : public cocos2d::CCNode
{
public:
    Gauge();
    virtual ~Gauge();
    
    virtual bool initWithValueAndTexturePaths(float value, float min, float max, const char* bgSpriteFrameName, const char* valueSpriteFrameName, const char* gaugeSpriteFrameName, bool isHorizontal=true);
    
    HARP_SYNTHESIZE_NONVIRTUAL(float, m_minGaugeValue, MinGaugeValue);
    HARP_SYNTHESIZE_NONVIRTUAL(float, m_maxGaugeValue, MaxGaugeValue);
    HARP_PROPERTY_NONVIRTUAL(float, m_currentGaugeValue, GaugeValue);
    
    bool getIsHorizontal();
    
    /**
     @brief Create a new gauge.
     */
    static Gauge* create(float value, float min, float max, const char* bgSpriteFrameName, const char* valueSpriteFrameName, const char* gaugeSpriteFrameName, bool isHorizontal=true);
    
protected:
    cocos2d::CCSprite* m_gaugeBG;
    cocos2d::CCSprite* m_gaugeValue;
    cocos2d::CCSprite* m_gaugeFrame;
    
    bool m_isHorizontal;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__Gauge__) */
