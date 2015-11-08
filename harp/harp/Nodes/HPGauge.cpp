//
//  PlayerBars.cpp
//  ZombieHero
//
//  Created by haxpor on 12/29/12.
//
//

#include "HPGauge.h"

using namespace cocos2d;
using namespace harp;

HPGauge::HPGauge():
    m_hpBG(NULL),
    m_hp1(NULL),
    m_hp2(NULL),
    m_hp3(NULL),
    m_hp4(NULL),
    m_hp5(NULL),
    m_hp6(NULL),
    m_hpFrame(NULL),
    m_gaugeRange(100)
{
}

HPGauge::~HPGauge()
{
    CCLOGINFO("Destroying HPGauge");
    
    // release its ui-element
    m_hpBG->release();
    m_hp1->release();
    m_hp2->release();
    m_hp3->release();
    m_hp4->release();
    m_hp5->release();
    m_hp6->release();
    m_hpFrame->release();
}

bool HPGauge::initWithStartHP(int startHP, int gaugeRange, const char* hpBG_sfName, const char* hp1_sfName, const char* hp2_sfName, const char* hp3_sfName, const char* hp4_sfName, const char* hp5_sfName, const char* hp6_sfName, const char* hpFrame_sfName)
{
    Core* core = Core::sharedCore();
    
    // set gauge's range
    m_gaugeRange = gaugeRange;
    
    // act like a layer or scene by ignore position
    ignoreAnchorPointForPosition(true);
    
    // set up sprites
    // HP
    m_hpBG = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(hpBG_sfName));
    m_hpBG->setAnchorPoint(ccp(0.0f, 0.5f));
    m_hpBG->setPosition(ccp(0, core->convertPointY(9)));
    m_hpBG->retain();
    
    // 1
    m_hp1 = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(hp1_sfName));
    m_hp1->setAnchorPoint(ccp(0.0f, 0.5f));
    m_hp1->setPosition(ccp(core->convertPointX(2), core->convertPointY(9)));
    m_hp1->retain();
    
    // 2
    m_hp2 = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(hp2_sfName));
    m_hp2->setAnchorPoint(ccp(0.0f, 0.5f));
    m_hp2->setPosition(ccp(core->convertPointX(2), core->convertPointY(9)));
    m_hp2->retain();
    
    // 3
    m_hp3 = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(hp3_sfName));
    m_hp3->setAnchorPoint(ccp(0.0f, 0.5f));
    m_hp3->setPosition(ccp(core->convertPointX(2), core->convertPointY(9)));
    m_hp3->retain();
    
    // 4
    m_hp4 = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(hp4_sfName));
    m_hp4->setAnchorPoint(ccp(0.0f, 0.5f));
    m_hp4->setPosition(ccp(core->convertPointX(2), core->convertPointY(9)));
    m_hp4->retain();
    
    // 5
    m_hp5 = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(hp5_sfName));
    m_hp5->setAnchorPoint(ccp(0.0f, 0.5f));
    m_hp5->setPosition(ccp(core->convertPointX(2), core->convertPointY(9)));
    m_hp5->retain();
    
    // 6
    m_hp6 = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(hp6_sfName));
    m_hp6->setAnchorPoint(ccp(0.0f, 0.5f));
    m_hp6->setPosition(ccp(core->convertPointX(2), core->convertPointY(9)));
    m_hp6->retain();
    
    // Frame
    m_hpFrame = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(hpFrame_sfName));
    m_hpFrame->setAnchorPoint(ccp(0.0f, 0.5f));
    m_hpFrame->setPosition(ccp(core->convertPointX(-4), core->convertPointY(12)));
    m_hpFrame->retain();
    
    // set hp value (gauge value)
    setHP(startHP);
    
    // add as childs
    this->addChild(m_hpBG);
    this->addChild(m_hp1);
    this->addChild(m_hp2);
    this->addChild(m_hp3);
    this->addChild(m_hp4);
    this->addChild(m_hp5);
    this->addChild(m_hp6);
    this->addChild(m_hpFrame);
    
    return true;
}

void HPGauge::setHP(int hp)
{
    // save hp
    m_hp = hp;
    
    if(hp < 0)
    {
        m_hp1->setVisible(false);
        m_hp2->setVisible(false);
        m_hp3->setVisible(false);
        m_hp4->setVisible(false);
        m_hp5->setVisible(false);
        m_hp6->setVisible(false);
    }
    else if(hp <= this->getGaugeRange())
    {
        m_hp1->setVisible(true);
        m_hp2->setVisible(false);
        m_hp3->setVisible(false);
        m_hp4->setVisible(false);
        m_hp5->setVisible(false);
        m_hp6->setVisible(false);
        
        m_hp1->setScaleX(hp*1.0f / this->getGaugeRange());
    }
    else if(hp <= this->getGaugeRange() * 2)
    {
        m_hp1->setVisible(true);
        m_hp2->setVisible(true);
        m_hp3->setVisible(false);
        m_hp4->setVisible(false);
        m_hp5->setVisible(false);
        m_hp6->setVisible(false);
        
        m_hp1->setScaleX(1.0f);
        m_hp2->setScaleX((hp*1.0f-this->getGaugeRange()) / this->getGaugeRange());
    }
    else if(hp <= this->getGaugeRange() * 3)
    {
        m_hp1->setVisible(false);
        m_hp2->setVisible(true);
        m_hp3->setVisible(true);
        m_hp4->setVisible(false);
        m_hp5->setVisible(false);
        m_hp6->setVisible(false);
        
        m_hp1->setScaleX(1.0f);
        m_hp2->setScaleX(1.0f);
        m_hp3->setScaleX((hp*1.0f-this->getGaugeRange()*2) / this->getGaugeRange());
    }
    else if(hp <= this->getGaugeRange() * 4)
    {
        m_hp1->setVisible(false);
        m_hp2->setVisible(false);
        m_hp3->setVisible(true);
        m_hp4->setVisible(true);
        m_hp5->setVisible(false);
        m_hp6->setVisible(false);

        m_hp1->setScaleX(1.0f);
        m_hp2->setScaleX(1.0f);
        m_hp3->setScaleX(1.0f);
        m_hp4->setScaleX((hp*1.0f-this->getGaugeRange()*3) / this->getGaugeRange());
    }
    else if(hp <= this->getGaugeRange() * 5)
    {
        m_hp1->setVisible(false);
        m_hp2->setVisible(false);
        m_hp3->setVisible(false);
        m_hp4->setVisible(true);
        m_hp5->setVisible(true);
        m_hp6->setVisible(false);

        m_hp1->setScaleX(1.0f);
        m_hp2->setScaleX(1.0f);
        m_hp3->setScaleX(1.0f);
        m_hp4->setScaleX(1.0f);
        m_hp5->setScaleX((hp*1.0f-this->getGaugeRange()*4) / this->getGaugeRange());
    }
    else if(hp <= this->getGaugeRange() * 6)
    {
        m_hp1->setVisible(false);
        m_hp2->setVisible(false);
        m_hp3->setVisible(false);
        m_hp4->setVisible(false);
        m_hp5->setVisible(true);
        m_hp6->setVisible(true);

        m_hp1->setScaleX(1.0f);
        m_hp2->setScaleX(1.0f);
        m_hp3->setScaleX(1.0f);
        m_hp4->setScaleX(1.0f);
        m_hp5->setScaleX(1.0f);
        m_hp6->setScaleX((hp*1.0f-this->getGaugeRange()*5) / this->getGaugeRange());
    }
}

int HPGauge::getHP()
{
    return m_hp;
}

int HPGauge::getGaugeRange()
{
    return m_gaugeRange;
}

HPGauge* HPGauge::create(int startHP, int gaugeRange, const char* hpBG_sfName, const char* hp1_sfName, const char* hp2_sfName, const char* hp3_sfName, const char* hp4_sfName, const char* hp5_sfName, const char* hp6_sfName, const char* hpFrame_sfName)
{
    HPGauge* instance = new HPGauge();
    if(instance && instance->initWithStartHP(startHP, gaugeRange, hpBG_sfName, hp1_sfName, hp2_sfName, hp3_sfName, hp4_sfName, hp5_sfName, hp6_sfName, hpFrame_sfName))
    {
        instance->autorelease();
        return instance;
    }
    else
    {
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}