//
//  Core.h
//  ZombieHero
//
//  Created by haxpor on 10/22/12.
//
//

#ifndef __ZombieHero__Core__
#define __ZombieHero__Core__

#include "cocos2d.h"
#include "CoreMacros.h"
#include "SimpleAudioEngine.h"

NS_S_HARPENGINE
    
class Core
{
public:
    Core();
    virtual ~Core();
    
    //-- Convert from iPad to iPhone coordinate. --//
    cocos2d::CCPoint convertPoint(cocos2d::CCPoint point);
    float convertPointX(float x);
    float convertPointY(float y);
    
    // -- Core utility -- //
    /* This function will help calculating x-pos incurred from scaled sprite so that positioning can be done correctly.
       Note: Only available on iPhone5. */
    float calculateIncurredXPosFromScaledNode(cocos2d::CCSprite *node);
    float calculateIncurredYPosFromScaledNode(cocos2d::CCSprite *node);
    
    // -- Log -- //
    void log(const char* str, ...);
    
    // -- Device type -- //
    void setDeviceType(DeviceType type);
    DeviceType getDeviceType();
    bool getIsDeviceTypeiPhone();
    bool getIsDeviceTypeiPad();
    
    // -- Scale factor -- //
    /* Set new scale factor for a specified sprite to fit the target size without distortion. */
    void setScaleFactorToFitTargetSize(cocos2d::CCSprite* sprite, const cocos2d::CCSize& targetSize);

    /*
        Get scale factor fitting the target size.
     */
    float getScaleFactorFittingTargetSize(const cocos2d::CCSize& size, const cocos2d::CCSize& targetSize);

    /*
        Get scale factor fitting the target sprite.
    */
    float getScaleFactorFittingTargetSize(cocos2d::CCSprite* sprite, const cocos2d::CCSize& targetSize);

    /* Set new scale factor for a specified sprite to fit the target size without distortion only if the platform is iPhone, otherwise nothing happen. */
    void setScaleFactorToFitTargetSizeIfPlatformIsIphone(cocos2d::CCSprite* sprite, const cocos2d::CCSize& targetSize);
    
    // -- Spritesheet -- //
    void loadSpriteFrames(const char* plistName);
    void loadSpriteFrames(const char* plistName, const char* textureFileName);
    void loadLocalizedSpriteFrames(const char* plistName, const char* textureFileName);
    void unloadSpriteFrames(const char* plistName);
    void unloadSpriteFramesAndTexture(const char* plistName, const char* textureFileName);
    cocos2d::CCSpriteFrame* getSpriteFrameByName(const char* frameName);
    void addFrameToArrayFor(const char* frameName, cocos2d::CCArray *array, int times);
    
    //-- Return the shared instance of Core class. --//
    static Core* sharedCore(void);
private:
    DeviceType m_deviceType;
    
    void init(void);
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__Core__) */
