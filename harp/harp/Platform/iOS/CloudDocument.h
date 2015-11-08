//
//  CloudDocument.h
//  ZombieHero
//
//  Created by haxpor on 1/28/14.
//
//

#ifndef __ZombieHero__CloudDocument__
#define __ZombieHero__CloudDocument__

#include <string>
#include "cocos2d.h"
#include "CoreMacros.h"
#include "ICloudDocumentProtocol.h"

#ifdef __OBJC__
@class CloudDocument_objc;
#endif

NS_S_HARPENGINE

#ifdef __OBJC__
typedef CloudDocument_objc *CloudDocument_objcPtr;
#else
typedef void* CloudDocument_objcPtr;
#endif

class CloudDocument : public cocos2d::CCObject
{
public:
    CloudDocument();
    virtual ~CloudDocument();
    
    bool initWithFileURL(std::string url, harp::ICloudDocumentProtocol* delegate, std::string fileName);
    bool initWithFileName(harp::ICloudDocumentProtocol* delegate, std::string fileName);
    void read();
    void write();
    bool exist();
    
    static CloudDocument* create(std::string url, harp::ICloudDocumentProtocol* delegate, std::string fileName);
    static CloudDocument* create(harp::ICloudDocumentProtocol* delegate, std::string fileName);
    
    /**
     @brief Allow others to check if iCloud is available before actually start.
     @return True if it's available, otherwise false.
     */
    static bool isiCloudAvailable();
    
    /** Document Text Property **/
    const char* getDocumentText();
    void setDocumentText(const char* var);
    
    /** Document Filename Property **/
    const char* getDocumentFileName();
    
    /** URL **/
    const char* getURL();
    
private:
    
    CloudDocument_objcPtr _cloudDocumentObjc;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__CloudDocument__) */
