//
//  CloudDocument_objc.m
//  ZombieHero
//
//  Created by haxpor on 1/28/14.
//
//

#import "CloudDocument_objc.h"
#include "cocos2d.h"

using namespace cocos2d;

@implementation CloudDocument_objc

/**
 Init CloudDocument with URL of file.
 
 @param paramURL URL
 @param delegate Delegate receives the change of document
 @return Initialized document
 */
- (id) initWithFileURL:(NSURL *)paramURL
              delegate:(ICloudDocumentProtocol)paramDelegate
              fileName:(NSString *)paramFileName {
    
    if(paramFileName == nil)
    {
        _documentFileName = [NSString stringWithUTF8String:"UserDocument.txt"];
    }
    else
    {
        _documentFileName = [NSString stringWithString:paramFileName];
    }
    
    @try
    {
        self = [super initWithFileURL:paramURL];
        if(self != nil)
        {
            if(paramDelegate == nil)
            {
                CCLOG("Warning: no delegate is given.");
            }
            
            _delegate = paramDelegate;
            _documentText = @"";
        }
    }
    @catch (NSException *exception)
    {
        if([exception.name isEqualToString:NSInvalidArgumentException])
        {
            CCLOG("Something not good happens about URL, return nil now.");
            return nil;
        }
        else
            return nil;
    }
    @finally
    {
        return self;
    }
}

/**
 Init CloudDocument with filename whose file URL will be a document itself residing on Documents folder on iCloud.
 @param paramDelegate Delegate receives the change of document
 @param paramFileName Filename
 @return Initialized document
 */
- (id) initWithFileName:(ICloudDocumentProtocol)paramDelegate
               fileName:(NSString*)paramFileName
{
    return [self initWithFileURL:[self urlForFileInDocumentsDirectoryInICloud:paramFileName] delegate:paramDelegate fileName:paramFileName];
}

- (id) initWithFileURL:(NSURL *)paramURL {
    return [self initWithFileURL:paramURL delegate:nil fileName:@"UserDocument.txt"];
}

// Internal write
- (id) contentsForType:(NSString *)typeName error:(NSError **)outError {
    NSUInteger length = [self.documentText length];
    
    CCLOG("typeName = %s", [typeName UTF8String]);
    CCLOG("documentText = %s (%d bytes)", [self.documentText UTF8String], length);
    
    // return created NSData from byte array
    return [NSData dataWithBytes:[self.documentText UTF8String] length:length];
}

// Internal read
- (BOOL) loadFromContents:(id)contents ofType:(NSString *)typeName error:(NSError **)outError {
    NSData *data = (NSData*)contents;
    
    if([data length] == 0)
    {
        self.documentText = @"";
    }
    else
    {
        self.documentText = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    }
    
    if(dynamic_cast<ICloudDocumentProtocol>(_delegate) != NULL)
    {
        _delegate->cloudDocumentChanged([self.documentText UTF8String]);
    }
    
    return YES;
}

/**
 Get URL for Documents directory on iCloud. If no Documents folder exists, then it tries to create it before returning such URL.
 
 @return URL of Documents folder on iCloud
 */
- (NSURL *) urlForDocumentDirectoryInICloud
{
    NSURL *result = nil;
    
    NSString *teamID = @PREFIX_ICLOUD_ID;
    NSString *containerID = [[NSBundle mainBundle] bundleIdentifier];
    NSString *teamIDandContainerID = [NSString stringWithFormat:@"%@.%@", teamID, containerID];
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSURL *iCloudURL = [fileManager URLForUbiquityContainerIdentifier:teamIDandContainerID];
    if(iCloudURL == nil)
    {
        CCLOG("Error: iCloud cannot be reached.");
        
        // call delegate's callback
        if(dynamic_cast<ICloudDocumentProtocol>(_delegate) != NULL)
        {
            _delegate->oniCloudCannotBeReached();
        }
        return nil;
    }
    
    NSURL *documentsFolderURLInIcloud = [iCloudURL URLByAppendingPathComponent:@"Documents" isDirectory:YES];
    // if it doesn't exist, create it
    if([fileManager fileExistsAtPath:[documentsFolderURLInIcloud path]] == NO)
    {
        //NSLog(@"The documents folder does NOT exist in iCloud. Creating ...");
        NSError *folderCreationError = nil;
        BOOL created = [fileManager createDirectoryAtURL:documentsFolderURLInIcloud withIntermediateDirectories:YES attributes:nil error:&folderCreationError];
        
        if(created)
        {
            CCLOG("Successfully created the Documents in iCloud");
            result = documentsFolderURLInIcloud;
        }
        else
        {
            CCLOG("Failed to create the Documents folder in iCloud \
                  Error = %s", [[folderCreationError localizedDescription] UTF8String]);
        }
    }
    else
    {
        CCLOG("The Documents folder already exists in iCloud.");
        result = documentsFolderURLInIcloud;
    }
    
    return result;
}

/**
 Get URL of file in Documents directory on iCloud.
 
 @return File URL in Documents directory on iCloud
 */
- (NSURL *) urlForFileInDocumentsDirectoryInICloud:(NSString *)paramFileName
{
    return [[self urlForDocumentDirectoryInICloud] URLByAppendingPathComponent:paramFileName];
}

+ (BOOL) isiCloudAvailable
{
    if([[NSFileManager defaultManager] ubiquityIdentityToken] == nil)
        return NO;
    else
        return YES;
}

+ (id) create:(NSURL *)paramURL delegate:(ICloudDocumentProtocol)paramDelegate fileName:(NSString *)paramFileName
{
    return [[self alloc] initWithFileURL:paramURL delegate:paramDelegate fileName:paramFileName];
}

+ (id) create:(ICloudDocumentProtocol)paramDelegate fileName:(NSString*)paramFileName
{
    return [[self alloc] initWithFileName:paramDelegate fileName:paramFileName];
}

// Read the document file on iCloud
- (void) read
{
    [self openWithCompletionHandler:^(BOOL success) {
        if(success)
        {
            CCLOG("Successfully loaded the document file from iCloud.");
            CCLOG("Content = %s", [_documentText UTF8String]);
            
            // call delegate's callback
            if(dynamic_cast<ICloudDocumentProtocol>(_delegate) != NULL)
            {
                _delegate->onSuccessReading();
            }
        }
        else
        {
            CCLOG("Failed to load the document from iCloud.");
            
            // call delegate's callback
            if(dynamic_cast<ICloudDocumentProtocol>(_delegate) != NULL)
            {
                _delegate->onErrorReading("Error reading: Failed to load the document from iCloud.");
            }
        }
    }];
}

// Write the document file on iCloud
- (void) write
{
    [self saveToURL:[self urlForFileInDocumentsDirectoryInICloud:_documentFileName] forSaveOperation:UIDocumentSaveForOverwriting completionHandler:^(BOOL success) {
        if(success)
        {
            CCLOG("Successfully wrote the document file to iCloud.");
            
            // call delegate's callback
            if(dynamic_cast<ICloudDocumentProtocol>(_delegate) != NULL)
            {
                _delegate->onSuccessWriting();
            }
        }
        else
        {
            CCLOG("Failed to write document file to iCloud.");
            
            // call delegate's callback
            if(dynamic_cast<ICloudDocumentProtocol>(_delegate) != NULL)
            {
                _delegate->onErrorWriting("Error writing: Failed to write document file to iCloud.");
            }
        }
    }];
}

/**
 @brief Check if the file exists.
 @return True if the file exists, otherwise returns false.
 */
- (BOOL) exist
{
    NSString *teamID = @PREFIX_ICLOUD_ID;
    NSString *containerID = [[NSBundle mainBundle] bundleIdentifier];
    NSString *teamIDandContainerID = [NSString stringWithFormat:@"%@.%@", teamID, containerID];
    
    CCLOG("containerID: %s", [teamIDandContainerID UTF8String]);
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSURL *iCloudURL = [fileManager URLForUbiquityContainerIdentifier:teamIDandContainerID];
    if(iCloudURL == nil)
    {
        CCLOG("Error: iCloud cannot be reached.");
        
        // call delegate's callback
        if(dynamic_cast<ICloudDocumentProtocol>(_delegate) != NULL)
        {
            _delegate->oniCloudCannotBeReached();
        }
        return NO;
    }
    
    // append filePath with iCloud URL
    NSString* filePath = [NSString stringWithFormat:@"Documents/%@", self.documentFileName];
    NSURL *fileIniCloudURL = [iCloudURL URLByAppendingPathComponent:filePath];
    if([fileManager fileExistsAtPath:[fileIniCloudURL path]] == YES)
    {
        return YES;
    }
    else
    {
        return NO;
    }
}

@end
