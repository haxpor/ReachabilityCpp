//
//  IAPHelper_objc.mm
//  ZombieHero
//
//  Created by haxpor on 9/4/14.
//
//

#import "IAPHelper_objc.h"
#include "cocos2d.h"
#import <StoreKit/StoreKit.h>

using namespace harp;
using namespace cocos2d;

NSString *const IAPHelperProductPurchasedNotification = @"IAPHelperProductPurchasedNotification";

typedef struct SKProduct_opPtr_
{
    SKProduct* product;
    
    SKProduct_opPtr_()
    {
        product = NULL;
    }
    
} SKProduct_opPtr_, *SKProduct_opPtr;

@interface IAPHelper_objc () <SKProductsRequestDelegate, SKPaymentTransactionObserver>
@end

@implementation IAPHelper_objc {
    SKProductsRequest *_productsRequest;
    
    NSSet *_productIdentifiers;
    NSMutableSet *_purchasedProductIdentifiers;
}

-(id)initWithProductIdentifiers:(NSSet *)productIdentifiers {
    
    if((self = [self init]))
    {
        // store product identifiers
        _productIdentifiers = productIdentifiers;
        
        // check for previously purchased products
        _purchasedProductIdentifiers = [NSMutableSet set];
        for (NSString *productIdentifier in _productIdentifiers) {
            BOOL productPurchased = [[NSUserDefaults standardUserDefaults] boolForKey:productIdentifier];
            if(productPurchased){
                [_purchasedProductIdentifiers addObject:productIdentifier];
                CCLOG("Previously purchased: %s", [productIdentifier UTF8String]);
            }
            else{
                CCLOG("Not purchased: %s", [productIdentifier UTF8String]);
            }
        }
        
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    }
    return self;
}

-(void)requestProducts {
    _productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:_productIdentifiers];
    _productsRequest.delegate = self;
    [_productsRequest start];
}

#pragma mark - SKProductsRequestDelegate

-(void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    CCLOG("Loaded list of products ...");
    _productsRequest = nil;
    
    // also create vector from NSArray
    std::vector<WrapperCppSKProduct*> *sendInProducts = new std::vector<WrapperCppSKProduct*>();
    
    NSArray *skProducts = response.products;
    for(SKProduct *skProduct in skProducts)
    {
        // create a WrapperCppSKProduct
        WrapperCppSKProduct *wrapperSKProduct = new WrapperCppSKProduct();
        wrapperSKProduct->productIdentifier = new std::string([skProduct.productIdentifier UTF8String]);
        wrapperSKProduct->localizedTitle = new std::string([skProduct.localizedTitle UTF8String]);
        wrapperSKProduct->localizedDescription = new std::string([skProduct.localizedDescription UTF8String]);
        wrapperSKProduct->price = skProduct.price.floatValue;
        
        // get localized price
        NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
        [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
        [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
        [numberFormatter setLocale:skProduct.priceLocale];
        
        wrapperSKProduct->localizedPrice = new std::string([[numberFormatter stringFromNumber:skProduct.price] UTF8String]);
        
        // save SKProduct
        SKProduct_opPtr tagSKProduct = new SKProduct_opPtr_();
        tagSKProduct->product = skProduct;
        // set it as tag inside a wrapper structure
        wrapperSKProduct->tag = (void*)tagSKProduct;
        
        sendInProducts->push_back(wrapperSKProduct);
    }
    
    // call callback function
    if(_delegate != nil)
        _delegate->onRequestProductsCompleted(true, sendInProducts);
    
    // delete array here
    delete sendInProducts;
    sendInProducts = NULL;
}

-(void)request:(SKRequest *)request didFailWithError:(NSError *)error {
    CCLOG("Failed to load products");
    _productsRequest = nil;
    
    // call callback function
    if(_delegate != nil)
        _delegate->onRequestProductsCompleted(false, NULL);
}

-(void)buyProduct:(SKProduct_opPtr)product {
    CCLOG("Buying %s...", [product->product.productIdentifier UTF8String]);
    
    SKPayment *payment = [SKPayment paymentWithProduct:product->product];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

-(BOOL)productPurchased:(NSString *)productIdentifier {
    return [_purchasedProductIdentifiers containsObject:productIdentifier];
}

-(void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
    for(SKPaymentTransaction *transaction in transactions)
    {
        switch(transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
                break;
        }
    }
}

-(void)completeTransaction:(SKPaymentTransaction *)transaction {
    CCLOG("Complete transaction...");
    
    [self provideContentForProductIdentifier:transaction.payment.productIdentifier];
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
    // call complete callback
    if(_delegate != nil)
        _delegate->onCompleteTransaction([transaction.payment.productIdentifier UTF8String]);
}

-(void)restoreTransaction:(SKPaymentTransaction *)transaction {
    CCLOG("Restore transaction...");
    
    [self provideContentForProductIdentifier:transaction.originalTransaction.payment.productIdentifier];
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
    // call complete callback
    if(_delegate != nil)
        _delegate->onCompleteTransaction([transaction.payment.productIdentifier UTF8String]);
}

-(void)failedTransaction:(SKPaymentTransaction *)transaction {
    // at last we finish the transaction
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
    // cancelled
    if(transaction.error.code == SKErrorPaymentCancelled)
    {
        CCLOG("Cancelled transaction...");
        
        if(_delegate != nil)
            _delegate->onCancelledTransaction([transaction.payment.productIdentifier UTF8String]);
    }
    else
    {
        CCLOG("Failed transaction ...");
        
        if(_delegate != nil)
            _delegate->onFailedTransaction([transaction.payment.productIdentifier UTF8String]);
    }
}

-(void)provideContentForProductIdentifier:(NSString *)productIdentifier {
    [_purchasedProductIdentifiers addObject:productIdentifier];
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:productIdentifier];
    [[NSUserDefaults standardUserDefaults] synchronize];
    [[NSNotificationCenter defaultCenter] postNotificationName:IAPHelperProductPurchasedNotification object:productIdentifier userInfo:nil];
}

-(void)restoreCompletedTransactions {
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

+(id)create:(NSSet*)productIdentifiers {
    return [[self alloc] initWithProductIdentifiers:productIdentifiers];
}

-(void)releaseInternalSKProduct:(void*)skProductPtr {
    if(skProductPtr != nil)
    {
        // convert to SKProduct_opPtr
        SKProduct_opPtr skProductStruct = (SKProduct_opPtr)skProductPtr;
        if(skProductStruct != nil)
        {
            // release SKProduct
            if(skProductStruct->product != nil)
            {
                skProductStruct->product = nil;
                
                CCLOG("      Released SKProduct->product");
            }
            
            // release its whole structure
            delete skProductStruct;
            CCLOG("   Released whole SKProduct");
        }
    }
}

@end

