//
//  CCDeviceLocaleIOS.h
//  harp
//
//  Created by haxpor.
//
//

#include "CCDeviceLocale.h"

NS_CC_EXT_BEGIN;

const char* ccdeviceLocale_getLanguage()
{
    return [[[NSLocale preferredLanguages] objectAtIndex:0] UTF8String];
}

CC_DLL kCCDeviceLanguage ccdeviceLocale_getLanguageK()
{
    NSString *lang = [[NSLocale preferredLanguages] objectAtIndex:0];
    
    BOOL iosLessThan9 = [[[UIDevice currentDevice] systemVersion] compare:@"9.0" options:NSNumericSearch] == NSOrderedAscending;
    
    if(iosLessThan9)
    {
        if([lang isEqualToString:@"en"])
        {
            return ENGLISH;
        }
        else if([lang isEqualToString:@"fr"])
        {
            return FRENCH;
        }
        else if([lang isEqualToString:@"it"])
        {
            return ITALIAN;
        }
        else if([lang isEqualToString:@"de"])
        {
            return GERMAN;
        }
        else if([lang isEqualToString:@"es"])
        {
            return SPANISH;
        }
        else if([lang isEqualToString:@"ko"])
        {
            return KOREAN;
        }
        else if([lang isEqualToString:@"ja"])
        {
            return JAPANESE;
        }
        else if([lang isEqualToString:@"zh-Hans"])
        {
            return SIMPLIFIED_CHINESE;
        }
        else if([lang isEqualToString:@"zh-Hant"])
        {
            return TRADITIONAL_CHINESE;
        }
        else if([lang isEqualToString:@"ru"])
        {
            return RUSSIAN;
        }
        else if([lang isEqualToString:@"pt-BR"])
        {
            return BRAZILIAN_PORTUGUESE;
        }
        else
        {
            // other cases, return english
            return ENGLISH;
        }
    }
    else
    {
        // special case for simplified chinese, traditional chinese, and pt-BR
        if([lang rangeOfString:@"zh-Hans"].location != NSNotFound)
        {
            return SIMPLIFIED_CHINESE;
        }
        else if([lang rangeOfString:@"zh-Hant"].location != NSNotFound)
        {
            return TRADITIONAL_CHINESE;
        }
        else if([lang rangeOfString:@"pt-BR"].location != NSNotFound)
        {
            return BRAZILIAN_PORTUGUESE;
        }
        // other language codes
        else
        {
            if([lang rangeOfString:@"en-"].location != NSNotFound)
            {
                return ENGLISH;
            }
            else if([lang rangeOfString:@"fr-"].location != NSNotFound)
            {
                return FRENCH;
            }
            else if([lang rangeOfString:@"it-"].location != NSNotFound)
            {
                return ITALIAN;
            }
            else if([lang rangeOfString:@"de-"].location != NSNotFound)
            {
                return GERMAN;
            }
            else if([lang rangeOfString:@"es-"].location != NSNotFound)
            {
                return SPANISH;
            }
            else if([lang rangeOfString:@"ko-"].location != NSNotFound)
            {
                return KOREAN;
            }
            else if([lang rangeOfString:@"ja-"].location != NSNotFound)
            {
                return JAPANESE;
            }
            else if([lang rangeOfString:@"ru-"].location != NSNotFound)
            {
                return RUSSIAN;
            }
            else
            {
                // other cases, return english
                return ENGLISH;
            }
        }
    }
}

NS_CC_EXT_END;