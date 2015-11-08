//
//  ReachabilityCpp.h
//  ReachabilityCpp
//
//  Created by Wasin Thonkaew on 11/8/15.
//
//

#ifndef ReachabilityCpp_h
#define ReachabilityCpp_h

#include "cocos2d.h"

#ifdef __OBJC__
@class Reachability;
typedef Reachability *ReachabilityPtr;
#else
typedef void* ReachabilityPtr;
#endif

/**
 Typedef the mirror NetworkStatus as defined in Reachability header.
 */
typedef enum : int {
    ErrorCpp = -1,
    NotReachableCpp,
    ReachableViaWiFiCpp,
    ReachableViaWWANCpp
} NetworkStatusCpp;

class ReachabilityCpp : public cocos2d::CCObject
{
private:
    ReachabilityCpp();
public:
    virtual ~ReachabilityCpp();
    
    virtual bool init();
    virtual bool initWithHostName(const char* hostname);
    virtual bool initForInternetConnection();
    virtual bool initForLocalWiFi();
    
    bool startNotifier();
    void stopNotifier();
    bool connectionRequired();
    NetworkStatusCpp currentReachabilityStatus();
    
    static ReachabilityCpp* createReachabilityForInternetConnection();
    static ReachabilityCpp* createReachabilityForLocalWiFi();
    static ReachabilityCpp* createReachabilityWithHostName(const char* hostname);
    
private:
    ReachabilityPtr _reachabilityObjc;
};


#endif /* ReachabilityCpp_h */
