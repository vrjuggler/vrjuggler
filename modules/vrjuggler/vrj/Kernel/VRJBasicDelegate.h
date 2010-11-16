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

#import <Foundation/NSObject.h>
#import <Cocoa/Cocoa.h>


@class NSApplication;
@class NSMenu;
@class NSMenuItem;
@class NSMutableArray;
@class NSMutableDictionary;
@class NSNotification;
@class NSString;

/**
 * Basic NSApplication delegate used by vrj::CocoaWrapper.
 *
 * @since 2.3.6
 */
@interface VRJBasicDelegate : NSObject
{
   BOOL                 mLoadConfigs;
   unsigned char        mMaxRecentFiles;
   NSMutableArray*      mRecentCfgFiles;
   NSString*            mPrefsFileName;
   NSMutableDictionary* mPrefsDict;

   NSString* mLastDir;
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

   /**
    * Loads the named VR Juggler configuration file using
    * vrj::Kernel::loadConfigFile().
    */
   -(void) kernelLoadConfigFile:(NSString*) fileName;
   
   /**
    * This is called before the main thread enters the blocking loop and is 
    * called after the Kernel has fully initialized.
    *
    * @since 3.1.1
    */
   -(void) preRun;
  
   /**
    * This is called every event loop in the run method and is used to process
    * the event queue from Cocoa.
    *
    * @since 3.1.1
    */
   -(void) runLoop;
 
   /**
    * This is called to get the next event to process in -runLoop. This
    * implementation uses a call that is known to work well with VR Juggler.
    * It can be overridden to customize the event acquisition, or it can be
    * called by an override of -runLoop to get the event for custom
    * handling.
    *
    * @since 3.1.1
    */
   -(NSEvent*) getNextEvent;
@end

extern NSString* VRJMaxRecentFiles;
extern NSString* VRJRecentCfgFiles;
