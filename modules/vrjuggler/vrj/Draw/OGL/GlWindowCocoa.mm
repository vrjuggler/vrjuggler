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

#include <vrj/Draw/OGL/Config.h>

#include <boost/bind.hpp>

#import <Foundation/NSArray.h>
#import <Foundation/NSException.h>
#import <Foundation/NSString.h>
#import <Foundation/NSLock.h>
#import <Foundation/NSNotification.h>
#import <Foundation/NSAutoreleasePool.h>
#import <AppKit/NSWindow.h>
#import <AppKit/NSOpenGL.h>
#import <AppKit/NSOpenGLView.h>
#import <AppKit/NSScreen.h>
#import <AppKit/NSApplication.h>
#import <OpenGL/GL.h>

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <jccl/Config/ConfigElement.h>

#include <vrj/Display/DisplayManager.h>
#include <vrj/Util/Debug.h>
#import <vrj/Draw/OGL/GlViewCocoa.h>
#include <vrj/Draw/OGL/GlWindowCocoa.h>


/**
 * A simple subclass of NSWindow that overrides -canBecomeKeyWindow so that
 * it returns YES.
 */
@interface VrjCustomWindow : NSWindow
@end

@implementation VrjCustomWindow
   -(BOOL) canBecomeKeyWindow
   {
      return YES;
   }
@end

/**
 * An instance of this class is set as the delegate for the NSWindow object
 * assocated with each vrj::GlWindowCocoa instance.
 */
@interface GlWindowDelegate : NSObject
{
   vrj::GlWindowCocoa* mWindow;
}

   -(id) initWithWindow:(vrj::GlWindowCocoa*) window;
@end

@implementation GlWindowDelegate
   -(id) initWithWindow:(vrj::GlWindowCocoa*) window
   {
      mWindow = window;
      return [super init];
   }

   /**
    * Invoked when the Cocoa window is being closed. We tell our
    * vrj::GlWindowCocoa instance that it is being closed so that it does not
    * try to close itself again later. The big issue that this handles is that
    * the NSWindow instance is deallocated when the window gets closed. Hence,
    * the vrj::GlWindowCocoa instance cannot perform any further operations on
    * the window after this point.
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
      mWindow->updateBounds(b.origin.x, b.origin.y, b.size.width,
                            b.size.height);
   }
@end

namespace vrj
{

GlWindowCocoa::GlWindowCocoa()
   : vrj::GlWindow()
   , gadget::InputAreaCocoa()
   , mThread(NULL)
   , mScreen(0)
   , mWindowName(@"Cocoa OpenGL Window")
   , mRenderLock(nil)
   , mGlView(nil)
{
   /* Do nothing. */ ;
}

GlWindowCocoa::~GlWindowCocoa()
{
   if ( NULL != mThread )
   {
      delete mThread;
      mThread = NULL;
   }
}

bool GlWindowCocoa::open()
{
   if ( mWindowIsOpen )
   {
      return true;
   }

   if ( mWindowWidth == -1 )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR")
         << ": [vrj::GlWindowCocoa::open()] Window has not been configured\n"
         << vprDEBUG_FLUSH;
      return false;
   }

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
      << "[vrj::GlWindowCocoa::open()] Open window\n" << vprDEBUG_FLUSH;

   mRenderLock = [[NSLock alloc] init];

   bool opened(false);

   @try
   {
      unsigned int style_mask(0);

      // If the window is borderless, set up the style mask to reflect that.
      // If the window is to be full screen, then we force it to be
      // borderless. This is to make Aqua happy.
      if ( ! mHasBorder || mIsFullScreen )
      {
         style_mask = NSBorderlessWindowMask;
      }
      else
      {
#if VPR_OS_RELEASE_MAJOR >= 9
         style_mask = NSTitledWindowMask | NSResizableWindowMask |
                         NSMiniaturizableWindowMask;
#else
         // Resizing VR Juggler OpenGL windows crashes the window server on
         // Mac OS X 10.4 (Tiger).
         style_mask = NSTitledWindowMask | NSMiniaturizableWindowMask;
#endif
      }

      NSRect content_rect = { {mOriginX, mOriginY},
                              {mWindowWidth, mWindowHeight} };

      // mScreen is already known to be a valid index.
      NSScreen* screen = [[NSScreen screens] objectAtIndex:mScreen];

      // Creating the window and its view does not open the window. That
      // happens when the -setContentView: message is delivered to
      // mCocoaWindow in finishOpen().
      mCocoaWindow =
         [[VrjCustomWindow alloc] initWithContentRect:content_rect
                                            styleMask:style_mask
                                              backing:NSBackingStoreBuffered
                                                defer:NO
                                               screen:screen];
      [mCocoaWindow setOneShot:YES];

      // Set the delegate for mCocoaWindow. In particular, we do this so that
      // we can be told if the window gets closed by some external entity.
      id delegate =
         [[[GlWindowDelegate alloc] initWithWindow:this] autorelease];
      [mCocoaWindow setDelegate:delegate];

      [mCocoaWindow setTitle:mWindowName];
      [mCocoaWindow setPreservesContentDuringLiveResize:NO];

      const BOOL handle_input = NULL != mKeyboardMouseDevice;
      mGlView = [[GlViewCocoa alloc] initWithFrame:content_rect
                                          glWindow:this
                                            screen:screen
                                     inputHandling:handle_input];
      mMainView = mGlView;

      // If the window is full screen, then it has to be at the shielding
      // window level.
      if ( mIsFullScreen )
      {
         [mCocoaWindow setLevel:CGShieldingWindowLevel()];
      }
      // If the window is to be on top of all other windows, we put it at a
      // level that is one less than the screen saver level. This should set
      // it up to get along well with other applications.
      else if ( mAlwaysOnTop )
      {
         [mCocoaWindow setLevel:NSScreenSaverWindowLevel - 1];
      }

      if ( [NSApp isRunning] )
      {
         finishOpen();
         opened = true;
      }
      else
      {
         try
         {
            mThread = new vpr::Thread(boost::bind(&GlWindowCocoa::waitAndOpen,
                                                  this));
            opened = true;
         }
         catch (vpr::Exception& ex)
         {
            vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
               << clrOutBOLD(clrRED, "ERROR")
               << ": Failed to spawn thread for opening Cocoa OpenGL window!\n"
               << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
               << ex.what() << std::endl << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
               << "Window named " << [mWindowName UTF8String]
               << " will not be opened." << std::endl;
         }
      }

      if ( opened )
      {
         makeCurrent();
         // TODO: Add extension loading stuff.

         // mCocoaWindow retains a refernece to mGlView, meaning that the
         // reference count on it is now two. We release our reference to
         // mGlView later when we are done with it.
         [mCocoaWindow setContentView:mGlView];
         [mCocoaWindow setInitialFirstResponder:mGlView];
      }
   }
   @catch (NSException* ex)
   {
      const char* exception_name   = [[ex name] UTF8String];
      const char* exception_reason = [[ex reason] UTF8String];
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": Failed to create Cocoa OpenGL window!\n" << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
         << exception_name << ": " << exception_reason << std::endl
         << vprDEBUG_FLUSH;

      // We do not need to release mMainView here because it is the same as
      // mGlView, and we did not retain an extra reference to it when
      // mMainView was asssigned.
      [mGlView release];
      [mCocoaWindow release];
      [mRenderLock release];

      mGlView      = nil;
      mMainView    = nil;
      mCocoaWindow = nil;
      mRenderLock  = nil;
   }

   return opened;
}

bool GlWindowCocoa::close()
{
   if ( mWindowIsOpen )
   {
      // This causes a release message to be sent to mCocoaWindow after it is
      // closed.
      [mCocoaWindow close];
      mWindowIsOpen = false;
   }

   [mGlView release];

   mGlView      = nil;
   mMainView    = nil;
   mCocoaWindow = nil;
   mVrjDisplay  = vrj::DisplayPtr();

   [mRenderLock release];
   mRenderLock = nil;

   if ( NULL != mThread )
   {
      delete mThread;
      mThread = NULL;
   }

   return true;
}

bool GlWindowCocoa::makeCurrent()
{
   if ( ! mWindowIsOpen )
   {
      return false;
   }

   [[mGlView openGLContext] makeCurrentContext];

   return true;
}

void GlWindowCocoa::configWindow(vrj::DisplayPtr displayWindow)
{
   vrj::GlWindow::configWindow(displayWindow);

   jccl::ConfigElementPtr display_elt = displayWindow->getConfigElement();
   gadget::InputArea::config(display_elt);

   mWindowName =
      [NSString stringWithUTF8String:display_elt->getName().c_str()];

   // Use +screens from NSScreen to allow opening of windows on different
   // screens.
   const int screen_num = displayWindow->getPipe();

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << "[vrj::GlWindowCocoa()] display_number = " << screen_num
      << std::endl << vprDEBUG_FLUSH;

   jccl::ConfigElementPtr disp_sys_elt =
      vrj::DisplayManager::instance()->getDisplaySystemElement();

   if ( screen_num >= 0 && screen_num < disp_sys_elt->getNum("pipes") )
   {
      mScreen = disp_sys_elt->getProperty<unsigned int>("pipes", screen_num);

      NSArray* screens = [NSScreen screens];
      if ( mScreen < 0 || mScreen >= [screens count] )
      {
         mScreen = 0;
      }

      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << "[vrj::GlWindowCocoa::config()] Screen: " << mScreen << std::endl
         << vprDEBUG_FLUSH;
   }

   // If we are opening the window full screen, then ignore whatever the
   // display window's configuration says about its dimensions and location.
   if ( displayWindow->isFullScreen() )
   {
      NSScreen* screen    = [[NSScreen screens] objectAtIndex:mScreen];
      NSRect screen_frame = [screen frame];
      mWindowWidth  = screen_frame.size.width;
      mWindowHeight = screen_frame.size.height;
      mOriginX = 0;
      mOriginY = 0;
   }
}

void GlWindowCocoa::swapBuffers()
{
   acquireRenderLock();
   GlWindow::swapBuffers();

//   [mGlView lockFocus];
   [[mGlView openGLContext] flushBuffer];
//   [mGlView unlockFocus];
   releaseRenderLock();
}

void GlWindowCocoa::acquireRenderLock()
{
   [mRenderLock lock];
}

void GlWindowCocoa::releaseRenderLock()
{
   [mRenderLock unlock];
}

void GlWindowCocoa::updateBounds(const float x, const float y,
                                 const float width, const float height)
{
   updateOriginAndSize(x, y, width, height);    // Input area update
   updateOriginSize(x, y, width, height);       // Graphics window update
   setDirtyViewport(true);                      // OpenGL viewport state
}

void GlWindowCocoa::setWindowOpen(const bool isOpen)
{
   mWindowIsOpen = isOpen;

   // It might not be a bad idea to set mCocoaWindow to nil here to avoid
   // crashes later if we should try to deliver a message to a deallocated
   // object.
}

void GlWindowCocoa::waitAndOpen()
{
   [sWinOpenLock lock];
   [sWinOpenLock unlock];

   finishOpen();
}

void GlWindowCocoa::finishOpen()
{
   // This causes the window to be opened.
   [mCocoaWindow makeKeyAndOrderFront:nil];

   mWindowIsOpen = true;
}

}
