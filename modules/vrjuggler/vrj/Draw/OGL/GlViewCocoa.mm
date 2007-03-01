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

#include <vrj/Draw/OGL/GlWindowCocoa.h>

#include <vector>

#import <Foundation/NSException.h>
#import <Foundation/NSString.h>
#import <Foundation/NSDictionary.h>
#import <AppKit/NSCursor.h>
#import <AppKit/NSOpenGL.h>
#import <AppKit/NSScreen.h>
#import <AGL/agl.h>

#include <jccl/Config/ConfigElement.h>
#include <vrj/Util/Debug.h>

#import <vrj/Draw/OGL/GlViewCocoa.h>


@interface GlViewCocoa (PrivateMethods)
   -(void) clearTrackingRect;
   -(void) resetTrackingRect;
   -(NSOpenGLPixelFormat*) createPixelFormat:(NSScreen*) screen;
@end

@implementation GlViewCocoa
   // Designated initializer implementation.
   -(id) initWithFrame:(NSRect) frameRect
              glWindow:(vrj::GlWindowCocoa*) window
                screen:(NSScreen*) screen
   {
      mVrjWindow = window;
      mTrackingRect = 0;

      NSOpenGLPixelFormat* pixel_format = [self createPixelFormat:screen];

      if ( pixel_format == nil )
      {
         NSException* ex =
            [NSException exceptionWithName:@"PixelFormatCreationException"
                                    reason:@"Coult not create OpenGL pixel format"
                                  userInfo:nil];
         @throw ex;
      }

      long renderer_id;
      [pixel_format getValues:&renderer_id
                 forAttribute:NSOpenGLPFARendererID
             forVirtualScreen:0];

      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << "Visual ID: " << std::hex << renderer_id << std::dec
         << std::endl << vprDEBUG_FLUSH;

      self = [super initWithFrame:frameRect
                      pixelFormat:pixel_format];

      if ( self && mVrjWindow->getDisplay()->isFullScreen() )
      {
         [[self openGLContext] setFullScreen];
      }

      return self;
   }
/*
   -(void) reshape
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "GlViewCocoa reshape" << std::endl << vprDEBUG_FLUSH;
      [self resetTrackingRect];
      [self clearTrackingRect];
//      NSRect bounds = [self bounds];
//      mVrjWindow->updateOriginSize(bounds.origin.x, bounds.origin.y,
//                                   bounds.size.width, bounds.size.height);
   }
*/
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
    * Removes the tracking rectangle for this view if it has one.
    */
   -(void) viewWillMoveToWindow:(NSWindow*) newWindow
   {
      [super viewWillMoveToWindow:newWindow];
      [self clearTrackingRect];
   }

   /**
    * Adds a tracking rectangle to this view if it needs one for mouse cursor
    * hiding.
    */
   -(void) viewDidMoveToWindow
   {
      [super viewDidMoveToWindow];
      [self resetTrackingRect];
   }

   /**
    * Ensures that the mouse cursor is visible when live resizing of this view
    * begins.
    */
   -(void) viewWillStartLiveResize
   {
      [super viewWillStartLiveResize];

      if ( mVrjWindow->getDisplay()->shouldHideMouse() )
      {
         [NSCursor unhide];
      }
   }

   /**
    * Hides the mouse cursor when the mouse enters the tracking rectangle for
    * thsi view.
    */
   -(void) mouseEntered:(NSEvent*) theEvent
   {
      [NSCursor hide];
      [super mouseEntered:theEvent];
   }

   /**
    * Un-hides the mouse cursor when the mouse leaves the tracking rectangle
    * for thsi view.
    */
   -(void) mouseExited:(NSEvent*) theEvent
   {
      [NSCursor unhide];
      [super mouseExited:theEvent];
   }

   -(void) resetCursorRects
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "GlViewCocoa resetCursorRects" << std::endl << vprDEBUG_FLUSH;
      [super resetCursorRects];
      [self resetTrackingRect];
//      NSRect bounds = [self bounds];
//      mVrjWindow->updateOriginSize(bounds.origin.x, bounds.origin.y,
//                                   bounds.size.width, bounds.size.height);
   }

   -(void) clearTrackingRect
   {
      if ( [self window] && mTrackingRect > 0 )
      {
         [self removeTrackingRect:mTrackingRect];
         mTrackingRect = 0;
      }
   }

   -(void) resetTrackingRect
   {
      [self clearTrackingRect];

      // If the mouse pointer should be hidden when inside this view, then we
      // create a tracking rectangle that is the size of this view. Then, the
      // NSResponder overrides of -mouseEntered: and -mouseExited: will be
      // invoked.
      if ( mVrjWindow->getDisplay()->shouldHideMouse() )
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
   -(NSOpenGLPixelFormat*) createPixelFormat:(NSScreen*) screen
   {
      vrj::Display* display = mVrjWindow->getDisplay();
      jccl::ConfigElementPtr gl_fb_elt = display->getGlFrameBufferConfig();

      int visual_id(-1);
      int red_size(1), green_size(1), blue_size(1), alpha_size(1), db_size(1),
          accum_red_size(1), accum_green_size(1), accum_blue_size(1),
          accum_alpha_size(1), stencil_size(1);
      int num_aux_bufs(0);
      BOOL want_fsaa(NO);

      if ( gl_fb_elt.get() != NULL )
      {
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
         want_fsaa        = gl_fb_elt->getProperty<bool>("fsaa_enable");
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

      const bool full_screen = display->isFullScreen();

      std::vector<NSOpenGLPixelFormatAttribute> attr_vec;

      if ( full_screen )
      {
         attr_vec.push_back(NSOpenGLPFAFullScreen);
         attr_vec.push_back(NSOpenGLPFAScreenMask);
         NSNumber* screen_num =
            [[screen deviceDescription] objectForKey:@"NSScreeNumber"];
         _CGDirectDisplayID* gl_id =
            reinterpret_cast<_CGDirectDisplayID*>([screen_num intValue]);
         attr_vec.push_back(
            static_cast<NSOpenGLPixelFormatAttribute>(
               CGDisplayIDToOpenGLDisplayMask(gl_id)
            )
         );
      }
      else
      {
         attr_vec.push_back(NSOpenGLPFANoRecovery);
      }

      attr_vec.push_back(NSOpenGLPFADoubleBuffer);
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

      attr_vec.push_back(static_cast<NSOpenGLPixelFormatAttribute>(0));
      NSOpenGLPixelFormatAttribute* attrs = &attr_vec[0];

      NSOpenGLPixelFormat* pixel_format =
         [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];

      if ( pixel_format == nil )
      {
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
      }

      return pixel_format;
   }
@end
