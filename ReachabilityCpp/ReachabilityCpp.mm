//
//  ReachabilityCpp.mm
//  ReachabilityCpp
//
//  Created by haxpor on 11/8/15.
//
//

#import "ReachabilityCpp.h"
#import "Reachability.h"

using namespace cocos2d;

ReachabilityCpp::ReachabilityCpp():
    _reachabilityObjc(NULL)
{
    
}

ReachabilityCpp::~ReachabilityCpp()
{
    CCLOGINFO("Destroying ReachabilityCpp");
}

bool ReachabilityCpp::init()
{
    return true;
}

bool ReachabilityCpp::initWithHostName(const char* hostname)
{
    _reachabilityObjc = [Reachability reachabilityWithHostName:[NSString stringWithUTF8String:hostname]];
    
    return true;
}

bool ReachabilityCpp::initForInternetConnection()
{
    _reachabilityObjc = [Reachability reachabilityForInternetConnection];
    
    return true;
}

bool ReachabilityCpp::initForLocalWiFi()
{
    _reachabilityObjc = [Reachability reachabilityForLocalWiFi];
    
    return true;
}

ReachabilityCpp* ReachabilityCpp::createReachabilityForInternetConnection()
{
    ReachabilityCpp *instance = new ReachabilityCpp();
    if(instance && instance->initForInternetConnection())
    {
        instance->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(instance);
    }
    
    return instance;
}

ReachabilityCpp* ReachabilityCpp::createReachabilityForLocalWiFi()
{
    ReachabilityCpp *instance = new ReachabilityCpp();
    if(instance && instance->initForLocalWiFi())
    {
        instance->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(instance);
    }
    
    return instance;
}

ReachabilityCpp* ReachabilityCpp::createReachabilityWithHostName(const char *hostname)
{
    ReachabilityCpp *instance = new ReachabilityCpp();
    if(instance && instance->initWithHostName(hostname))
    {
        instance->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(instance);
    }
    
    return instance;
}

bool ReachabilityCpp::startNotifier()
{
    if (_reachabilityObjc)
        return [_reachabilityObjc startNotifier] ? true : false;
    return false;
}

void ReachabilityCpp::stopNotifier()
{
    if (_reachabilityObjc)
        [_reachabilityObjc stopNotifier];
}

bool ReachabilityCpp::connectionRequired()
{
    if (_reachabilityObjc)
        return [_reachabilityObjc connectionRequired] ? true : false;
    return false;
}

NetworkStatusCpp ReachabilityCpp::currentReachabilityStatus()
{
    if (_reachabilityObjc)
    {
        NetworkStatus networkStatus = [_reachabilityObjc currentReachabilityStatus];
        switch (networkStatus) {
            case NotReachable:
                return NotReachableCpp;
            
            case ReachableViaWiFi:
                return ReachableViaWiFiCpp;
                
            case ReachableViaWWAN:
                return ReachableViaWWANCpp;
                
            default:
                break;
        }
    }
    
    return ErrorCpp;
}