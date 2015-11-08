//
//  CloudDocument_objc.h
//  ZombieHero
//
//  Created by haxpor on 1/28/14.
//
//

#import <UIKit/UIKit.h>
#include "ICloudDocumentProtocol.h"

//#define PREFIX_ICLOUD_ID "XGKHW634KL"
#define PREFIX_ICLOUD_ID "iCloud"

typedef harp::ICloudDocumentProtocol* ICloudDocumentProtocol;

@interface CloudDocument_objc : UIDocument

@property (nonatomic, strong) NSString *documentText;
@property (nonatomic, assign) ICloudDocumentProtocol delegate;
@property (nonatomic, strong, readonly) NSString *documentFileName;

/* Designated Initializer */
- (id) initWithFileURL:(NSURL *)paramURL delegate:(ICloudDocumentProtocol)paramDelegate fileName:(NSString*)paramFileName;
- (id) initWithFileName:(ICloudDocumentProtocol)paramDelegate fileName:(NSString*)paramFileName;
+ (id) create:(NSURL *)paramURL delegate:(ICloudDocumentProtocol)paramDelegate fileName:(NSString*)paramFileName;
+ (id) create:(ICloudDocumentProtocol)paramDelegate fileName:(NSString*)paramFileName;
- (void) dealloc;

/**
 @brief Allow others to check if iCloud is available before actually start.
 @return True if it's available, otherwise false.
 */
+ (BOOL) isiCloudAvailable;

- (NSURL *) urlForDocumentDirectoryInICloud;
- (NSURL *) urlForFileInDocumentsDirectoryInICloud:(NSString *)paramFileName;

- (void) read;
- (void) write;
- (BOOL) exist;

@end
