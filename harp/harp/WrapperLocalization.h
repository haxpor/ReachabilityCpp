//
//  WrapperLocalization.h
//  ZombieHero
//
//  Created by haxpor on 11/26/13.
//
//

#ifndef __ZombieHero__WrapperLocalization__
#define __ZombieHero__WrapperLocalization__

#include "CoreMacros.h"

NS_S_HARPENGINE

#define HarpLocalizedString(key, comment) \
    WrapperLocalization::sharedInstance()->getLocalizedStringForKey(key, comment)

#define LANGUAGE_TOTAL 11
enum Language {
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

class WrapperLocalization
{
public:
    WrapperLocalization();
    ~WrapperLocalization();
    
    void setupPreferredLanguages();
    const char* getLocalizedStringForKey(const char* keyStr, const char* comment);
    Language getCurrentLanguage();
    const char* getCurrentLanguageString();
    void changeLanguageTo(Language language);
    
    static WrapperLocalization* sharedInstance();
    
protected:
    Language m_selectedLanguage;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__WrapperLocalization__) */
