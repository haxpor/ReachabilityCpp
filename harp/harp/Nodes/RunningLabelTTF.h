//
//  RunningLabelTTF.h
//  ZombieHero
//
//  Created by haxpor on 4/29/13.
//
//

#ifndef __ZombieHero__RunningLabelTTF__
#define __ZombieHero__RunningLabelTTF__

#include "cocos2d.h"
#include "CoreMacros.h"

NS_S_HARPENGINE

class RunningLabelTTF : public cocos2d::CCLabelTTF
{
public:
    /* Change this to match your fulltext's length usage. */
    static int FULLTEXT_LENGTH;
public:
    RunningLabelTTF();
    virtual ~RunningLabelTTF();
    
    bool initWithFullText(const char* fullText, const char* fontName, float fontSize, float delay);
    bool initWithFullTextAndFinishedCallback(const char* fullText, const char* fontName, float fontSize, float delay, cocos2d::CCObject* targetCallback, cocos2d::SEL_CallFuncD finishedCallback, void* data);
    virtual void update(float dt);
    
    /* Set delay of new character to be popped up and shows on screen. Every "s" passed, a new character will be showed. Measure in seconds. */
    void setDelay(float s);
    float getDelay();
    bool isPaused();
    bool isFinished();
    
    void start();
    void pause();
    void resume();
    void skip();
    const char* getFullText();
    
    static RunningLabelTTF* create(const char* fullText, const char* fontName, float fontSize, float delay);
    static RunningLabelTTF* createWithFinishedCallback(const char* fullText, const char* fontName, float fontSize, float delay, cocos2d::CCObject* targetCallback, cocos2d::SEL_CallFuncD finishedCallback, void* data);
    
protected:
    void _setFinishedCallback(cocos2d::CCObject *targetCallback, cocos2d::SEL_CallFuncD finishedCallback, void* data);
    
protected:
    std::string *m_fullText;
    float m_delay;
    float m_countingSpeed;
    int m_charIndexOfFullText;
    bool m_isStarted;
    bool m_isPaused;
    bool m_isFinished;
    bool m_isSkipped;
    cocos2d::CCObject* m_targetCallback;
    cocos2d::SEL_CallFuncD m_finishedCallback;
    void* m_paramCallbackData;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__RunningLabelTTF__) */
