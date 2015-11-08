//
//  CCDeviceLocale.h
//  harp
//
//  Created by haxpor.
//
//

#ifndef __CCDeviceLocale_H__
#define __CCDeviceLocale_H__

#include "cocos2d.h"
#include "cocos-ext.h"

NS_CC_EXT_BEGIN

enum kCCDeviceLanguage
{
    ENGLISH,
    FRENCH,
    ITALIAN,
    GERMAN,
    SPANISH,
    KOREAN,
    JAPANESE,
    SIMPLIFIED_CHINESE,
    TRADITIONAL_CHINESE,
    RUSSIAN,
    BRAZILIAN_PORTUGUESE
};

/**
 Get raw language string of device's language setting.
 */
CC_DLL const char* ccdeviceLocale_getLanguage();

/**
 Get language of device's langauge setting in constant of kCCDeviceLanguage.
 */
CC_DLL kCCDeviceLanguage ccdeviceLocale_getLanguageK();

NS_CC_EXT_END

#endif