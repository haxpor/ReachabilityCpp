//
//  MathUtil.h
//  ZombieHero
//
//  Created by haxpor on 2/4/13.
//
//

#ifndef __ZombieHero__MathUtil__
#define __ZombieHero__MathUtil__

#include "cocos2d.h"
#include "CoreMacros.h"

NS_S_HARPENGINE

/**
 *  Singleton class for MathUtil.
 */
class MathUtil
{
public:
    MathUtil();
    virtual ~MathUtil();
    
    void init(void);
    
    // -- Random integer number within [0, max). --/
    int randomInteger(int max=RAND_MAX);
    // -- Random integer number within [min, max). --/
    int randomIntegerBetween(int min, int max);
    // -- Random floating point number within [0,max]. --/
    float randomFloat(float max=1.0f);
    // -- Random floating point number within [min, max]. --/
    float randomFloatBetween(float min, float max);
    // -- Random true or false. **/
    bool randomBoolean();
    // -- Random to get -1.0 or 1.0. --/
    float randomMinusOneOrOne();
    // -- Random degree from 0 - 360 --/
    float randomDegreesInFullCircle();
    // -- Lerp --/
    float lerp(float a, float b, float u);
    // -- Clamped, return -1 <= n <= 1 --/
    float randomClamped();
    
    // -- Random a point within a rectangle --/
    cocos2d::CCPoint randomPointFrom(const cocos2d::CCRect& rect);
    
    //-- Return the shared instance of MathUtil class. --//
    static MathUtil* sharedInstance(void);
    
private:
    // .. random between 0.0-1.0
    float _randomUnityFloat();
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__MathUtil__) */
