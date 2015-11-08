//
//  CCURLOpener.mm
//  HelloCpp
//
//  Created by haxpor on 11/3/15.
//
//

#include "CCURLOpener.h"

NS_CC_EXT_BEGIN;

CCURLOpener::CCURLOpener()
{
    
}

CCURLOpener::~CCURLOpener()
{
    CCLOGINFO("Destroying CCURLOpener");
}

bool CCURLOpener::init()
{
    if(CCNode::init())
    {
        return true;
    }
    return false;
}

CCURLOpener* CCURLOpener::create()
{
    CCURLOpener* pRet = new CCURLOpener();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool CCURLOpener::openURL(const char *url)
{
    NSURL *nsUrl = [NSURL URLWithString:[NSString stringWithUTF8String:url]];
    if([[UIApplication sharedApplication] canOpenURL:nsUrl])
    {
        [[UIApplication sharedApplication] openURL:nsUrl];
        return true;
    }
    return false;
}

NS_CC_EXT_END;