//
//  ActivityIndicatorViewController.mm
//  ZombieHero
//
//  Created by haxpor on 9/5/14.
//
//

#include "ActivityIndicatorViewController.h"
#import <UIKit/UIKit.h>

using namespace harp;
using namespace cocos2d;

// shared Util instance
static ActivityIndicatorViewController s_sharedInstance;

ActivityIndicatorViewController::ActivityIndicatorViewController():
    _isHide_optimized(true)  // begin with hide
{
}

ActivityIndicatorViewController::~ActivityIndicatorViewController()
{
}

void ActivityIndicatorViewController::show()
{
    dispatch_async(dispatch_get_main_queue(), ^{
        // hide the activity indicator view
        [[[UIApplication sharedApplication].keyWindow.rootViewController.view viewWithTag:1] setHidden:NO];
        
        _isHide_optimized = false;
    });
}

void ActivityIndicatorViewController::hide()
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [[[UIApplication sharedApplication].keyWindow.rootViewController.view viewWithTag:1] setHidden:YES];
        
        _isHide_optimized = true;
    });
}

ActivityIndicatorViewController* ActivityIndicatorViewController::sharedInstance()
{
    static bool isFirstTimeCall = true;
    if(isFirstTimeCall)
    {
        // not first time anymore
        isFirstTimeCall = false;
        
        // do additional init here
        
        return &s_sharedInstance;
    }
    else
        return &s_sharedInstance;
}

bool ActivityIndicatorViewController::isHide()
{
    return _isHide_optimized;
}
