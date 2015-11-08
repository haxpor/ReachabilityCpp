/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCScene.h"
#include "CCLayer.h"
#include "support/CCPointExtension.h"
#include "CCDirector.h"

NS_CC_BEGIN

CCScene::CCScene():
    m_sceneState(-1)
{
    m_bIgnoreAnchorPointForPosition = true;
    setAnchorPoint(ccp(0.5f, 0.5f));
}

CCScene::~CCScene()
{
}

bool CCScene::init()
{
    bool bRet = false;
     do 
     {
         CCDirector * pDirector;
         CC_BREAK_IF( ! (pDirector = CCDirector::sharedDirector()) );
         this->setContentSize(pDirector->getWinSize());
         // success
         bRet = true;
     } while (0);
     return bRet;
}

CCScene *CCScene::create()
{
    CCScene *pRet = new CCScene();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

int CCScene::getSceneState()
{
    return m_sceneState;
}

void CCScene::setSceneState(int sceneState)
{
    if(m_sceneState != sceneState)
        m_sceneState = sceneState;
}

void CCScene::addLayerWithState(CCLayer* layer, int state, CCObject* ptrTargetSelector, SEL_CallFunc disablingCallFunc)
{
    // call callback function if need
    if(ptrTargetSelector != NULL && disablingCallFunc != NULL)
        (ptrTargetSelector->*disablingCallFunc)();
    
    // set state
    setSceneState(state);
    // add a specified layer to this scene
    addChild((CCNode*)(layer));
}
void CCScene::addLayerWithState(CCLayer* layer, int state, int zOrder, CCObject* ptrTargetSelector, SEL_CallFunc disablingCallFunc)
{
    // call callback function if need
    if(ptrTargetSelector != NULL && disablingCallFunc != NULL)
        (ptrTargetSelector->*disablingCallFunc)();
    
    // set state
    setSceneState(state);
    // add a specified layer to this scene
    addChild((CCNode*)(layer), zOrder);
}
void CCScene::addLayerWithState(CCLayer* layer, int state, int zOrder, int tag, CCObject* ptrTargetSelector, SEL_CallFunc disablingCallFunc)
{
    // call callback function if need
    if(ptrTargetSelector != NULL && disablingCallFunc != NULL)
        (ptrTargetSelector->*disablingCallFunc)();
    
    // set state
    setSceneState(state);
    // add a specified layer to this scene
    addChild((CCNode*)(layer), zOrder, tag);
}

void CCScene::addLayerWithState(cocos2d::CCLayer* layer, int state, int zOrder, int tag, cocos2d::CCObject* ptrTargetSelector, SEL_CallFuncD disablingCallFuncD, void* data)
{
    // call callback function if need
    if(ptrTargetSelector != NULL && disablingCallFuncD != NULL)
        (ptrTargetSelector->*disablingCallFuncD)(data);
    
    // set state
    setSceneState(state);
    // add a specified layer to this scene
    addChild((CCNode*)(layer), zOrder, tag);
}

void CCScene::removeLayerAndFallbackToState(CCLayer* layer, int fallbackState, cocos2d::CCObject* ptrTargetSelector, SEL_CallFunc enablingCallFunc)
{
    // remove a layer from this scene (also clear out the resource)
    layer->removeFromParentAndCleanup(true);
    
    // call callback function if need
    if(ptrTargetSelector != NULL && enablingCallFunc != NULL)
        (ptrTargetSelector->*enablingCallFunc)();
    // set state
    setSceneState(fallbackState);
}

void CCScene::removeLayerAndFallbackToState(cocos2d::CCLayer* layer, int fallbackState, cocos2d::CCObject* ptrTargetSelector, SEL_CallFuncD enablingCallFuncD, void* data)
{
    // remove a layer from this scene (also clear out the resource)
    layer->removeFromParentAndCleanup(true);
    
    // call callback function if need
    if(ptrTargetSelector != NULL && enablingCallFuncD != NULL)
        (ptrTargetSelector->*enablingCallFuncD)(data);
    // set state
    setSceneState(fallbackState);
}

NS_CC_END
