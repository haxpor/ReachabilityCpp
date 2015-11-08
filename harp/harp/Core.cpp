//
//  Core.cpp
//  ZombieHero
//
//  Created by haxpor on 10/22/12.
//
//

#include "Core.h"
#include "CCFileUtils.h"
#include "WrapperLocalization.h"

using namespace cocos2d;

NS_S_HARPENGINE

// shared instance of HarpEngine's Core
static Core s_sharedCore;

Core::Core():
    m_deviceType(DEVICE_UNKNOWN)
{
    // do nothing here
}

Core::~Core()
{
    CCLOGINFO("deallocating Core ...");
}

void Core::init(void)
{
    // do any initialization code here
}

Core* Core::sharedCore(void)
{
    static bool s_isFirstUseCore = true;
    if (s_isFirstUseCore)
    {
        s_isFirstUseCore = false;
        s_sharedCore.init();
    }
    
    return &s_sharedCore;
}

CCPoint Core::convertPoint(CCPoint point)
{
    if(CCApplication::sharedApplication()->getTargetPlatform() == kTargetIphone)
        return ccp(point.x/2, point.y/2);
    else
        return point;
}

float Core::convertPointX(float x)
{
    if(CCApplication::sharedApplication()->getTargetPlatform() == kTargetIphone)
        return x/2.0f;
    else
        return x;
}

float Core::convertPointY(float y)
{
    if(CCApplication::sharedApplication()->getTargetPlatform() == kTargetIphone)
        return y/2.0f;
    else
        return y;
}

void Core::log(const char *strFormat, ...)
{
    if(IS_DEBUG)
    {
        va_list ap;
        va_start(ap, strFormat);
        CCLog(strFormat, ap);
        va_end(ap);
    }
}

void Core::setDeviceType(DeviceType type)
{
    if(type != m_deviceType)
        m_deviceType = type;
}

DeviceType Core::getDeviceType()
{
    return m_deviceType;
}

bool Core::getIsDeviceTypeiPhone()
{
    if(m_deviceType == iPhone3GS || m_deviceType == iPhone4 || m_deviceType == iPhone5 || m_deviceType == iPhone6 || m_deviceType == iPhone6Plus)
        return true;
    else
        return false;
}

bool Core::getIsDeviceTypeiPad()
{
    if(m_deviceType == iPadRetina || m_deviceType == iPad)
        return true;
    else
        return false;
}

void Core::loadSpriteFrames(const char *plistName)
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistName);
}

void Core::loadSpriteFrames(const char *plistName, const char *textureFileName)
{
    // this will in turn cache texture file as well
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistName, textureFileName);
}

void Core::loadLocalizedSpriteFrames(const char* plistName, const char* textureFileName)
{
    // check the current language being used
    const char* language = WrapperLocalization::sharedInstance()->getCurrentLanguageString();
    // get the current resource directory
    const char* resDirectory = CCFileUtils::sharedFileUtils()->getSearchPaths()[0].c_str();
    
    // form localized resource path
    char localizedPlistPath[2048];
    char localizedTextureFilePath[2048];
    snprintf(localizedPlistPath, sizeof(localizedPlistPath), "%s.lproj/%s%s", language, resDirectory, plistName);
    snprintf(localizedTextureFilePath, sizeof(localizedTextureFilePath), "%s.lproj/%s%s", language, resDirectory, textureFileName);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(localizedPlistPath, localizedTextureFilePath);
}

void Core::unloadSpriteFrames(const char* plistName)
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(plistName);
}

void Core::unloadSpriteFramesAndTexture(const char* plistName, const char* textureFileName)
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(plistName);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(textureFileName);
}

CCSpriteFrame* Core::getSpriteFrameByName(const char* frameName)
{
    return CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
}

void Core::addFrameToArrayFor(const char* frameName, cocos2d::CCArray *array, int times)
{
    // get frame from framename
    CCSpriteFrame *frame = getSpriteFrameByName(frameName);
    if(frame)
    {
        for(int i=1; i<=times; i++)
            array->addObject(frame);
    }
}

void Core::setScaleFactorToFitTargetSize(CCSprite* sprite, const CCSize& targetSize)
{
    float scaleX = targetSize.width / sprite->getContentSize().width;
    float scaleY = targetSize.height / sprite->getContentSize().height;
    
    if(scaleX < scaleY)
        sprite->setScale(scaleX);
    else
        sprite->setScale(scaleY);
}

float Core::getScaleFactorFittingTargetSize(const cocos2d::CCSize& size, const cocos2d::CCSize& targetSize)
{
    float scaleX = targetSize.width / size.width;
    float scaleY = targetSize.height / size.height;

    if(scaleX < scaleY)
        return scaleX;
    else
        return scaleY;
}

float Core::getScaleFactorFittingTargetSize(cocos2d::CCSprite* sprite, const cocos2d::CCSize& targetSize)
{
    float scaleX = targetSize.width / sprite->getContentSize().width;
    float scaleY = targetSize.height / sprite->getContentSize().height;

    if(scaleX < scaleY)
        return scaleX;
    else
        return scaleY;
}

void Core::setScaleFactorToFitTargetSizeIfPlatformIsIphone(CCSprite* sprite, const CCSize& targetSize)
{
    if(getIsDeviceTypeiPhone())
        setScaleFactorToFitTargetSize(sprite, targetSize);
}

float Core::calculateIncurredXPosFromScaledNode(CCSprite *node)
{
    if(getIsDeviceTypeiPhone())
        // we use getTextureRect() as the sprite may be a child in a texture atlas
        return (1.0f - node->getScale()) * node->getTextureRect().size.width / 8.0f;
    else
        return 0.0f;
}

float Core::calculateIncurredYPosFromScaledNode(CCSprite *node)
{
    if(getIsDeviceTypeiPhone())
        return (1.0f - node->getScale()) * node->getTextureRect().size.height / 8.0f;
    else
        return 0.0f;
}

NS_E_HARPENGINE   // ended of namespace harp
