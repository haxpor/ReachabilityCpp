//
//  BufferScene.h
//  ZombieHero
//
//  Created by haxpor on 4/22/13.
//
//

#ifndef __ZombieHero__BufferScene__
#define __ZombieHero__BufferScene__

#include "cocos2d.h"
#include "CoreMacros.h"

NS_S_HARPENGINE

/* 
    Abstract class
    BufferScene will be used when a need to show the loading screen in between before the target scene get loading.
 
    It's CCLayer not CCScene as this is an actual scene maintaining a layer. 
 
    Derived class needs to implement the following functions
    - initStaticVisual()
    - initTargetScene()
 */
class BufferScene : public cocos2d::CCLayer
{
public:
    static const float DELAY_LOADING_TIME;
    static const int NUM_FRAME_DRAWED;
public:
    BufferScene();
    virtual ~BufferScene();
    virtual void dealloc();
    
    bool initWithTargetSceneState(cocos2d::CCNode* parentNode, int targetSceneState, bool isApplyWithTransitionScene, bool isCountDownTime=false, float delayDuration=-1.0f);
    
    /*  Implement initializing function for this layer defining BufferScene's UI. Return true when everything is okay, otherwise return false.
        
        You should not initialize a target layer (wrapped up by scene) in this function as it will make a sudden slow down to the game.
     
        Tip: This function is meant to be initialized with static / fixed frame with not animation. This will give the best performance and benefit to initialize a target scene via initTargetScene().
     */
    virtual bool initStaticVisual(cocos2d::CCNode* parentScene, int targetSceneState) = 0;
    
    /*  Implement initializing function for a target layer (wrapped by scene). 
        
        This function will be called when a first frame of BufferScene is drawed to the screen.
    */
    virtual bool initTargetScene(int targetSceneState) = 0;
    
    /* Implement this function to replace the current running scene with the initialized target scene handling by derived class. */
    virtual void replaceScene() = 0;
    
    virtual void onEnterTransitionDidFinish();
    virtual void onEnter();
    virtual void onExit();
    virtual void visit(void);
    virtual void update(float dt);
    
    int getTargetSceneState();
    bool getIsAppliedWithTransitionScene();
    bool getIsCountDownTime();
    float getDelayDuration();

    // override this method to define user defined function to call when time is up
    virtual void onTimeup();

protected:
    void initalizeTargetScene();

protected:
    int m_targetSceneState;
    float m_delayDurationCountingTime;
    int m_numberOfFrameDrawed;
    bool m_isTargetSceneInitialized;
    bool m_isAppliedWithTransitionScene;
    bool m_isTransitionFisnishedInPreviousFrame;
    bool m_isReadyToReplaceScene;

    bool m_isCountDownTime;
    float m_delayDuration;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__BufferScene__) */
