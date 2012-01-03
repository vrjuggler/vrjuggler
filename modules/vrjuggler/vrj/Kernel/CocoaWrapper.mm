/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#include <assert.h>
#include <boost/bind.hpp>
#include <objc/objc-runtime.h>

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <vpr/System.h>

#include <gadget/Devices/KeyboardMouseDevice/InputAreaCocoa.h>

#include <jccl/RTRC/ConfigManager.h>

#import <vrj/Kernel/VRJBasicDelegate.h>
#include <vrj/Kernel/CocoaWrapper.h>

#import <Foundation/NSObject.h>
#import <Foundation/NSBundle.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSLock.h>
#import <Foundation/NSAutoreleasePool.h>
#import <AppKit/NSApplication.h>
#import <Cocoa/Cocoa.h>

@interface VRJDummyThread : NSObject
   +(void) run:(id) obj;
@end

@implementation VRJDummyThread
   +(void) run:(id) obj
   {
      /* Do nothing. */ ;
   }
@end

namespace vrj
{

CocoaWrapper::CocoaWrapper()
   : mMainPool(nil)
   , mRunning(false)
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
                            toTarget:[VRJDummyThread class]
                          withObject:nil];
   assert([NSThread isMultiThreaded]);
    
    // If the user wants to use a different NSApplication than the default one
    // look it up in the Info.plist file for this app bundle.
    NSDictionary* info_dictionary = [[NSBundle mainBundle] infoDictionary];
    Class principal_class = 
        NSClassFromString([info_dictionary objectForKey:@"NSPrincipalClass"]);
    if (principal_class != nil)
    {
        NSApplication* the_application = [principal_class sharedApplication];
    }
    
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
   NSString* ctrl_class_name = @"VRJBasicDelegate";

   if ( vrj_dict )
   {
      NSNumber* cfg_handling = [vrj_dict objectForKey:@"VRJConfigHandling"];

      if ( cfg_handling )
      {
         load_cfg_files = [cfg_handling boolValue];
      }

      NSString* name = [vrj_dict objectForKey:@"VRJDelegateClass"];

      if ( nil != name )
      {
         ctrl_class_name = name;
      }
   }

   id controller_class = objc_getClass([ctrl_class_name UTF8String]);

   if ( nil == controller_class )
   {
      NSLog(@"WARNING: Could not find declaration of %@!\n", ctrl_class_name);
      NSLog(@"         Falling back on VRJBasicDelegate\n");
      controller_class = [VRJBasicDelegate class];
   }

   id main_controller = [[[controller_class alloc] init] autorelease];
   [main_controller setLoadConfigs:load_cfg_files];

   [app setDelegate:main_controller];
   
   mVRJDelegate = main_controller;
}

CocoaWrapper::~CocoaWrapper()
{
   mThreadStartConn.disconnect();
   mThreadExitConn.disconnect();

   [mMainPool release];
   mMainPool = nil;
   mVRJDelegate = nil;
}

void CocoaWrapper::run()
{
   NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

   NSApplication* app = [NSApplication sharedApplication];

   [pool drain];

   NSLog(@"NSApp in CocoaWrapper::run : %@", NSApp);

   // We must block here until the kernel thread has been initialized so that
   // we can let Cocoa know that all windows have been created.
   while (! jccl::ConfigManager::instance()->isPendingStale())
   {
      vpr::System::msleep( 200 );  // tenth-second delay
   }

   [mVRJDelegate preRun];

   // NOTE: The following is an attempt to reconstruct the NSApplication run
   // method. This approach would be preferred over sending the run message to
   // app because the default behavior of the NSApplication run method keeps
   // it from ever returning.
   pool = [[NSAutoreleasePool alloc] init];

   // Call this now that we know all windows managed by VR Juggler have been
   // created and that any Application specific Cocoa objects have been created
   // in the preRun delegate call.
   [app finishLaunching];

   [pool drain];

   mRunning = true;

   pool = [[NSAutoreleasePool alloc] init];

   // Ensure that the VR Juggler application becomes focused even if it is
   // executed directly from within its bundle.
   [app activateIgnoringOtherApps:YES];

   [pool drain];

   while (mRunning)
   {
      pool = [[NSAutoreleasePool alloc] init];

      [mVRJDelegate runLoop];
        
      [app updateWindows];

      [pool drain];
   }

   NSLog(@"CocoaWrapper::run returning\n");
}

void CocoaWrapper::stop()
{
   mRunning = false;
}

void CocoaWrapper::threadStarted()
{
   NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
   *mThreadPool = pool;
}

void CocoaWrapper::threadEnded()
{
   try
   {
      // XXX: Accessing the thread-specific data can fail in some cases. It is
      // not clear to me why this can happen, but it seems to be tied to
      // using remote run-time reconfiguration. It may be related to omniORB
      // creating threads of its own, but I am not sure.  -PH 2/13/2008
      NSAutoreleasePool* pool = *mThreadPool;
      [pool release];
      //*mThreadPool = nil;
   }
   catch (vpr::BadCastException& ex)
   {
      std::cerr << "NOTE: Failed to release reference to NSAutoreleasePool:\n"
                << ex.getExtendedDescription() << std::endl;
   }
}

}
