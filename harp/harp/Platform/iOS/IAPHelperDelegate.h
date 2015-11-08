//
//  IAPHelperDelegate.h
//  ZombieHero
//
//  Created by haxpor on 9/4/14.
//
//

#ifndef ZombieHero_IAPHelperDelegate_h
#define ZombieHero_IAPHelperDelegate_h

#include "CoreMacros.h"
#include <vector>
#include <string>

NS_S_HARPENGINE

#pragma mark - Wrapper SKProduct

// Only necessary info ripped from SKProduct
class WrapperCppSKProduct
{
public:
    std::string *productIdentifier;
    std::string *localizedTitle;
    std::string *localizedDescription;
    float price;
    std::string *localizedPrice;
    void* tag;  // caller responsible to delete this
    
    virtual ~WrapperCppSKProduct()
    {
        if(productIdentifier != NULL)
        {
            delete productIdentifier;
            productIdentifier = NULL;
        }
        if(localizedTitle != NULL)
        {
            delete localizedTitle;
            localizedTitle = NULL;
        }
        
        if(localizedDescription != NULL)
        {
            delete localizedDescription;
            localizedDescription = NULL;
        }
        
        if(localizedPrice != NULL)
        {
            delete localizedPrice;
            localizedPrice = NULL;
        }
    }
};

class IAPHelperDelegate
{
public:
    virtual ~IAPHelperDelegate() {}
    
    virtual void onRequestProductsCompleted(bool isSuccess, std::vector<harp::WrapperCppSKProduct*> *products) = 0;
    virtual void onCompleteTransaction(const char* productIdentifier) = 0;
    virtual void onRestoreTransaction(const char* productIdentifier) = 0;
    virtual void onFailedTransaction(const char* productIdentifier) = 0;
    virtual void onCancelledTransaction(const char* productIdentifier) = 0;
};

NS_E_HARPENGINE

#endif
