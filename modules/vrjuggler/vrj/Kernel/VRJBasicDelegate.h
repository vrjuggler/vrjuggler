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
@end

extern NSString* VRJMaxRecentFiles;
extern NSString* VRJRecentCfgFiles;
