/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vrjConfig.h>

#import <Foundation/NSDictionary.h>
#import <Foundation/NSLock.h>
#import <Foundation/NSNotification.h>
#import <Foundation/NSPathUtilities.h>
#import <AppKit/NSApplication.h>

#include <gadget/Devices/KeyboardMouseDevice/InputAreaCocoa.h>

#import <vrj/Kernel/Kernel.h>
#import <vrj/Kernel/VRJBasicDelegate.h>


NSString* VRJMaxRecentFiles = @"VRJMaxRecentFiles";
NSString* VRJRecentCfgFiles = @"VRJRecentCfgFiles";

@implementation VRJBasicDelegate
   -(id) init
   {
      mLoadConfigs    = YES;
      mMaxRecentFiles = 10;
      mRecentCfgFiles = nil;
      mPrefsFileName  = nil;
      mPrefsDict      = nil;

      NSFileManager* mgr = [NSFileManager defaultManager];
      NSArray* paths =
         NSSearchPathForDirectoriesInDomains(
            NSApplicationSupportDirectory, NSUserDomainMask, YES
         );

      if ( [paths count] > 0 )
      {
         NSString* app_dir = [paths objectAtIndex:0];
         NSString* vrj_app_dir =
            [app_dir stringByAppendingPathComponent:@"VR Juggler"];
#if VPR_OS_RELEASE_MAJOR < 9
         BOOL created = [mgr createDirectoryAtPath:vrj_app_dir
                                        attributes:nil];
#else
         NSError* error;
         BOOL created =
            [mgr       createDirectoryAtPath:vrj_app_dir
                 withIntermediateDirectories:YES
                                  attributes:nil
                                       error:&error];
#endif

         if ( created )
         {
            mPrefsFileName =
               [vrj_app_dir stringByAppendingPathComponent:@"prefs.plist"];
            [mPrefsFileName retain];
         }
         else
         {
#if VPR_OS_RELEASE_MAJOR < 9
            NSLog(@"Failed to create directory %@\n", vrj_app_dir);
#else
            NSLog(@"Failed to create directory %@: %@\n", vrj_app_dir, error);
#endif
         }
      }

      // If we were able to construct the path to the preferences file, then
      // we try to load it into a dictionary.
      if ( mPrefsFileName )
      {
         mPrefsDict =
            [NSDictionary dictionaryWithContentsOfFile:mPrefsFileName];
      }

      // Create a mutable copy of mPrefsDict.
      if ( mPrefsDict )
      {
         mPrefsDict = [mPrefsDict mutableCopy];
      }
      // If we don't have a preferences dictionary at this point, then we
      // will create one.
      else
      {
         mPrefsDict = [[NSMutableDictionary dictionary] retain];
      }

      NSNumber* max_recent = [mPrefsDict objectForKey:VRJMaxRecentFiles];

      if ( max_recent )
      {
         mMaxRecentFiles = [max_recent unsignedCharValue];
      }

      mRecentCfgFiles = [mPrefsDict objectForKey:VRJRecentCfgFiles];

      // Create a mutable copy of mRecentCfgFiles.
      if ( mRecentCfgFiles )
      {
         mRecentCfgFiles = [mRecentCfgFiles mutableCopy];
      }
      else
      {
         mRecentCfgFiles = [[NSMutableArray array] retain];
      }

      const unsigned int count = [mRecentCfgFiles count];

      NSDocumentController* doc_ctrl =
         [NSDocumentController sharedDocumentController];

      for ( unsigned int i = 0; i < count; ++i )
      {
         NSURL* url =
            [NSURL fileURLWithPath:[mRecentCfgFiles objectAtIndex:i]];
         [doc_ctrl noteNewRecentDocumentURL:url];
      }

      return [super init];
   }

   -(void) setLoadConfigs:(BOOL) load
   {
      mLoadConfigs = load;
   }

   -(BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*) sencedr
   {
      // We return NO here because we have a different way of shutting down
      // the application. When vrj::Kernel::stop() is invoked, it will cause
      // the application run loop to stop by invoking
      // vrj::CocoaWrapper::stop().
      return NO;
   }

   -(void) applicationWillFinishLaunching:(NSNotification*) aNotification
   {
   }

   -(void) applicationDidFinishLaunching:(NSNotification*) aNotification
   {
      // We're ready to allow windows to open!
      NSConditionLock* lock = gadget::InputAreaCocoa::getWindowLock();
      [lock unlock];
   }

   -(NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication*) sender
   {
      return NSTerminateNow;
   }

   /**
    * Clean up before terminating.
    */
   -(void) applicationWillTerminate:(NSNotification*) aNotification
   {
      if ( mPrefsFileName )
      {
         [mPrefsDict setObject:[NSNumber numberWithUnsignedChar:mMaxRecentFiles]
                        forKey:VRJMaxRecentFiles];
         [mPrefsDict setObject:mRecentCfgFiles
                        forKey:VRJRecentCfgFiles];

         [mPrefsDict writeToFile:mPrefsFileName
                      atomically:YES];
         [mPrefsFileName release];
      }

      vrj::Kernel::instance()->stop();

      // NOTE: This call is to ensure that we wait for the kernel to stop
      // completely before exiting the run loop. This is necessary to prevent
      // race conditions on shutdown caused by the default NSApplication run
      // method implementation calling exit(3) which in turn causes the Juggler
      // singletons to be deleted. We do not want that to happen until after
      // the kernel has stopped itself.
      vrj::Kernel::instance()->doWaitForKernelStop();
   }

   -(BOOL) application:(NSApplication*) theApplication
              openFile:(NSString*) file
   {
      if ( mLoadConfigs )
      {
         [self kernelLoadConfigFile:file];
      }

      return YES;
   }

   -(BOOL) application:(NSApplication*) theApplication
             openFiles:(NSArray*) files
   {
      if ( mLoadConfigs )
      {
         const int count = [files count];
         for ( int i = 0; i < count; ++i )
         {
            [self kernelLoadConfigFile:[files objectAtIndex:i]];
         }
      }

      return YES;
   }

   -(void) kernelLoadConfigFile:(NSString*) fileName
   {
      // Load the configuration file.
      vrj::Kernel::instance()->loadConfigFile([fileName UTF8String]);

      // Update the submenu listing the recently opened configuration files
      // to include fileName.
      const NSUInteger index = [mRecentCfgFiles indexOfObject:fileName];

      if ( index != NSNotFound )
      {
         [mRecentCfgFiles removeObjectAtIndex:index];
      }
      else if ( ! [mRecentCfgFiles count] > mMaxRecentFiles )
      {
         [mRecentCfgFiles removeObjectAtIndex:0];
      }

      [mRecentCfgFiles addObject:fileName];

      NSDocumentController* doc_ctrl =
         [NSDocumentController sharedDocumentController];
      [doc_ctrl noteNewRecentDocumentURL:[NSURL fileURLWithPath:fileName]];
   }

   /**
    * Receives the message sent by the File -> Open menu item. An NSOpenPanel
    * is displayed, and if the user clicks the OK button, the selected .jconf
    * file(s) is (are) loaded.
    *
    * @post If the user clicks the OK button, \c mLastDir has the name of the
    *       directory where the opened panel was when it was closed.
    *
    * @see -kernelLoadConfigFile:
    */
   -(IBAction) openDocument:(id) sender
   {
      NSArray* file_types = [NSArray arrayWithObject:@"jconf"];
      NSOpenPanel* panel = [NSOpenPanel openPanel];

      [panel setAllowsMultipleSelection:YES];

      if ( ! mLastDir )
      {
         mLastDir = NSHomeDirectory();
         [mLastDir retain];
      }

      const int result = [panel runModalForDirectory:mLastDir
                                                file:nil
                                               types:file_types];

      if ( result == NSOKButton )
      {
         [mLastDir release];
         mLastDir = [panel directory];
         [mLastDir retain];

         NSArray* files = [panel filenames];
         const unsigned int count = [files count];

         for ( unsigned int i = 0; i < count; ++i )
         {
            NSString* file = [files objectAtIndex:i];
            [self kernelLoadConfigFile:file];
         }
      }
   }

   /**
    * Receives the message sent by the "Clear Menu" item in the
    * File -> Open Recent submenu.
    *
    * @post \c mRecentCfgFiles is empty.
    */
   -(IBAction) clearRecentDocuments:(id) sender
   {
      NSDocumentController* doc_ctrl =
         [NSDocumentController sharedDocumentController];
      [doc_ctrl clearRecentDocuments:sender];
      [mRecentCfgFiles removeAllObjects];
   }
@end
