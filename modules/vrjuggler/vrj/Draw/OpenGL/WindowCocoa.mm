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

#include <vrj/Draw/OpenGL/Config.h>

#include <boost/bind.hpp>

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Perf/ProfileManager.h>
#include <jccl/Config/ConfigElement.h>

#include <vrj/Display/DisplayManager.h>
#include <vrj/Util/Debug.h>
#import <vrj/Draw/OpenGL/VRJOpenGLView.h>
#include <vrj/Draw/OpenGL/WindowCocoa.h>

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
#import <OpenGL/gl.h>


/**
 * A simple subclass of NSWindow that overrides -canBecomeKeyWindow so that
 * it returns YES.
 */
@interface VRJCustomWindow : NSWindow
@end

@implementation VRJCustomWindow
   -(BOOL) canBecomeKeyWindow
   {
      return YES;
   }
@end

/**
 * An instance of this class is set as the delegate for the NSWindow object
 * assocated with each vrj::opengl::WindowCocoa instance.
 */
@interface VRJWindowDelegate : NSObject
{
   vrj::opengl::WindowCocoa* mWindow;
}

   -(id) initWithWindow:(vrj::opengl::WindowCocoa*) window;
@end

@implementation VRJWindowDelegate
   -(id) initWithWindow:(vrj::opengl::WindowCocoa*) window
   {
      mWindow = window;
      return [super init];
   }

   /**
    * Invoked when the Cocoa window is being closed. We tell our
    * vrj::opengl::WindowCocoa instance that it is being closed so that it
    * does not try to close itself again later. The big issue that this
    * handles is that the NSWindow instance is deallocated when the window
    * gets closed. Hence, the vrj::opengl::WindowCocoa instance cannot perform
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
      mWindow->updateBounds(b.origin.x, b.origin.y, b.size.width,
                            b.size.height);
   }
@end

namespace vrj
{

namespace opengl
{

WindowCocoa::WindowCocoa()
   : vrj::opengl::Window()
   , gadget::InputAreaCocoa()
   , mThread(NULL)
   , mScreen(0)
   , mWindowName(@"Cocoa OpenGL Window")
   , mRenderLock(nil)
   , mGlView(nil)
{
   /* Do nothing. */ ;
}

WindowCocoa::~WindowCocoa()
{
   if ( NULL != mThread )
   {
      delete mThread;
      mThread = NULL;
   }
}

bool WindowCocoa::open()
{
   if ( mWindowIsOpen )
   {
      return true;
   }

   if ( mWindowWidth == -1 )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR")
         << ": [vrj::opengl::WindowCocoa::open()] Window has not been "
         << "configured\n" << vprDEBUG_FLUSH;
      return false;
   }

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
      << "[vrj::opengl::WindowCocoa::open()] Open window\n" << vprDEBUG_FLUSH;

   mRenderLock = [[NSLock alloc] init];

   bool opened(false);

   @try
   {
      unsigned int style_mask(0);

      mLockXCenter = mWindowWidth / 2;
      mLockYCenter = mWindowHeight / 2;

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
         [[VRJCustomWindow alloc] initWithContentRect:content_rect
                                            styleMask:style_mask
                                              backing:NSBackingStoreBuffered
                                                defer:NO
                                               screen:screen];
      [mCocoaWindow setOneShot:YES];

      // Set the delegate for mCocoaWindow. In particular, we do this so that
      // we can be told if the window gets closed by some external entity.
      id delegate =
         [[[VRJWindowDelegate alloc] initWithWindow:this] autorelease];
      [mCocoaWindow setDelegate:delegate];

      [mCocoaWindow setTitle:mWindowName];
      [mCocoaWindow setPreservesContentDuringLiveResize:NO];
      [mCocoaWindow setAutodisplay:YES];

      const BOOL handle_input = NULL != mKeyboardMouseDevice;
      mGlView = [[VRJOpenGLView alloc] initWithFrame:content_rect
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

      // mCocoaWindow retains a refernece to mGlView, meaning that the
      // reference count on it is now two. We release our reference to
      // mGlView later when we are done with it.
      [mCocoaWindow setContentView:mGlView];
      [mCocoaWindow setInitialFirstResponder:mGlView];
       
      finishOpen();
      opened = true;
      makeCurrent();
      
      // We will decrement the reference count of mGlView so that mCocoaWindow
      // will have complete ownership of the NSOpenGLView memory.
      // http://developer.apple.com/library/mac/#documentation/cocoa/reference/ApplicationKit/Classes/NSWindow_Class/Reference/Reference.html
      [mGlView release];

      // TODO: Add extension loading stuff.
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

bool WindowCocoa::close()
{
   if ( mWindowIsOpen )
   {
      // This causes a release message to be sent to mCocoaWindow after it is
      // closed.
      [mCocoaWindow close];
      mWindowIsOpen = false;
   }

   // http://developer.apple.com/library/mac/#documentation/cocoa/Conceptual/AppArchitecture/Tasks/GracefulAppTermination.html%23//apple_ref/doc/uid/20001280
   [mCocoaWindow setDelegate:nil];
   [mCocoaWindow makeFirstResponder:nil];
   [mCocoaWindow setContentView:nil];

   mCocoaWindow = nil;
   mGlView      = nil;
   mMainView    = nil;
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

bool WindowCocoa::makeCurrent()
{
   if ( ! mWindowIsOpen )
   {
      return false;
   }

   [[mGlView openGLContext] makeCurrentContext];

   return true;
}

void WindowCocoa::configWindow(vrj::DisplayPtr displayWindow)
{
   vrj::opengl::Window::configWindow(displayWindow);

   jccl::ConfigElementPtr display_elt = displayWindow->getConfigElement();
   gadget::InputArea::config(display_elt);

   mWindowName =
      [NSString stringWithUTF8String:display_elt->getName().c_str()];

   // Use +screens from NSScreen to allow opening of windows on different
   // screens.
   const int screen_num = displayWindow->getPipe();

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << "[vrj::opengl::WindowCocoa()] display_number = " << screen_num
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
         << "[vrj::opengl::WindowCocoa::config()] Screen: " << mScreen
         << std::endl << vprDEBUG_FLUSH;
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

void WindowCocoa::swapBuffers()
{
   VPR_PROFILE_GUARD_HISTORY("WindowCocoa::swapBuffers", 10);
   vrj::opengl::Window::swapBuffers();

   //Swapbuffers is guarded by the render lock in vrj::opengl::Pipe::controlLoop
   [[mGlView openGLContext] flushBuffer];
}

void WindowCocoa::acquireRenderLock()
{
   //TODO : Figure out what to do when we cannot lock focus on a context
   //       See docs on NSView on the apple developer site for details
   BOOL canDraw = [mGlView lockFocusIfCanDraw];
}

void WindowCocoa::releaseRenderLock()
{
    if ([mGlView canDraw] && [NSView focusView] == mGlView)
    {
       [mGlView unlockFocus];
    }
}

void WindowCocoa::updateBounds(const float x, const float y, const float width,
                               const float height)
{
   acquireRenderLock();
   resize(width, height);                       // Input area update
   updateOriginSize(x, y, width, height);       // Graphics window update
   setDirtyViewport(true);                      // OpenGL viewport state
   releaseRenderLock();
}

void WindowCocoa::setWindowOpen(const bool isOpen)
{
   mWindowIsOpen = isOpen;
}

void WindowCocoa::waitAndOpen()
{
   [sWinOpenLock lock];
   [sWinOpenLock unlock];

   finishOpen();
}

void WindowCocoa::finishOpen()
{
   // This causes the window to be opened.
   [mCocoaWindow makeKeyAndOrderFront:nil];

   [mGlView setNeedsDisplay:YES];
    
   mWindowIsOpen = true;
}

}

}
