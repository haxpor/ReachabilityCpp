//
//  DummyScene.hpp
//  HelloCpp
//
//  Created by haxpor on 11/5/15.
//
//

#ifndef DummyScene_hpp
#define DummyScene_hpp

#include "cocos2d.h"

class DummyScene : public cocos2d::CCLayer
{
public:
    virtual ~DummyScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(DummyScene);
};

#endif /* DummyScene_hpp */
