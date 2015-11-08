//
//  CloudDocument.mm
//  ZombieHero
//
//  Created by haxpor on 1/28/14.
//
//

#include "CloudDocument.h"
#include "CloudDocument_objc.h"
#include "CCFileUtils.h"

using namespace harp;

CloudDocument::CloudDocument()
{
    
}

CloudDocument::~CloudDocument()
{
}

bool CloudDocument::initWithFileURL(std::string url, harp::ICloudDocumentProtocol* delegate, std::string fileName)
{
    // initialize a managed cloud document
    _cloudDocumentObjc = [CloudDocument_objc create:[NSURL fileURLWithPath:[NSString stringWithUTF8String:url.c_str()]]  delegate:delegate fileName:[NSString stringWithUTF8String:fileName.c_str()]];
    
    return true;
}

bool CloudDocument::initWithFileName(harp::ICloudDocumentProtocol *delegate, std::string fileName)
{
    _cloudDocumentObjc = [CloudDocument_objc create:delegate fileName:[NSString stringWithUTF8String:fileName.c_str()]];
    
    return true;
}

void CloudDocument::read()
{
    [_cloudDocumentObjc read];
}

void CloudDocument::write()
{
    [_cloudDocumentObjc write];
}

CloudDocument* CloudDocument::create(std::string url, harp::ICloudDocumentProtocol* delegate, std::string fileName)
{
    CloudDocument *instance = new CloudDocument();
    if(instance->initWithFileURL(url, delegate, fileName))
    {
        instance->autorelease();
        return instance;
    }
    else
    {
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}

CloudDocument* CloudDocument::create(harp::ICloudDocumentProtocol *delegate, std::string fileName)
{
    CloudDocument *instance = new CloudDocument();
    if(instance->initWithFileName(delegate, fileName))
    {
        instance->autorelease();
        return instance;
    }
    else
    {
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}

const char* CloudDocument::getDocumentText()
{
    return [_cloudDocumentObjc.documentText UTF8String];
}
void CloudDocument::setDocumentText(const char* var)
{
    _cloudDocumentObjc.documentText = [NSString stringWithUTF8String:var];
}

const char* CloudDocument::getDocumentFileName()
{
    return [_cloudDocumentObjc.documentFileName UTF8String];
}

const char* CloudDocument::getURL()
{
    return [[_cloudDocumentObjc.fileURL path] UTF8String];
}

bool CloudDocument::exist()
{
    if([_cloudDocumentObjc exist])
        return true;
    else
        return false;
}

bool CloudDocument::isiCloudAvailable()
{
    return [CloudDocument_objc isiCloudAvailable];
}