//
//  IAPHelper.mm
//  ZombieHero
//
//  Created by haxpor on 9/4/14.
//
//

#include "IAPHelper.h"
#include "IAPHelper_objc.h"
#import <StoreKit/StoreKit.h>

using namespace harp;

typedef struct SKProduct_opPtr2_
{
    SKProduct *product;
    
    SKProduct_opPtr2_()
    {
        product = NULL;
    }
    
} SKProduct_opPtr2_, *SKProduct_opPtr2;

IAPHelper::IAPHelper()
{
    
}

IAPHelper::~IAPHelper()
{
    CCLOG("Destroying IAPHelper...");
}

bool IAPHelper::initWithProductIdentifiers(std::vector<std::string> productIdentifiers)

{
    // create an empty NSSet
    NSMutableSet *productIdentifiers_set = [NSMutableSet set];
    // add each item from productIdentifiers to NSSet
    for (int i=0; i<productIdentifiers.size(); i++)
    {
        [productIdentifiers_set addObject:(id)[NSString stringWithUTF8String:productIdentifiers[i].c_str()]];
    }
    
    // call the super init function
    _iapHelperObjc = [IAPHelper_objc create:productIdentifiers_set];
    
    return true;
}

void IAPHelper::requestProducts()
{
    [_iapHelperObjc requestProducts];
}

void IAPHelper::buyProduct(SKProduct_opPtr2 product)
{
    [_iapHelperObjc buyProduct:(SKProduct_opPtr)product];
}

bool IAPHelper::productPurchased(const char* productIdentifier)
{
    return [_iapHelperObjc productPurchased:[NSString stringWithUTF8String:productIdentifier]];
}

void IAPHelper::restoreCompletedTransactions()
{
    [_iapHelperObjc restoreCompletedTransactions];
}

void IAPHelper::setDelegate(harp::IAPHelperDelegate* delegate)
{
    _iapHelperObjc.delegate = delegate;
}

harp::IAPHelperDelegate* IAPHelper::getDelegate()
{
    return _iapHelperObjc.delegate;
}

void IAPHelper::releaseCompletelyFor(WrapperCppSKProduct *product)
{
    // release SKProduct_optPtr (as tag)
    if(product->tag != NULL)
    {
        // send in for internal system to release
        [_iapHelperObjc releaseInternalSKProduct:product->tag];
    }
    
    // release wrapper product (also its internal on-heap data)
    delete product;
    CCLOG("Released internal WrapperCppSKProduct");
}

void IAPHelper::releaseCompletelyFor(WrapperCppSKProduct* array[], int size)
{
    for(int i=0; i<size; i++)
    {
        if(array[i] != NULL)
        {
            // get pointer to use
            WrapperCppSKProduct *wrapperSKProduct = array[i];
            
            // release SKProduct_optPtr (as tag)
            if(wrapperSKProduct->tag != NULL)
            {
                // send in for internal system to release
                [_iapHelperObjc releaseInternalSKProduct:wrapperSKProduct->tag];
            }
            
            // release wrapper product (also its internal on-heap data)
            delete wrapperSKProduct;
            array[i] = NULL;
            CCLOG("Released internal WrapperCppSKProduct");
        }
    }
}