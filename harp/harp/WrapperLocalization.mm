//
//  WrapperLocalization.mm
//  ZombieHero
//
//  Created by haxpor on 11/26/13.
//
//

#include "WrapperLocalization.h"

NS_S_HARPENGINE

static WrapperLocalization *s_wrapperLocalization;

WrapperLocalization::WrapperLocalization():
    m_selectedLanguage(ENGLISH)
{
    
}

WrapperLocalization::~WrapperLocalization()
{
    
}

WrapperLocalization* WrapperLocalization::sharedInstance()
{
    if(!s_wrapperLocalization)
    {
        s_wrapperLocalization = new WrapperLocalization();
    }
    
    return s_wrapperLocalization;
}

const char* WrapperLocalization::getLocalizedStringForKey(const char* keyStr, const char* comment)
{
    NSString *path;
    if(m_selectedLanguage == ENGLISH)
        path = [[NSBundle mainBundle] pathForResource:@"en" ofType:@"lproj"];
    else if(m_selectedLanguage == FRENCH)
        path = [[NSBundle mainBundle] pathForResource:@"fr" ofType:@"lproj"];
    else if(m_selectedLanguage == ITALIAN)
        path = [[NSBundle mainBundle] pathForResource:@"it" ofType:@"lproj"];
    else if(m_selectedLanguage == GERMAN)
        path = [[NSBundle mainBundle] pathForResource:@"de" ofType:@"lproj"];
    else if(m_selectedLanguage == SPANISH)
        path = [[NSBundle mainBundle] pathForResource:@"es" ofType:@"lproj"];
    else if(m_selectedLanguage == KOREAN)
        path = [[NSBundle mainBundle] pathForResource:@"ko" ofType:@"lproj"];
    else if(m_selectedLanguage == JAPANESE)
        path = [[NSBundle mainBundle] pathForResource:@"ja" ofType:@"lproj"];
    else if(m_selectedLanguage == SIMPLIFIED_CHINESE)
        path = [[NSBundle mainBundle] pathForResource:@"zh-Hans" ofType:@"lproj"];
    else if(m_selectedLanguage == TRADITIONAL_CHINESE)
        path = [[NSBundle mainBundle] pathForResource:@"zh-Hant" ofType:@"lproj"];
    else if(m_selectedLanguage == RUSSIAN)
        path = [[NSBundle mainBundle] pathForResource:@"ru" ofType:@"lproj"];
    else if(m_selectedLanguage == BRAZILIAN_PORTUGUESE)
        path = [[NSBundle mainBundle] pathForResource:@"pt-BR" ofType:@"lproj"];
    
    else
        // fallback case
        path = [[NSBundle mainBundle] pathForResource:@"en" ofType:@"lproj"];
    
    NSBundle* languageBundle = [NSBundle bundleWithPath:path];
    NSString* str = [languageBundle localizedStringForKey:[NSString stringWithUTF8String:keyStr] value:@"" table:nil];
    return [str UTF8String];
}

Language WrapperLocalization::getCurrentLanguage()
{
    return m_selectedLanguage;
}

const char* WrapperLocalization::getCurrentLanguageString()
{
    if(m_selectedLanguage == ENGLISH)
        return "en";
    else if(m_selectedLanguage == FRENCH)
        return "fr";
    else if(m_selectedLanguage == ITALIAN)
        return "it";
    else if(m_selectedLanguage == GERMAN)
        return "de";
    else if(m_selectedLanguage == SPANISH)
        return "es";
    else if(m_selectedLanguage == KOREAN)
        return "ko";
    else if(m_selectedLanguage == JAPANESE)
        return "ja";
    else if(m_selectedLanguage == SIMPLIFIED_CHINESE)
        return "zh-Hans";
    else if(m_selectedLanguage == TRADITIONAL_CHINESE)
        return "zh-Hant";
    else if(m_selectedLanguage == RUSSIAN)
        return "ru";
    else if(m_selectedLanguage == BRAZILIAN_PORTUGUESE)
        return "pt-BR";
    else
        // should not happen, but use this as fall back case
        return "en";
}

void WrapperLocalization::changeLanguageTo(Language language)
{
    m_selectedLanguage = language;
}

NS_E_HARPENGINE
