//
//  RunningLabelTTF.cpp
//  ZombieHero
//
//  Created by haxpor on 4/29/13.
//
//

#include "RunningLabelTTF.h"

using namespace cocos2d;

NS_S_HARPENGINE

int RunningLabelTTF::FULLTEXT_LENGTH = MEDIUM_CHARS_BUFFER;

RunningLabelTTF::RunningLabelTTF():
    m_fullText(NULL),
    m_delay(1.0f),
    m_countingSpeed(0.0f),
    m_charIndexOfFullText(0),
    m_isStarted(false),
    m_isPaused(false),
    m_isFinished(false),
    m_isSkipped(false),
    m_targetCallback(NULL),
    m_finishedCallback(NULL),
    m_paramCallbackData(NULL)
{
    
}

RunningLabelTTF::~RunningLabelTTF()
{
    CCLOGINFO("Destroying RunningLabelTTF.");
    
    if(!m_isFinished)
        unscheduleUpdate();
    
    CC_SAFE_DELETE(m_fullText);
}

bool RunningLabelTTF::initWithFullText(const char* fullText, const char* fontName, float fontSize, float delay)
{
    return initWithFullTextAndFinishedCallback(fullText, fontName, fontSize, delay, NULL, NULL, NULL);
}

bool RunningLabelTTF::initWithFullTextAndFinishedCallback(const char* fullText, const char* fontName, float fontSize, float delay, CCObject* targetCallback, SEL_CallFuncD finishedCallback, void* data)
{
    // TODO: Add more strict checking rules for "text" here ...
    // note: added empty space to fix wrong texture's cut out issue, so the whole character will be showed
    char inputStr[2];
    sprintf(inputStr, "%c ", std::string(fullText).c_str()[0]);
    
    if(!CCLabelTTF::initWithString(inputStr, fontName, fontSize))
    {
        return false;
    }
    
    m_fullText = new std::string(fullText);
    m_delay = delay;
    // save callback
    _setFinishedCallback(targetCallback, finishedCallback, data);
    
    scheduleUpdate();
    
    return true;
}

RunningLabelTTF* RunningLabelTTF::create(const char* fullText, const char* fontName, float fontSize, float delay)
{
    return createWithFinishedCallback(fullText, fontName, fontSize, delay, NULL, NULL, NULL);
}

RunningLabelTTF* RunningLabelTTF::createWithFinishedCallback(const char* fullText, const char* fontName, float fontSize, float delay, cocos2d::CCObject* targetCallback, cocos2d::SEL_CallFuncD finishedCallback, void* data)
{
    RunningLabelTTF* instance = new RunningLabelTTF();
    if(instance && instance->initWithFullTextAndFinishedCallback(fullText, fontName, fontSize, delay, targetCallback, finishedCallback, data))
    {
        instance->autorelease();
        return instance;
    }
    else
    {
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}

void RunningLabelTTF::update(float dt)
{
    if(!m_isSkipped)
    {
        if(m_isStarted && !m_isPaused && !m_isFinished)
        {
            m_countingSpeed += dt;
            
            if(m_countingSpeed > m_delay)
            {
                m_countingSpeed -= m_delay;
                
                // bring in next character to show
                m_charIndexOfFullText++;
                if(m_charIndexOfFullText < m_fullText->length())
                {
                    // update with next character
                    char str[FULLTEXT_LENGTH];
                    
                    // remove the previous empty space
                    std::string prevStr = getString();
                    std::string cutPrevStr = prevStr.substr(0, prevStr.length()-1);
                    
                    sprintf(str, "%s%c ", cutPrevStr.c_str(), m_fullText->c_str()[m_charIndexOfFullText]);
                    // update to labelttf
                    setString(str);
                }
                else
                {
                    // finish
                    m_isFinished = true;
                    // unschedule update
                    unscheduleUpdate();
                    
                    // call the finished callback
                    if(m_targetCallback != NULL && m_finishedCallback != NULL)
                        (m_targetCallback->*m_finishedCallback)(m_paramCallbackData);
                }
            }
        }
    }
    else
    {
        if(!m_isFinished)
        {
            // skip then add all characters to show
            setString(m_fullText->c_str());
            // set finish
            m_isFinished = true;
            // unschedule update
            unscheduleUpdate();
            
            // call the finished callback
            if(m_targetCallback != NULL && m_finishedCallback != NULL)
                (m_targetCallback->*m_finishedCallback)(m_paramCallbackData);
        }
    }
}

void RunningLabelTTF::setDelay(float delay)
{
    if(m_delay != delay)
        m_delay = delay;
}

float RunningLabelTTF::getDelay()
{
    return m_delay;
}

void RunningLabelTTF::start()
{
    m_isStarted = true;
}

void RunningLabelTTF::pause()
{
    m_isPaused = true;
}

void RunningLabelTTF::skip()
{
    m_isSkipped = true;
}

void RunningLabelTTF::resume()
{
    m_isPaused = false;
}

const char* RunningLabelTTF::getFullText()
{
    return m_fullText->c_str();
}

bool RunningLabelTTF::isPaused()
{
    return m_isPaused;
}

bool RunningLabelTTF::isFinished()
{
    return m_isFinished;
}

void RunningLabelTTF::_setFinishedCallback(CCObject *targetCallback, SEL_CallFuncD finishedCallback, void* data)
{
    // data can be null
    CCAssert((targetCallback == NULL && finishedCallback == NULL) ||
             (targetCallback != NULL && finishedCallback != NULL), "targetCallback and finishedCallback must be NULL or non-NULL together.");
    m_targetCallback = targetCallback;
    m_finishedCallback = finishedCallback;
    m_paramCallbackData = data;
}

NS_E_HARPENGINE