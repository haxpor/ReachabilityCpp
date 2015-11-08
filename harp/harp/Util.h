//
//  Util.h
//  ZombieHero
//
//  Created by haxpor on 12/4/13.
//
//

#ifndef __ZombieHero__Util__
#define __ZombieHero__Util__

#include "cocos2d.h"
#include "CoreMacros.h"

NS_S_HARPENGINE

class Util
{
public:
    Util();
    virtual ~Util();
    
    void init();
    
    // -- Return 1.0f if platform is iPad -- //
    float getSmallerScaleIfPlatformIsIphoneFrom(cocos2d::CCNode* node);

    /*
     * Get time representation string from total time.
     * It will return string in the format HH:MM:SS as string.
     */
    std::string getTimeStringFromTotalSeconds(double totalSeconds);
    
    // -- Return the shared instance of Util class --//
    static Util* sharedInstance();
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__Util__) */
