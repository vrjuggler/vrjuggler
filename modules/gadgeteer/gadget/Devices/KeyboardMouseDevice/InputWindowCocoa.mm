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

#include <gadget/gadgetConfig.h>

#include <boost/bind.hpp>

#import <Foundation/NSString.h>
#import <Foundation/NSException.h>
#import <Foundation/NSLock.h>
#import <Foundation/NSNotification.h>
#import <AppKit/NSWindow.h>
#import <AppKit/NSApplication.h>

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>

#include <jccl/Config/ConfigElement.h>

#include <gadget/Util/Debug.h>

#include <gadget/Devices/KeyboardMouseDevice/InputViewCocoa.h>
#include <gadget/Devices/KeyboardMouseDevice/InputWindowCocoa.h>


/**
 * An instance of this class is registered with the default
 * NSNotificationCenter for each gadget::InputWindowCocoa instance to be
 * informed about changes made to the state of the window by external
 * entities (such as the user).
 */
@interface WindowObserver : NSObject
{
   gadget::InputWindowCocoa* mWindow;
}

   -(id) initWithWindow:(gadget::InputWindowCocoa*) window;
   -(void) windowWillClose:(NSNotification*) aNotification;
   -(void) windowDidMove:(NSNotification*) aNotification;
   -(void) windowDidResize:(NSNotification*) aNotification;
@end

@implementation WindowObserver
   -(id) initWithWindow:(gadget::InputWindowCocoa*) window
   {
      mWindow = window;
      return [super init];
   }

   /**
    * Invoked when the Cocoa window is being closed. We tell our
    * gadget::InputWindowCocoa instance that it is being closed so that it
    * does not try to close itself again later. The big issue that this
    * handles is that the NSWindow instance is deallocated when the window
    * gets closed. Hence, the gadget::InputWindowCocoa instance cannot perform
    * any further operations on the window after this point.
    */
   -(void) windowWillClose:(NSNotification*) aNotification
   {
      mWindow->setWindowOpen(false);
   }

   /**
    * Invoked after the Cocoa window is moved.
    */
   -(void) windowDidMove:(NSNotification*) aNotification
   {
      // Inform our input area of the change in our bounds.
      NSRect b = [[aNotification object] frame];
      mWindow->updateOriginAndSize(b.origin.x, b.origin.y, b.size.width,
                                   b.size.height);
   }

   /**
    * Invoked after the Cocoa window is resized (including during live
    * resizing).
    */
   -(void) windowDidResize:(NSNotification*) aNotification
   {
      // Inform our input area of the change in our bounds.
      NSRect b = [[aNotification object] frame];
      mWindow->updateOriginAndSize(b.origin.x, b.origin.y, b.size.width,
                                   b.size.height);
   }
@end

namespace gadget
{

InputWindowCocoa::InputWindowCocoa()
   : gadget::InputAreaCocoa()
   , gadget::Input()
   , mWindowOpen(false)
{
   /* Do nothing. */ ;
}

InputWindowCocoa::~InputWindowCocoa()
{
   /* Do nothing. */ ;
}

std::string InputWindowCocoa::getElementType()
{
   return std::string("input_window");
}

bool InputWindowCocoa::config(jccl::ConfigElementPtr e)
{
   const unsigned int required_definition_ver(1);

   if ( e->getVersion() < required_definition_ver )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": [gadget::InputWindowCocoa::config()] Element named '"
         << e->getName() << "'" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "is version " << e->getVersion()
         << ", but we require at least version " << required_definition_ver
         << ". Ignoring.\n" << vprDEBUG_FLUSH;
      return false;
   }

   if ( ! ( InputArea::config(e) && Input::config(e) ) )
   {
      return false;
   }

   // Get size and position.
   mWidth  = e->getProperty<float>("size", 0);
   mHeight = e->getProperty<float>("size", 1);

   // Sanity checks.
   if ( mWidth <= 0.0f )
   {
      mWidth = 400.0f;
   }

   if ( mHeight <= 0.0f )
   {
      mHeight = 400.0f;
   }

   mX = e->getProperty<float>("origin", 0);
   mY = e->getProperty<float>("origin", 1);

   // TODO: We could use the display_number property to choose the NSScreen
   // where this window will be opened. We could even have the display_system
   // pipe identifiers correlate to NSScreen instances so that we continue to
   // have the same level of indirection that is supposed with X11.
/*
   const int screen_num = e->getProperty<int>("display_number");

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "[gadget::InputWindowCocoa()] display_number = " << screen_num
      << std::endl << vprDEBUG_FLUSH;

   jccl::ConfigElementPtr disp_sys_elt =
      gadget::InputManager::instance()->getDisplaySystemElement();

   if ( NULL == disp_sys_elt.get() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << clrOutNORM(clrRED, "ERROR")
         << ": [gadget::InputWindowCocoa::config()] display_system element "
         << "is NULL.\n" << vprDEBUG_FLUSH;
      return false;
   }

   if ( screen_num >= 0 )
   {
      mScreen = disp_sys_elt->getProperty<unsigned int>("pipes", screen_num);

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "[gadget::InputWindowCocoa] Screen: " << mXDisplayString
         << std::endl << vprDEBUG_FLUSH;
   }
   else
   {
      mScreen = 0;
   }
*/

   return true;
}

bool InputWindowCocoa::sample()
{
   return true;
}

bool InputWindowCocoa::startSampling()
{
   if ( NULL != mThread )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR")
         << ": [gadget::InputWindowXWin::startSampling()] Already sampling!\n"
         << vprDEBUG_FLUSH;
      vprASSERT(false);

      return false;
   }

   bool started(false);

   @try
   {
      unsigned int style_mask = NSTitledWindowMask | NSResizableWindowMask |
                                   NSMiniaturizableWindowMask;

      NSRect content_rect = { {mX, mY}, {mWidth, mHeight} };
      // Creating the window and its view does not open the window. That
      // happens when the -setContentView: message is delivered to
      // mCocoaWindow in waitAndOpen().
      mCocoaWindow =
         [[NSWindow alloc] initWithContentRect:content_rect
                                     styleMask:style_mask
                                       backing:NSBackingStoreBuffered
                                         defer:NO];

      // Register an observer for NSWindowWillCloseNotification events coming
      // from mCocoaWindow. We do this so that we can be told if the window
      // gets closed by some external entity.
      WindowObserver* observer = [[WindowObserver alloc] initWithWindow:this];
      NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
      [nc addObserver:observer
             selector:@selector(windowWillClose:)
                 name:NSWindowWillCloseNotification
               object:mCocoaWindow];

      // Register our observer as an interested party for move and resize
      // notifications.
      [nc addObserver:observer
             selector:@selector(windowDidMove:)
                 name:NSWindowDidMoveNotification
               object:mCocoaWindow];
      [nc addObserver:observer
             selector:@selector(windowDidResize:)
                 name:NSWindowDidResizeNotification
               object:mCocoaWindow];

      NSString* title = [NSString stringWithUTF8String:mInstName.c_str()];
      [mCocoaWindow setTitle:title];
      mMainView = [[InputViewCocoa alloc] initWithFrame:content_rect
                                           andInputArea:this];

      // Ownership of mMainView gets transferred to mCocoaWindow.
      [mCocoaWindow setContentView:mMainView];
      [mCocoaWindow setInitialFirstResponder:mMainView];

      // The reference count for each of mCocoaWindow and mMainView is 1.

      // If the application run loop is already running, then we can go ahead
      // and open the window now.
      if ( [NSApp isRunning] )
      {
         finishOpen();
         started = true;
      }
      // If the run loop is not yet running, then we have to delay opening the
      // window until it is. In this case, we are assuming that sWinOpenLock
      // is held by an object in the primordial thread that will eventually
      // start the application run loop.
      // TODO: See if there is a cleaner way of doing this by somehow queuing
      // up window opening or something so that it happens as soon as it can
      // once the run loop starts.
      else
      {
         try
         {
            mThread =
               new vpr::Thread(boost::bind(&InputWindowCocoa::waitAndOpen,
                                           this));
            started = true;
         }
         // If we cannot spawn the thread that waits to open the window, then
         // we cannot open this window.
         catch (vpr::Exception& ex)
         {
            [mCocoaWindow release];

            mMainView    = nil;
            mCocoaWindow = nil;

            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << clrOutBOLD(clrRED, "ERROR")
               << ": Failed to spawn thread for opening Cocoa input window!\n"
               << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << ex.what() << std::endl << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "Window named " << mKeyboardMouseDeviceName
               << " will not be opened." << std::endl;
         }
      }
   }
   @catch (NSException* ex)
   {
      const char* exception_name   = [[ex name] UTF8String];
      const char* exception_reason = [[ex reason] UTF8String];
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": Failed to create Cocoa input window!\n" << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << exception_name << ": " << exception_reason << std::endl
         << vprDEBUG_FLUSH;

      [mCocoaWindow release];

      mMainView    = nil;
      mCocoaWindow = nil;
   }

   return started;
}

bool InputWindowCocoa::stopSampling()
{
   // The window may have been closed already by the application shutdown
   // procedure.
   if ( mWindowOpen )
   {
      // This causes a release message to be sent to mCocoaWindow after it is
      // closed.
      [mCocoaWindow close];
   }

   mMainView    = nil;
   mCocoaWindow = nil;

   if ( NULL != mThread )
   {
      delete mThread;
      mThread = NULL;
   }

   return true;
}

void InputWindowCocoa::updateData()
{
   /* Do nothing. */ ;
}

void InputWindowCocoa::setWindowOpen(const bool isOpen)
{
   mWindowOpen = isOpen;

   // It might not be a bad idea to set mCocoaWindow to nil here to avoid
   // crashes later if we should try to deliver a message to a deallocated
   // object.
}

void InputWindowCocoa::waitAndOpen()
{
   [sWinOpenLock lock];
   [sWinOpenLock unlock];

   finishOpen();
}

void InputWindowCocoa::finishOpen()
{
   // This causes the window to be opened.
   [mCocoaWindow makeKeyAndOrderFront:mCocoaWindow];
   mWindowOpen = true;

   // We are in the locked state initially.
   if ( mLockState == Lock_LockKey )
   {
      lockMouse();
   }
}

}
