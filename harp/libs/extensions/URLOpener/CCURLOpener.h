//
//  CCURLOpener.h
//  HelloCpp
//
//  Created by haxpor on 11/3/15.
//
//

#ifndef CCURLOpener_h
#define CCURLOpener_h

#include "cocos2d.h"
#include "cocos-ext.h"

NS_CC_EXT_BEGIN

class CCURLOpener : public CCNode
{
public:
    CCURLOpener();
    virtual ~CCURLOpener();
    
    virtual bool init() override;
    
    /**
     Try to open the specified URL.
     If success then return true, otherwise return false.
     */
    bool openURL(const char* url);
    
    static CCURLOpener* create();
};

NS_CC_EXT_END

#endif /* CCURLOpener_hpp */
