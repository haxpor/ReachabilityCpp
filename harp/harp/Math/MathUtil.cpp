//
//  MathUtil.cpp
//  ZombieHero
//
//  Created by haxpor on 2/4/13.
//
//

#include "MathUtil.h"
#include "Core.h"
#include <cstdlib>
#include <ctime>

using namespace cocos2d;

NS_S_HARPENGINE

// shard MathUtil instance
static harp::MathUtil s_sharedInstance;

MathUtil::MathUtil()
{
    // do nothing here
}

MathUtil::~MathUtil()
{
    CCLOGINFO("deallocating MathUtil ...");
}

MathUtil* MathUtil::sharedInstance()
{
    static bool s_isFirstUseInstance = true;
    if(s_isFirstUseInstance)
    {
        s_isFirstUseInstance = false;
        s_sharedInstance.init();
    }
    
    return &s_sharedInstance;
}

void MathUtil::init()
{
    // seed once
    srand( (unsigned int)time( 0 ) );
}

int MathUtil::randomInteger(int max)
{
    return rand() % max;
}

int MathUtil::randomIntegerBetween(int min, int max)
{
    return (rand() % (max-min)) + min;
}

float MathUtil::randomFloat(float max)
{
    return _randomUnityFloat() * max;
}

float MathUtil::randomFloatBetween(float min, float max)
{
    return min + (max - min) * _randomUnityFloat();
}

bool MathUtil::randomBoolean()
{
    if(_randomUnityFloat() > 0.5)
        return true;
    else
        return false;
}

float MathUtil::randomMinusOneOrOne()
{
    if(_randomUnityFloat() > 0.5)
        return 1.0;
    else
        return -1.0;
}

float MathUtil::lerp(float a, float b, float u)
{
    return (1 - u) * a + u * b;
}

CCPoint MathUtil::randomPointFrom(const CCRect& rect)
{
    // random x-pos
    float x = randomFloatBetween(rect.origin.x, rect.origin.x + rect.size.width);
    
    // random y-pos
    float y = randomFloatBetween(rect.origin.y, rect.origin.y + rect.size.height);
    
    return CCPointMake(x, y);
}

float MathUtil::randomDegreesInFullCircle()
{
    return randomFloatBetween(0.0f, 360.0f);
}

float MathUtil::randomClamped()
{
    return randomFloat() - randomFloat();
}

float MathUtil::_randomUnityFloat()
{
    return (float)rand()/(float)RAND_MAX;
}

NS_E_HARPENGINE   // ended of namespace Core