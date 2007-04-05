/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vrjConfig.h>

#include <assert.h>
#include <boost/bind.hpp>

#import <Foundation/NSValue.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSBundle.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSLock.h>
#import <Foundation/NSAutoreleasePool.h>
#import <Foundation/NSNotification.h>
#import <Foundation/NSPathUtilities.h>
#import <AppKit/NSApplication.h>
#import <Cocoa/Cocoa.h>

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <gadget/Devices/KeyboardMouseDevice/InputAreaCocoa.h>

#include <vrj/Kernel/Kernel.h>
#include <vrj/Kernel/CocoaWrapper.h>


static NSMenu* getRecentFilesMenu()
{
   NSMenu* files_menu = nil;
   NSMenuItem* item = [[NSApp mainMenu] itemWithTitle:@"File"];

   if ( item )
   {
      item = [[item submenu] itemWithTitle:@"Open Recent"];

      if ( item )
      {
         files_menu = [item submenu];
      }
   }

   return files_menu;
}

static NSMenuItem* insertCfgFileItem(NSMenu* menu, NSString* title,
                                     NSString* accel, const int index)
{
/*
   NSMenuItem* item = [[NSMenuItem alloc] init];
   [item setTitle:title];
   [item setAction:@selector(loadConfigFile:)];
   [item setKeyEquivalent:accel];
   [menu insertItem:item
            atIndex:index];
*/
   NSMenuItem* item = [menu insertItemWithTitle:title
                                         action:@selector(loadConfigFile:)
                                  keyEquivalent:accel
                                        atIndex:index];
/*
   NSMenuItem* item = [menu addItemWithTitle:title
                                      action:@selector(loadConfigFile:)
                               keyEquivalent:accel];
*/
   [item setKeyEquivalentModifierMask:NSCommandKeyMask];
   NSLog(@"Inserted item %@ at index %d in menu %@\n", item, index, menu);

   return item;
}

@interface VrjMainController : NSObject
{
   BOOL      mLoadConfigs;
   NSString* mRecentCfgFileName;
}

   -(void) setLoadConfigs:(BOOL) load;
   -(BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*) sencedr;
   -(void) applicationWillFinishLaunching:(NSNotification*) aNotification;
   -(void) applicationDidFinishLaunching:(NSNotification*) aNotification;
   -(NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication*) sender;
   -(void) applicationWillTerminate:(NSNotification*) aNotification;
   -(BOOL) application:(NSApplication*) theApplication
              openFile:(NSString*) file;
   -(BOOL) application:(NSApplication*) theApplication
             openFiles:(NSArray*) files;
   -(IBAction) loadConfigFile:(id) sender;
@end

@implementation VrjMainController
   -(id) init
   {
      mLoadConfigs = YES;
      mRecentCfgFileName = nil;
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
         [mgr createDirectoryAtPath:vrj_app_dir
                         attributes:nil];

         mRecentCfgFileName =
            [vrj_app_dir stringByAppendingPathComponent:@"recent_cfgs"];
         [mRecentCfgFileName retain];
      }

      NSMenu* file_menu = getRecentFilesMenu();

      if ( file_menu )
      {
         NSArray* cfg_files =
            [NSArray arrayWithContentsOfFile:mRecentCfgFileName];

         if ( cfg_files )
         {
            if ( [cfg_files count] > 0 )
            {
               for ( unsigned int i = 0; i < [cfg_files count]; ++i )
               {
                  NSString* fname = [cfg_files objectAtIndex:i];;
                  NSString* accel = [NSString stringWithFormat:@"%d", i];
                  insertCfgFileItem(file_menu, fname, accel, i);
               }
            }
         }
      }

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
      NSMenu* menu = getRecentFilesMenu();

      if ( menu && mRecentCfgFileName )
      {
         const int count = [menu numberOfItems];
         NSMutableArray* recent_files =
            [NSMutableArray arrayWithCapacity:count];

         for ( int i = 0; i < count; ++i )
         {
            NSMenuItem* item = [menu itemAtIndex:i];

            if ( [item isSeparatorItem] )
            {
               break;
            }
         }

         NSLog(@"Files: %@\n", recent_files);
         [recent_files writeToFile:mRecentCfgFileName
                        atomically:YES];
         [mRecentCfgFileName release];
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
         vrj::Kernel::instance()->loadConfigFile([file UTF8String]);
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
            NSString* file = [files objectAtIndex:i];
            vrj::Kernel::instance()->loadConfigFile([file UTF8String]);
         }
      }

      return YES;
   }

   -(IBAction) loadConfigFile:(id) sender
   {
      vrj::Kernel::instance()->loadConfigFile([[sender title] UTF8String]);
   }
@end

@interface DummyThread : NSObject
   +(void) run:(id) obj;
@end

@implementation DummyThread
   +(void) run:(id) obj
   {
      /* Do nothing. */ ;
   }
@end

@interface VrjApplicationController : NSObject
{
   NSString* mLastDir;
}

   -(IBAction) openConfiguration:(id) sender;
   -(IBAction) clearRecentDocuments:(id) sender;
@end

@implementation VrjApplicationController
   -(IBAction) openConfiguration:(id) sender
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

         NSMenu* files_menu = getRecentFilesMenu();

         NSArray* files = [panel filenames];
         int count = [files count];
         for ( int i = 0; i < count; ++i )
         {
            NSString* file = [files objectAtIndex:i];
            vrj::Kernel::instance()->loadConfigFile([file UTF8String]);

            // TODO: Make this limit controlled by user preferences.
            // NOTE: The value 12 accounts for the separator item and the
            // "Clear Menu" item after the separator.
            if ( files_menu && [files_menu numberOfItems] > 12 )
            {
               [files_menu removeItemAtIndex:0];

               for ( int i = 0; i < 9; ++i )
               {
                  NSMenuItem* item = [files_menu itemAtIndex:i];
                  NSString* equiv = [NSString stringWithFormat:@"%d", i];
                  [item setKeyEquivalent:equiv];
               }
            }

            if ( files_menu )
            {
               const int index = [files_menu indexOfItemWithTitle:file];

               if ( index == -1 )
               {
                  int insert_index(0);

                  for ( int i = 0; i < [files_menu numberOfItems]; ++i )
                  {
                     NSMenuItem* item = [files_menu itemAtIndex:i];

                     if ( [item isSeparatorItem] )
                     {
                        insert_index = i - 1;
                        break;
                     }
                  }

                  NSString* equiv =
                     [NSString stringWithFormat:@"%d", insert_index];
                  insertCfgFileItem(files_menu, file, equiv, insert_index);
               }
            }
         }
      }
   }

   -(IBAction) clearRecentDocuments:(id) sender
   {
      NSMenu* menu = [sender menu];
      const int init_size = [menu numberOfItems];

      for ( int i = 0; i < init_size; ++i )
      {
         NSMenuItem* item = [menu itemAtIndex:0];

         if ( [item isSeparatorItem] )
         {
            break;
         }
         else
         {
            [menu removeItem:item];
         }
      }
   }
@end

namespace vrj
{

CocoaWrapper::CocoaWrapper()
   : mMainPool(nil)
//   , mRunning(false)
{
   mMainPool = [[NSAutoreleasePool alloc] init];

   mThreadStartConn =
      vpr::Thread::addThreadStartCallback(
         boost::bind(&CocoaWrapper::threadStarted, this)
      );
   mThreadExitConn =
      vpr::Thread::addThreadExitCallback(
         boost::bind(&CocoaWrapper::threadEnded, this)
      );

   NSConditionLock* lock = gadget::InputAreaCocoa::getWindowLock();
   [lock lock];

   // Spawn a thread so that Cocoa knows that this is a multi-threaded
   // application.
   [NSThread detachNewThreadSelector:@selector(run:)
                            toTarget:[DummyThread class]
                          withObject:nil];
   assert([NSThread isMultiThreaded]);

   NSApplication* app = [NSApplication sharedApplication];

   // Load the NIB file from our bundle.
   NSBundle* bundle = [NSBundle mainBundle];
   NSDictionary* dict = [bundle infoDictionary];
   [NSBundle loadNibNamed:[dict objectForKey:@"NSMainNibFile"]
                    owner:NSApp];

   // Load the VR Juggler property list and use its contents for customizing
   // the behavior of this code.
   NSString* resource_path = [bundle resourcePath];
   NSString* vrj_info_file =
      [resource_path stringByAppendingPathComponent:@"vrjuggler.plist"];
   NSDictionary* vrj_dict =
      [NSDictionary dictionaryWithContentsOfFile:vrj_info_file];

   // Determine whether the application delegate needs to respond to
   // application:openFile: and application:openFiles: messages.
   BOOL load_cfg_files = YES;

   if ( vrj_dict )
   {
      NSNumber* cfg_handling = [vrj_dict objectForKey:@"VRJConfigHandling"];

      if ( cfg_handling )
      {
         load_cfg_files = [cfg_handling boolValue];
      }
   }

   VrjMainController* main_controller =
      [[[VrjMainController alloc] init] autorelease];
   [main_controller setLoadConfigs:load_cfg_files];

   [app setDelegate:main_controller];
}

CocoaWrapper::~CocoaWrapper()
{
   mThreadStartConn.disconnect();
   mThreadExitConn.disconnect();

   [mMainPool release];
   mMainPool = nil;
}

void CocoaWrapper::run()
{
   NSApplication* app = [NSApplication sharedApplication];
/*
   // NOTE: The following is an attempt to reconstruct the NSApplication run
   // method. This approach would be preferred over sending the run message to
   // app because the default behavior of the NSApplication run method keeps
   // it from ever returning.
   [app finishLaunching];
   mRunning = true;
   NSEvent* event = [app nextEventMatchingMask:NSAnyEventMask
                                     untilDate:[NSDate distantFuture]
                                        inMode:NSDefaultRunLoopMode
                                       dequeue:YES];

   // Ensure that the VR Juggler application becomes focused even if it is
   // executed directly from within its bundle.
   [app activateIgnoringOtherApps:YES];

   NSDate* date = [NSDate date];

   while ( mRunning )
   {
      if ( event != nil )
      {
         [app sendEvent:event];
      }

      date = [date addTimeInterval:0.5];
      event = [app nextEventMatchingMask:NSAnyEventMask
                               untilDate:date
                                  inMode:NSDefaultRunLoopMode
                                 dequeue:YES];
   }

   NSLog(@"CocoaWrapper::run returning\n");
*/
   // Ensure that the VR Juggler application becomes focused even if it is
   // executed directly from within its bundle.
   [app activateIgnoringOtherApps:YES];

   // This never returns.
   [app run];
}

void CocoaWrapper::stop()
{
//   mRunning = false;
   [[NSApplication sharedApplication] terminate:nil];
}

void CocoaWrapper::threadStarted()
{
   NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
   *mThreadPool = pool;
}

void CocoaWrapper::threadEnded()
{
   NSAutoreleasePool* pool = *mThreadPool;
   [pool release];
   *mThreadPool = nil;
}

}
