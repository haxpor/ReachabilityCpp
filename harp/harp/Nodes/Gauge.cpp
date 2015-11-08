//
//  Gauge.cpp
//  ZombieHero
//
//  Created by haxpor on 2/5/14.
//
//

#include "Gauge.h"

using namespace cocos2d;
using namespace harp;

Gauge::Gauge():
    m_gaugeBG(NULL),
    m_gaugeValue(NULL),
    m_gaugeFrame(NULL),
    m_isHorizontal(true)
{
    
}

Gauge::~Gauge()
{
    CCLOGINFO("Destroying Gauge");
}

bool Gauge::initWithValueAndTexturePaths(float value, float min, float max, const char *bgSpriteFrameName, const char *valueSpriteFrameName, const char *gaugeSpriteFrameName, bool isHorizontal)
{
    // ** external call prior to call this function needs to load a particular spritesheet holding all the required sprite textures used in this class.
    // ** this is for optimization in memory usage
    
    Core* core = Core::sharedCore();
    
    // ignore anchor point to add children
    ignoreAnchorPointForPosition(true);
    
    // set gauge's orientation
    m_isHorizontal = isHorizontal;
    // set min/max gauge value
    this->setMinGaugeValue(min);
    this->setMaxGaugeValue(max);
    
    if(isHorizontal)
    {
        m_gaugeBG = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(bgSpriteFrameName));
        m_gaugeBG->setAnchorPoint(ccp(0.0f, 0.5f));
        
        m_gaugeValue = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(valueSpriteFrameName));
        m_gaugeValue->setAnchorPoint(ccp(0.0f, 0.5f));
        
        m_gaugeFrame = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(gaugeSpriteFrameName));
        m_gaugeFrame->setAnchorPoint(ccp(0.0f, 0.5f));
    }
    else
    {
        m_gaugeBG = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(bgSpriteFrameName));
        m_gaugeBG->setAnchorPoint(ccp(0.5f, 0.0f));
        
        m_gaugeValue = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(valueSpriteFrameName));
        m_gaugeValue->setAnchorPoint(ccp(0.5f, 0.0f));
        
        m_gaugeFrame = CCSprite::createWithSpriteFrame(core->getSpriteFrameByName(gaugeSpriteFrameName));
        m_gaugeFrame->setAnchorPoint(ccp(0.5f, 0.0f));
    }
    
    this->addChild(m_gaugeBG);
    this->addChild(m_gaugeValue);
    this->addChild(m_gaugeFrame);
    
    // initially set the current value first
    this->setGaugeValue(value);
    
    return true;
}

Gauge* Gauge::create(float value, float min, float max, const char *bgSpriteFrameName, const char *valueSpriteFrameName, const char *gaugeSpriteFrameName, bool isHorizontal)
{
    Gauge *instance = new Gauge();
    if(instance->initWithValueAndTexturePaths(value, min, max, bgSpriteFrameName, valueSpriteFrameName, gaugeSpriteFrameName, isHorizontal))
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

float Gauge::getGaugeValue() { return m_currentGaugeValue; }
void Gauge::setGaugeValue(float value)
{
    // bound setting
    this->m_currentGaugeValue = MIN(MAX(value, getMinGaugeValue()), getMaxGaugeValue());
    
    // update to gauge sprite according to its
    if(this->getIsHorizontal())
    {
        m_gaugeValue->setScaleX( (this->m_currentGaugeValue - this->getMinGaugeValue()) / (this->getMaxGaugeValue() - this->getMinGaugeValue()));
    }
    else
    {
        m_gaugeValue->setScaleY( (this->m_currentGaugeValue - this->getMinGaugeValue()) / (this->getMaxGaugeValue() - this->getMinGaugeValue()));
    }
}

bool Gauge::getIsHorizontal() { return m_isHorizontal; }