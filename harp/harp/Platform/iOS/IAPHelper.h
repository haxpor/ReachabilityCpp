//
//  IAPHelper.h
//  ZombieHero
//
//  Created by haxpor on 9/4/14.
//
//

#ifndef __ZombieHero__IAPHelper__
#define __ZombieHero__IAPHelper__

#include <string>
#include "cocos2d.h"
#include "CoreMacros.h"
#include "IAPHelperDelegate.h"

#ifdef __OBJC__
@class IAPHelper_objc;
#endif

NS_S_HARPENGINE

typedef struct SKProduct_opPtr2_ *SKProduct_opPtr2;

#ifdef __OBJC__
typedef IAPHelper_objc *IAPHelper_objcPtr;
#else
typedef void* IAPHelper_objcPtr;
#endif

class IAPHelper
{
public:
    IAPHelper();
    virtual ~IAPHelper();
    
    bool initWithProductIdentifiers(std::vector<std::string> productIdentifiers);
    void requestProducts();
    void buyProduct(SKProduct_opPtr2 product);
    bool productPurchased(const char* productIdentifier);
    void restoreCompletedTransactions();
    void releaseCompletelyFor(WrapperCppSKProduct* product);
    void releaseCompletelyFor(WrapperCppSKProduct* array[], int size);
    
    void setDelegate(harp::IAPHelperDelegate* delegate);
    harp::IAPHelperDelegate* getDelegate();
    
private:
    IAPHelper_objcPtr _iapHelperObjc;
};

NS_E_HARPENGINE

#endif /* defined(__ZombieHero__IAPHelper__) */
