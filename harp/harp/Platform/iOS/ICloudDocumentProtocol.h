//
//  ICloudDocumentProtocol.h
//  ZombieHero
//
//  Created by haxpor on 1/28/14.
//
//

#ifndef ZombieHero_ICloudDocumentProtocol_h
#define ZombieHero_ICloudDocumentProtocol_h

#include "CoreMacros.h"
#include <string>

NS_S_HARPENGINE

class ICloudDocumentProtocol
{
public:
    virtual ~ICloudDocumentProtocol() {}
    
    virtual void cloudDocumentChanged(const char* newData) = 0;
    virtual void oniCloudCannotBeReached() = 0;
    
    virtual void onErrorWriting(const char* errorDescription) = 0;
    virtual void onSuccessWriting() = 0;
    virtual void onErrorReading(const char* errorDescription) = 0;
    virtual void onSuccessReading() = 0;
};

NS_E_HARPENGINE

#endif
