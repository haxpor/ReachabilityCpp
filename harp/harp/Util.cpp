//
//  Util.cpp
//  ZombieHero
//
//  Created by haxpor on 12/4/13.
//
//

#include <iomanip>
#include "Util.h"
#include "Core.h"

using namespace cocos2d;
using namespace harp;

// shared Util instance
static harp::Util s_sharedUtilInstance;

Util::Util()
{
    
}

Util::~Util()
{
    CCLOGINFO("Destroying Util ...");
}

void Util::init()
{
    // has nothing to do here
}

Util* Util::sharedInstance()
{
    static bool s_isFirstUseInstance = true;
    if(s_isFirstUseInstance)
    {
        s_isFirstUseInstance = false;
        s_sharedUtilInstance.init();
    }
    
    return &s_sharedUtilInstance;
}

float Util::getSmallerScaleIfPlatformIsIphoneFrom(cocos2d::CCNode* node)
{
    if(Core::sharedCore()->getIsDeviceTypeiPhone())
    {
        if(node->getScaleX() < node->getScaleY())
            return node->getScaleX();
        else
            return node->getScaleY();
    }
    else
        return 1.0f;
}

std::string Util::getTimeStringFromTotalSeconds(double totalSeconds) {
    if(totalSeconds < 0)
        return std::string("00:00:00");
    else
    {
        // remaining from each step of calculation
        double remaining;

        // hour
        int hours = floor(totalSeconds / 3600.0f);
        remaining = (long long) totalSeconds % 3600;

        // minutes
        int minutes = floor(remaining / 60.0f);
        // seconds (remaining becomes seconds)
        remaining = (long long) remaining % 60;

        // combine all string together
        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << remaining;

        return std::string(ss.str());
    }
}
