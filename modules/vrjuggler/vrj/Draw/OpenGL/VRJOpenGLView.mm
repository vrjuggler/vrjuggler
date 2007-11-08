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

#include <vrj/Draw/OpenGL/Config.h>

#include <vector>

#import <Foundation/NSException.h>
#import <Foundation/NSString.h>
#import <Foundation/NSDictionary.h>
#import <AppKit/NSCursor.h>
#import <AppKit/NSOpenGL.h>
#import <AppKit/NSScreen.h>
#import <AppKit/NSWindow.h>
#import <AGL/agl.h>

#include <jccl/Config/ConfigElement.h>
#include <vrj/Util/Debug.h>
#include <vrj/Draw/OpenGL/WindowCocoa.h>

#import <vrj/Draw/OpenGL/VRJOpenGLView.h>


@interface VRJOpenGLView (PrivateMethods)
   -(void) clearTrackingRect;
   -(void) resetTrackingRect;
   -(NSOpenGLPixelFormat*) createPixelFormat;
   -(CGDirectDisplayID) getDisplayID:(NSScreen*) screen;
@end

@implementation VRJOpenGLView
   // Designated initializer implementation.
   -(id) initWithFrame:(NSRect) frameRect
              glWindow:(vrj::opengl::WindowCocoa*) window
                screen:(NSScreen*) screen
         inputHandling:(BOOL) handleInput
   {
      vrj::DisplayPtr display = window->getDisplay();

      mVrjWindow        = window;
      mFullScreen       = display->isFullScreen();
      mDisplayID        = [self getDisplayID:screen];
      mOrigDisplayMode  = (NSDictionary*) CGDisplayCurrentMode(mDisplayID);
      mHandleInput      = handleInput;
      mShouldHideCursor = display->shouldHideMouse();
      mTrackingRect     = 0;

      [mOrigDisplayMode retain];

      NSOpenGLPixelFormat* pixel_format = [self createPixelFormat];

      if ( pixel_format == nil )
      {
         NSException* ex =
            [NSException exceptionWithName:@"PixelFormatCreationException"
                                    reason:@"Coult not create OpenGL pixel format"
                                  userInfo:nil];
         @throw ex;
      }

      GLint renderer_id;
      [pixel_format getValues:&renderer_id
                 forAttribute:NSOpenGLPFARendererID
             forVirtualScreen:0];

      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << "Visual ID: " << std::hex << renderer_id << std::dec << std::endl
         << vprDEBUG_FLUSH;

      self = [super initWithFrame:frameRect
                      pixelFormat:pixel_format];
      [pixel_format release];
      pixel_format = nil;

      if ( self && mFullScreen )
      {
         CGDisplayErr err = CGDisplayCapture(mDisplayID);

         if ( err != CGDisplayNoErr )
         {
            NSException* ex =
               [NSException exceptionWithName:@"DisplayCaptureException"
                                       reason:@"Coult not capture displays"
                                     userInfo:nil];
            @throw ex;
         }

         // This has the side effect of creating the OpenGL context if it did
         // not already exist.
         [[self openGLContext] setFullScreen];
      }

      return self;
   }

   -(void) dealloc
   {
      [mOrigDisplayMode release];
      [super dealloc];
   }

   -(void) reshape
   {
   }
/*
   -(void) reshape
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "VRJOpenGLView reshape" << std::endl << vprDEBUG_FLUSH;
      [self resetTrackingRect];
      [self clearTrackingRect];
//      NSRect bounds = [self bounds];
//      mVrjWindow->updateOriginSize(bounds.origin.x, bounds.origin.y,
//                                   bounds.size.width, bounds.size.height);
//      mVrjWindow->setDirtyViewport(true);
   }
*/
   /** @name NSResponder overrides */
   //@{
   /**
    * Indicates whether this object can take on the first responder role for
    * the containing window.
    *
    * @return YES is always returned.
    */
   -(BOOL) acceptsFirstResponder
   {
      return YES;
   }

   /**
    * Indicates that we accept the role of first responder when it is
    * assigned to us by the containing NSWindow instance.
    *
    * @return \c YES is always returned.
    */
   -(BOOL) becomeFirstResponder
   {
      return YES;
   }

   /**
    * Indicates that we allow some other entity to take on the role of first
    * responder for the containing NSWindow instance.
    *
    * @return \c YES is always returned.
    */
   -(BOOL) resignFirstResponder
   {
      return YES;
   }

   /**
    * Removes the tracking rectangle for this view (if it has one) before it
    * gets assigned to a new window.
    *
    * @post \c mTrackingRerct is 0.
    */
   -(void) viewWillMoveToWindow:(NSWindow*) newWindow
   {
      [super viewWillMoveToWindow:newWindow];
      [self clearTrackingRect];
   }

   /**
    * Adds a tracking rectangle to this view if it needs one for mouse cursor
    * hiding and/or input handling.
    */
   -(void) viewDidMoveToWindow
   {
      [super viewDidMoveToWindow];
      [self resetTrackingRect];

      // If this view was moved to a new window, determine if the mouse is
      // currently within the bounds of this view. If it is, then we post a
      // fake mouseEntered event to inform this view that the mouse is within
      // it. Without this, the user would have to move the mouse out of the
      // window and back in to get the mouseEntered event to be posted.
      if ( [self window] )
      {
         const NSPoint mouse_loc =
            [[self window] mouseLocationOutsideOfEventStream];
         const NSRect bounds = [self bounds];

         if ( [self mouse:mouse_loc inRect:bounds] )
         {
            [self mouseEntered:nil];
         }
      }
      // This view has been removed from its window, so we need to clean up
      // its state.
      else
      {
         [NSOpenGLContext clearCurrentContext];
         [[self openGLContext] clearDrawable];

         if ( mFullScreen )
         {
            CGDisplaySwitchToMode(mDisplayID,
                                  (CFDictionaryRef) mOrigDisplayMode);
            CGDisplayErr err = CGDisplayRelease(mDisplayID);

            if ( err != CGDisplayNoErr )
            {
               vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
                  << "VRJOpenGLView failed to release display "
                  << std::hex << mDisplayID << std::dec << std::endl
                  << vprDEBUG_FLUSH;
            }
         }
      }
   }

   /**
    * Ensures that the mouse cursor is visible when live resizing of this view
    * begins.
    */
   -(void) viewWillStartLiveResize
   {
      mVrjWindow->acquireRenderLock();

      [super viewWillStartLiveResize];

      if ( mShouldHideCursor )
      {
         [NSCursor unhide];
      }
   }

   -(void) viewDidEndLiveResize
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "VRJOpenGLView viewDidEndLiveResize" << std::endl
         << vprDEBUG_FLUSH;
      mVrjWindow->releaseRenderLock();
      [super viewDidEndLiveResize];
   }

   /**
    * Hides the mouse cursor when the mouse enters the tracking rectangle for
    * thsi view.
    */
   -(void) mouseEntered:(NSEvent*) theEvent
   {
      [[self window] setAcceptsMouseMovedEvents:mHandleInput];

      if ( mShouldHideCursor )
      {
         [NSCursor hide];
      }

      [super mouseEntered:theEvent];
   }

   /**
    * Un-hides the mouse cursor when the mouse leaves the tracking rectangle
    * for thsi view.
    */
   -(void) mouseExited:(NSEvent*) theEvent
   {
      if ( mHandleInput )
      {
         [[self window] setAcceptsMouseMovedEvents:NO];
      }

      if ( mShouldHideCursor )
      {
         [NSCursor unhide];
      }

      [super mouseExited:theEvent];
   }

   /**
    * Responds to mouse movement. This is only invoked if the mouse is inside
    * the tracking rectangle for this view and \c mHandleInput is YES.
    */
   -(void) mouseMoved:(NSEvent*) theEvent
   {
      mVrjWindow->addMouseMoveEvent(theEvent);
   }

   /**
    * Responds to mouse movement while the left button is pressed. This is
    * only invoked if the mouse is inside the tracking rectangle for this view
    * and \c mHandleInput is YES.
    */
   -(void) mouseDragged:(NSEvent*) theEvent
   {
      mVrjWindow->addMouseMoveEvent(theEvent);
   }

   /**
    * Responds to mouse left button press.
    */
   -(void) mouseDown:(NSEvent*) theEvent
   {
      if ( mHandleInput )
      {
         mVrjWindow->addMouseButtonEvent(gadget::MBUTTON1,
                                         gadget::MouseButtonPressEvent,
                                         theEvent);
      }
      else
      {
         [super mouseDown:theEvent];
      }
   }

   /**
    * Responds to mouse left button release.
    */
   -(void) mouseUp:(NSEvent*) theEvent
   {
      if ( mHandleInput )
      {
         mVrjWindow->addMouseButtonEvent(gadget::MBUTTON1,
                                         gadget::MouseButtonReleaseEvent,
                                         theEvent);
      }
      else
      {
         [super mouseUp:theEvent];
      }
   }

   /**
    * Responds to mouse movement while the right button is pressed. This is
    * only invoked if the mouse is inside the tracking rectangle for this view
    * and \c mHandleInput is YES.
    */
   -(void) rightMouseDragged:(NSEvent*) theEvent
   {
      mVrjWindow->addMouseMoveEvent(theEvent);
   }

   /**
    * Responds to mouse right button press.
    */
   -(void) rightMouseDown:(NSEvent*) theEvent
   {
      if ( mHandleInput )
      {
         mVrjWindow->addMouseButtonEvent(gadget::MBUTTON2,
                                         gadget::MouseButtonPressEvent,
                                         theEvent);
      }
      else
      {
         [super rightMouseDown:theEvent];
      }
   }

   /**
    * Responds to mouse right button release.
    */
   -(void) rightMouseUp:(NSEvent*) theEvent
   {
      if ( mHandleInput )
      {
         mVrjWindow->addMouseButtonEvent(gadget::MBUTTON2,
                                         gadget::MouseButtonReleaseEvent,
                                         theEvent);
      }
      else
      {
         [super rightMouseUp:theEvent];
      }
   }

   /**
    * Responds to mouse movement while a button other than the left or the
    * right is pressed. This is only invoked if the mouse is inside the
    * tracking rectangle for this view and \c mHandleInput is YES.
    */
   -(void) otherMouseDragged:(NSEvent*) theEvent
   {
      mVrjWindow->addMouseMoveEvent(theEvent);
   }

   /**
    * Responds to mouse button press other than the right or the left.
    */
   -(void) otherMouseDown:(NSEvent*) theEvent
   {
      if ( mHandleInput )
      {
         mVrjWindow->addMouseButtonEvent(gadget::MouseButtonPressEvent,
                                         theEvent);
      }
      else
      {
         [super otherMouseDown:theEvent];
      }
   }

   /**
    * Responds to mouse button release other than the right or the left.
    */
   -(void) otherMouseUp:(NSEvent*) theEvent
   {
      if ( mHandleInput )
      {
         mVrjWindow->addMouseButtonEvent(gadget::MouseButtonReleaseEvent,
                                         theEvent);
      }
      else
      {
         [super otherMouseUp:theEvent];
      }
   }

   /**
    * Responds to scroll wheel use.
    */
   -(void) scrollWheel:(NSEvent*) theEvent
   {
      // Add a mouse button event.
   }

   /**
    * Responds to a key press.
    */
   -(void) keyDown:(NSEvent*) theEvent
   {
      if ( mHandleInput )
      {
         mVrjWindow->addKeyEvent(gadget::KeyPressEvent, theEvent);
      }
      else
      {
         [super keyDown:theEvent];
      }
   }

   /**
    * Responds to a key release.
    */
   -(void) keyUp:(NSEvent*) theEvent
   {
      if ( mHandleInput )
      {
         mVrjWindow->addKeyEvent(gadget::KeyReleaseEvent, theEvent);
      }
      else
      {
         [super keyUp:theEvent];
      }
   }

   /**
    * Responds to a change in the keyboard modifier flags.
    */
   -(void) flagsChanged:(NSEvent*) theEvent
   {
      if ( mHandleInput )
      {
         mVrjWindow->flagsChanged(theEvent);
      }
      else
      {
         [super flagsChanged:theEvent];
      }
   }
   //@}

   /**
    * Invoked whenever the window containing this view is moved or resized.
    * The job of this method is to update the tracking rectangle for this
    * view.
    */
   -(void) resetCursorRects
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "VRJOpenGLView resetCursorRects" << std::endl << vprDEBUG_FLUSH;
//      mVrjWindow->acquireRenderLock();
      [super resetCursorRects];

      // Clear the current tracking rectnagle and define a new one based on
      // our new bounds.
      [self resetTrackingRect];

      NSRect bounds = [self bounds];
      mVrjWindow->updateOriginSize(bounds.origin.x, bounds.origin.y,
                                   bounds.size.width, bounds.size.height);
//      mVrjWindow->releaseRenderLock();
      mVrjWindow->setDirtyViewport(true);
   }

   /** @name Private Methods */
   //@{
   /**
    * Removes the current tracking rectangle for this view (if it has one).
    *
    * @post This view has no tracking rectangle associated with it.
    *       \c mTrackingRect is 0.
    */
   -(void) clearTrackingRect
   {
      if ( [self window] && mTrackingRect > 0 )
      {
         [self removeTrackingRect:mTrackingRect];
         mTrackingRect = 0;
      }
   }

   /**
    * Sets a new tracking rectangle for this view. If this view already has a
    * tracking rectangle, the existing one is removed first.
    *
    * @post This view has a tracking rectangle whose ID is stored in
    *       \c mTrackingRect.
    */
   -(void) resetTrackingRect
   {
      [self clearTrackingRect];

      // If the mouse pointer should be hidden when inside this view or we are
      // handling input, then we create a tracking rectangle that is the size
      // of this view. Then, the NSResponder overrides of -mouseEntered: and
      // -mouseExited: will be invoked.
      if ( mShouldHideCursor || mHandleInput )
      {
         mTrackingRect = [self addTrackingRect:[self visibleRect]
                                         owner:self
                                      userData:NULL
                                  assumeInside:NO];
      }
   }

   /**
    * Attempts to create an NSOpenGLPixelFormat object meeting the
    * requirements of the OpenGL frame buffer configuration held by the
    * vrj::Display object associated with \c mVrjWindow.
    */
   -(NSOpenGLPixelFormat*) createPixelFormat
   {
      vrj::DisplayPtr display = mVrjWindow->getDisplay();
      jccl::ConfigElementPtr gl_fb_elt = display->getGlFrameBufferConfig();

      int visual_id(-1);
      int red_size(1), green_size(1), blue_size(1), alpha_size(1), db_size(1),
          accum_red_size(1), accum_green_size(1), accum_blue_size(1),
          accum_alpha_size(1), stencil_size(1);
      int num_aux_bufs(0);
      bool enable_multisamp(false);
      int num_sample_bufs(1);
      int num_samples(2);

      if ( gl_fb_elt.get() != NULL )
      {
         if ( gl_fb_elt->getVersion() < 3 )
         {
            vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
               << clrOutBOLD(clrYELLOW, "WARNING:") << " Display window '"
               << display->getName() << "'" << std::endl;
            vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
               << "has an out of date OpenGL frame buffer configuration.\n";
            vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
               << "Expected version 3 but found version "
               << gl_fb_elt->getVersion() << ".  Default values\n";
            vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
               << "will be used for some frame buffer settings.\n"
               << vprDEBUG_FLUSH;
         }

         visual_id        = gl_fb_elt->getProperty<int>("visual_id");
         red_size         = gl_fb_elt->getProperty<int>("red_size");
         green_size       = gl_fb_elt->getProperty<int>("green_size");
         blue_size        = gl_fb_elt->getProperty<int>("blue_size");
         alpha_size       = gl_fb_elt->getProperty<int>("alpha_size");
         num_aux_bufs     = gl_fb_elt->getProperty<int>("auxiliary_buffer_count");
         db_size          = gl_fb_elt->getProperty<int>("depth_buffer_size");
         stencil_size     = gl_fb_elt->getProperty<int>("stencil_buffer_size");
         accum_red_size   = gl_fb_elt->getProperty<int>("accum_red_size");
         accum_green_size = gl_fb_elt->getProperty<int>("accum_green_size");
         accum_blue_size  = gl_fb_elt->getProperty<int>("accum_blue_size");
         accum_alpha_size = gl_fb_elt->getProperty<int>("accum_alpha_size");
         num_sample_bufs  = gl_fb_elt->getProperty<int>("num_sample_buffers");
         num_samples      = gl_fb_elt->getProperty<int>("num_samples");

         enable_multisamp = num_sample_bufs > 0;
      }

      if ( red_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Color buffer red channel size was negative ("
            << red_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         red_size = 1;
      }

      if ( green_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Color buffer green channel size was negative ("
            << green_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         green_size = 1;
      }

      if ( blue_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Color buffer blue channel size was negative ("
            << blue_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         blue_size = 1;
      }

      if ( alpha_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Color buffer alpha channel size was negative ("
            << alpha_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         alpha_size = 1;
      }

      if ( num_aux_bufs < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Auxiliary buffer count was negative (" << num_aux_bufs
            << ").  Setting to 0.\n" << vprDEBUG_FLUSH;
         num_aux_bufs = 0;
      }

      if ( db_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Depth buffer size was negative (" << db_size
            << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         db_size = 1;
      }

      if ( stencil_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Stencil buffer size was negative (" << stencil_size
            << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         stencil_size = 1;
      }

      if ( accum_red_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Accumulation buffer red channel size was negative ("
            << accum_red_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_red_size = 1;
      }

      if ( accum_green_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Accumulation buffer green channel size was negative ("
            << accum_green_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_green_size = 1;
      }

      if ( accum_blue_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Accumulation buffer blue channel size was negative ("
            << accum_blue_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_blue_size = 1;
      }

      if ( accum_alpha_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Accumulation buffer alpha channel size was negative ("
            << accum_alpha_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_alpha_size = 1;
      }

      std::vector<NSOpenGLPixelFormatAttribute> attr_vec;

      if ( mFullScreen )
      {
         attr_vec.push_back(NSOpenGLPFAFullScreen);
         attr_vec.push_back(NSOpenGLPFAScreenMask);
         attr_vec.push_back(
            static_cast<NSOpenGLPixelFormatAttribute>(
               CGDisplayIDToOpenGLDisplayMask(mDisplayID)
            )
         );
         attr_vec.push_back(NSOpenGLPFAAllRenderers);
      }
      else
      {
         attr_vec.push_back(NSOpenGLPFANoRecovery);
      }

      attr_vec.push_back(NSOpenGLPFADoubleBuffer);
      attr_vec.push_back(NSOpenGLPFAAccelerated);
      attr_vec.push_back(NSOpenGLPFAColorSize);
      attr_vec.push_back(
         static_cast<NSOpenGLPixelFormatAttribute>(red_size + green_size + blue_size)
      );

      const unsigned int alpha_attrib_index = attr_vec.size();
      attr_vec.push_back(NSOpenGLPFAAlphaSize);
      attr_vec.push_back(
         static_cast<NSOpenGLPixelFormatAttribute>(alpha_size)
      );

      attr_vec.push_back(NSOpenGLPFAAuxBuffers);
      attr_vec.push_back(
         static_cast<NSOpenGLPixelFormatAttribute>(num_aux_bufs)
      );
      attr_vec.push_back(NSOpenGLPFAStencilSize);
      attr_vec.push_back(
         static_cast<NSOpenGLPixelFormatAttribute>(stencil_size)
      );

      const unsigned int accum_attrib_index = attr_vec.size();
      attr_vec.push_back(NSOpenGLPFAAccumSize);
      attr_vec.push_back(
         static_cast<NSOpenGLPixelFormatAttribute>(accum_red_size +
                                                      accum_green_size +
                                                      accum_blue_size)
      );
      // What about accum_alpha_size?

      const unsigned int stereo_attrib_index = attr_vec.size();
      const bool want_stereo(display->isStereoRequested());
      if ( want_stereo )
      {
         attr_vec.push_back(NSOpenGLPFAStereo);
         mVrjWindow->setInStereo(true);
      }
      else
      {
         mVrjWindow->setInStereo(false);
      }

      const unsigned int multisamp_attrib_index = attr_vec.size();
      const unsigned int multisamp_buffers_attrib_index =
         multisamp_attrib_index + 1;
      const unsigned int multisamp_samples_attrib_index =
         multisamp_attrib_index + 3;

      if ( enable_multisamp )
      {
         if ( num_sample_bufs < 0 )
         {
            vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
               << clrOutBOLD(clrYELLOW, "WARNING")
               << ": Number of multisampling buffers was negative ("
               << num_sample_bufs << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
            num_sample_bufs = 1;
         }

         if ( num_samples < 0 )
         {
            vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
               << clrOutBOLD(clrYELLOW, "WARNING")
               << ": Number of samples for multisampling was negative ("
               << num_samples << ").  Setting to 2.\n" << vprDEBUG_FLUSH;
            num_samples = 2;
         }

         attr_vec.push_back(NSOpenGLPFASampleBuffers);
         attr_vec.push_back(
            static_cast<NSOpenGLPixelFormatAttribute>(num_sample_bufs)
         );
         attr_vec.push_back(NSOpenGLPFASamples);
         attr_vec.push_back(
            static_cast<NSOpenGLPixelFormatAttribute>(num_samples)
         );

         // Mac OS X 10.4 and newer have the NSOpenGLPFAMultisample hint.
#if VPR_OS_RELEASE_MAJOR >= 8
         attr_vec.push_back(NSOpenGLPFAMultisample);
         attr_vec.push_back(static_cast<NSOpenGLPixelFormatAttribute>(YES));
#endif
      }

      attr_vec.push_back(static_cast<NSOpenGLPixelFormatAttribute>(0));
      NSOpenGLPixelFormatAttribute* attrs = &attr_vec[0];

      NSOpenGLPixelFormat* pixel_format =
         [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];

      if ( pixel_format == nil )
      {
         // Try reducing or disabling multisampling if it was enabled.
         if ( enable_multisamp )
         {
            const int orig_num_samples(num_samples);

            // This process reduces the number of sample buffers and samples
            // per buffer until we get something that works or we conclude
            // that multisampling simply isn't available. It begins by
            // reducing the number of samples per buffer for the current
            // number of sample buffers. If that fails, then we reduce the
            // number of sample buffers by one and start over with the
            // original number of samples per buffer. If we do not find a
            // working visual once we get to one sample buffer and one sample
            // per buffer, then we disable multisampling altogether.
            // TODO: The logic employed in this loop seems far messier than it
            // ought to be. It would be nice if someone came up with a way to
            // clean it up and/or simplify it.
            do
            {
               // We do not want to have zero samples per buffer.
               while ( num_samples > 1 )
               {
                  vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
                     << clrOutBOLD(clrYELLOW, "WARNING")
                     << ": Could not get a valid pixel format with\n";
                  vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
                     << "         " << num_sample_bufs
                     << " sample buffer" << (num_sample_bufs > 1 ? "s" : "")
                     << " and " << num_samples << " sample"
                     << (num_samples > 1 ? "s" : "") << "per buffer.\n";
                  vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
                     << "         Trying with " << num_samples - 1
                     << " sample" << (num_samples - 1 > 1 ? "s" : "" )
                     << ".\n" << vprDEBUG_FLUSH;

                  // Reduce the numbmer of samples per buffer by one and try
                  // to get a visual.
                  --num_samples;
                  attr_vec[multisamp_samples_attrib_index] =
                     static_cast<NSOpenGLPixelFormatAttribute>(num_samples);

                  NSOpenGLPixelFormat* pixel_format =
                     [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];

                  if ( pixel_format != nil )
                  {
                     return pixel_format;
                  }
               }

               // If we are currently trying only one sample buffer, then we
               // are done trying to back off the multisampling requirements.
               if ( num_sample_bufs == 1 )
               {
                  break;
               }

               vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
                  << clrOutBOLD(clrYELLOW, "WARNING")
                  << ": Could not get a valid pixel format with\n";
               vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
                  << "         " << num_sample_bufs << " sample buffer"
                  << (num_sample_bufs > 1 ? "s" : "") << " and "
                  << num_samples << " sample" << (num_samples > 1 ? "s" : "")
                  << "per buffer.\n";
               vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
                  << "         Trying with " << num_sample_bufs - 1
                  << " sample buffer" << (num_sample_bufs - 1 > 1 ? "s" : "")
                  << " and " << orig_num_samples << " sample"
                  << (orig_num_samples > 1 ? "s" : "") << ".\n"
                  << vprDEBUG_FLUSH;

               // Restore the number of samples per buffer originally requested
               // and try reducing the number of sample buffers.
               num_samples = orig_num_samples;
               attrs[multisamp_samples_attrib_index] =
                  static_cast<NSOpenGLPixelFormatAttribute>(num_samples);
               --num_sample_bufs;
               attrs[multisamp_buffers_attrib_index] =
                  static_cast<NSOpenGLPixelFormatAttribute>(num_sample_bufs);

               NSOpenGLPixelFormat* pixel_format =
                  [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];

               if ( pixel_format != nil )
               {
                  return pixel_format;
               }
            }
            while ( num_sample_bufs >= 1 );

            vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
               << clrOutBOLD(clrYELLOW, "WARNING")
               << ": Could not get a valid pixel format with\n"
               << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
               << "         multisampling.  Trying without it.\n"
               << vprDEBUG_FLUSH;

            // Set the number sample buffers and samples per buffer to 0.
            attr_vec[multisamp_buffers_attrib_index] =
               static_cast<NSOpenGLPixelFormatAttribute>(0);
            attr_vec[multisamp_samples_attrib_index] =
               static_cast<NSOpenGLPixelFormatAttribute>(0);

            // Mac OS X 10.4 and newer have the NSOpenGLPFAMultisample hint,
            // and we need to disable it.
#if VPR_OS_RELEASE_MAJOR >= 8
            attr_vec[multisamp_attrib_index + 5] =
               static_cast<NSOpenGLPixelFormatAttribute>(NO);
#endif

            NSOpenGLPixelFormat* pixel_format =
               [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];

            if ( pixel_format != nil )
            {
               return pixel_format;
            }
         }

         if ( want_stereo )
         {
            vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
               << "WARNING: Could not get a valid pixel format" << std::endl;
            vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
               << "with stereo rendering enabled; trying without.\n"
               << vprDEBUG_FLUSH;

            mVrjWindow->setInStereo(false);
            attr_vec[stereo_attrib_index] =
               static_cast<NSOpenGLPixelFormatAttribute>(0);

            pixel_format =
               [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];

            if ( pixel_format != nil )
            {
               return pixel_format;
            }

            // Stereo must not have been the problem, so re-enable it.
            attr_vec[stereo_attrib_index] =
               static_cast<NSOpenGLPixelFormatAttribute>(NSOpenGLPFAStereo);
            mVrjWindow->setInStereo(true);
         }

         // If we reached this point, we still do not have a usable pixel
         // format. Disabling the alpha channel may give us something usable.
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
            << "WARNING: Could not get an OpenGL visual with a color buffer\n";
         vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
            << "         alpha channel; trying without.\n" << vprDEBUG_FLUSH;

         attr_vec[alpha_attrib_index] =
            static_cast<NSOpenGLPixelFormatAttribute>(0);

         pixel_format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];

         if ( pixel_format != nil )
         {
            return pixel_format;
         }
      }

      return pixel_format;
   }

   -(CGDirectDisplayID) getDisplayID:(NSScreen*) screen
   {
      NSNumber* screen_num =
         [[screen deviceDescription] objectForKey:@"NSScreeNumber"];

      // NOTE: A C-style cast is being used here because this is Objective-C
      // and because CGDirectDisplayID is defined differently on Mac OS X 10.5
      // than on earlier platforms. A generic C-style cast captures that more
      // easily than using an #ifdef.
      return (CGDirectDisplayID) [screen_num longValue];
   }
   //@}
@end
