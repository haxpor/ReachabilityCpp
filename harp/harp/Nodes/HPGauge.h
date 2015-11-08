//
//  PlayerBars.h
//  ZombieHero
//
//  Created by haxpor on 12/29/12.
//
//

#ifndef __ZombieHero__HPGauge__
#define __ZombieHero__HPGauge__

#include "cocos2d.h"
#include "Core.h"

NS_S_HARPENGINE

class HPGauge : public cocos2d::CCNode
{
public:
    HPGauge();
    virtual ~HPGauge();
    
    /**
     Init with start HP, and HP's spriteframe names.
     */
    virtual bool initWithStartHP(int startHP, int gaugeRange, const char* hpBG_sfName, const char* hp1_sfName, const char* hp2_sfName, const char* hp3_sfName, const char* hp4_sfName, const char* hp5_sfName, const char* hp6_sfName, const char* hpFrame_sfName);
    
    /**
     Update HP
     */
    virtual void setHP(int hp);
    int getHP();
    
    /**
     Get gauge range.
     */
    int getGaugeRange();
    
    static HPGauge* create(int startHP, int gaugeRange, const char* hpBG_sfName, const char* hp1_sfName, const char* hp2_sfName, const char* hp3_sfName, const char* hp4_sfName, const char* hp5_sfName, const char* hp6_sfName, const char* hpFrame_sfName);

protected:
    cocos2d::CCSprite* m_hpBG;
    cocos2d::CCSprite* m_hp1;
    cocos2d::CCSprite* m_hp2;
    cocos2d::CCSprite* m_hp3;
    cocos2d::CCSprite* m_hp4;
    cocos2d::CCSprite* m_hp5;
    cocos2d::CCSprite* m_hp6;
    cocos2d::CCSprite* m_hpFrame;
    
    int m_hp;
    int m_gaugeRange;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__HPGauge__) */
