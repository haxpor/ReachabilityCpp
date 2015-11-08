//
//  ActivityIndicatorViewController.h
//  ZombieHero
//
//  Created by haxpor on 9/5/14.
//
//

#ifndef __ZombieHero__ActivityIndicatorViewController__
#define __ZombieHero__ActivityIndicatorViewController__

#include <stdio.h>
#include "cocos2d.h"
#include "CoreMacros.h"

#ifdef __OBJC__
@class UIView;
@class UIViewController;
#endif

NS_S_HARPENGINE

#ifdef __OBJC__
typedef UIView *UIView_objcPtr;
typedef UIViewController *UIViewController_objcPtr;
#else
typedef void* UIView_objcPtr;
typedef void* UIViewController_objcPtr;
#endif

class ActivityIndicatorViewController : cocos2d::CCObject
{
public:
    ActivityIndicatorViewController();
    virtual ~ActivityIndicatorViewController();
    
    void show();
    void hide();
    bool isHide();
    
    static ActivityIndicatorViewController* sharedInstance();
    
private:
    bool _isHide_optimized;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__ActivityIndicatorViewController__) */
