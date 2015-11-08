//
//  BufferScene.cpp
//  ZombieHero
//
//  Created by haxpor on 4/22/13.
//
//

#include "BufferScene.h"
#include "CCNotificationCenter.h"

using namespace cocos2d;
using namespace harp;

const float BufferScene::DELAY_LOADING_TIME = 1.0f; // in sec
const int BufferScene::NUM_FRAME_DRAWED = 2;  // a threshold to begin initialize a target scene

BufferScene::BufferScene():
    m_targetSceneState(-1),
    m_delayDurationCountingTime(0.0f),
    m_numberOfFrameDrawed(0),
    m_isTargetSceneInitialized(false),
    m_isTransitionFisnishedInPreviousFrame(false),
    m_isReadyToReplaceScene(false),
    m_delayDuration(180.0f),
    m_isCountDownTime(false)
{
    
}

BufferScene::~BufferScene()
{
    CCLOGINFO("deallocating BufferScene");
}

void BufferScene::dealloc()
{
    this->unscheduleUpdate();
}

// on "init" you need to initialize your instance
bool BufferScene::initWithTargetSceneState(CCNode* parentScene, int targetSceneState, bool isApplyWithTransitionScene, bool isCountDownTime, float delayDuration)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    // save data
    m_targetSceneState = targetSceneState;
    m_isAppliedWithTransitionScene = isApplyWithTransitionScene;
    m_isCountDownTime = isCountDownTime;
    m_delayDuration = delayDuration;

    this->scheduleUpdate();
    
    // call user-defined function
    return initStaticVisual(parentScene, targetSceneState);
}

int BufferScene::getTargetSceneState()
{
    return m_targetSceneState;
}

bool BufferScene::getIsAppliedWithTransitionScene()
{
    return m_isAppliedWithTransitionScene;
}

void BufferScene::onEnter()
{
    CCLayer::onEnter();
    
    // set the scene state when "stage" enter this layer
    static_cast<CCScene*>(getParent())->setSceneState(BUFFERSCENE_BUFFERLAYER_STATE);
}

void BufferScene::onExit()
{
    dealloc();
    
    // as the memory will be in cached which won't contribute much, then we need not to purge cache or anything
    CCLayer::onExit();
}

void BufferScene::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
    
    CCDirector::sharedDirector()->purgeCachedData();
    
    if(m_isAppliedWithTransitionScene)
    {
        // schedule next frame to initialize a target scene & replace scene
        // note: we need to wait for one more scene to let the previous potential same scene to be deleted, and allow for clean transition
        // note 2: we just lost 1 frame showing off loading buffer scene. I can live with that :-/
        if(!m_isTransitionFisnishedInPreviousFrame)
            m_isTransitionFisnishedInPreviousFrame = true;
    }
}

void BufferScene::initalizeTargetScene()
{
    if(!m_isTargetSceneInitialized)
    {
        // initialize a target scene, then replace with it immediately
        bool result = initTargetScene(m_targetSceneState);
        CCAssert(result == true, "Initalize of target scene should always be successful.");
    
        m_isTargetSceneInitialized = true;
    }
}

void BufferScene::update(float dt)
{
    // update counting down time if needed
    if(m_isCountDownTime)
    {
        m_delayDurationCountingTime += dt;
    }
}

void BufferScene::visit(void)
{
    // just usual
    CCLayer::visit();
    
    // if not apply with transition scene then use immediate frame count to transition instead
    if(!m_isAppliedWithTransitionScene)
    {
        if(m_numberOfFrameDrawed < NUM_FRAME_DRAWED)
            m_numberOfFrameDrawed++;
        
        // check if the first frame drawed
        if(m_numberOfFrameDrawed >= NUM_FRAME_DRAWED && !m_isTargetSceneInitialized)
        {
            if(m_isCountDownTime)
            {
                if(m_delayDurationCountingTime >= m_delayDuration)
                {
                    this->onTimeup();
                    
                    initalizeTargetScene();
                    replaceScene();
                }
            }
            else
            {
                initalizeTargetScene();
                replaceScene();
            }
        }
    }
    else
    {
        if(m_isTransitionFisnishedInPreviousFrame)
        {
            if(m_isCountDownTime)
            {
                if(m_delayDurationCountingTime >= m_delayDuration)
                {
                    this->onTimeup();
                    
                    initalizeTargetScene();
                    replaceScene();

                    // never call this again
                    m_isTransitionFisnishedInPreviousFrame = false;
                }
            }
            else
            {
                initalizeTargetScene();
                replaceScene();

                // never call this again
                m_isTransitionFisnishedInPreviousFrame = false;
            }
        }
    }
}

bool BufferScene::getIsCountDownTime()
{
    return m_isCountDownTime;
}

float BufferScene::getDelayDuration()
{
    return m_delayDuration;
}

void BufferScene::onTimeup()
{
    // do nothing here, derived class should be adding detail here ...
}