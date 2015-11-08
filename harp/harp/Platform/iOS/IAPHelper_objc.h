//
//  IAPHelper_objc.h
//  ZombieHero
//
//  Created by haxpor on 9/4/14.
//
//

#import <Foundation/Foundation.h>

#include "IAPHelperDelegate.h"

typedef harp::IAPHelperDelegate* IAPHelperDelegate;
UIKIT_EXTERN NSString *const IAPHelperProductPurchasedNotification;

@interface IAPHelper_objc : NSObject

typedef struct SKProduct_opPtr_ *SKProduct_opPtr;

@property (nonatomic, assign) IAPHelperDelegate delegate;

- (id)initWithProductIdentifiers:(NSSet*)productIdentifiers;
-(void)requestProducts;
-(void)buyProduct:(SKProduct_opPtr)product;
-(BOOL)productPurchased:(NSString*)productIdentifier;
-(void)restoreCompletedTransactions;
+(id)create:(NSSet*)productIdentifiers;
-(void)dealloc;
-(void)releaseInternalSKProduct:(void*)skProductPtr;

@end
