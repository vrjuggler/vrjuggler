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

#import <Foundation/NSString.h>
#import <AppKit/NSWindow.h>
#import <AppKit/NSEvent.h>

#include <gadget/Type/KeyboardMouse/Keys.h>
#include <gadget/Devices/KeyboardMouseDevice/InputAreaCocoa.h>

#import <gadget/Devices/KeyboardMouseDevice/InputViewCocoa.h>


/**
 * Extend InputViewCocoa using the category PrivateMethods (just a convention,
 * not a keyword). The end result is that the methods declared here are
 * private.
 */
@interface InputViewCocoa (PrivateMethods)
   -(void) clearTrackingRect;
   -(void) resetTrackingRect;
@end

@implementation InputViewCocoa
   -(id) initWithFrame:(NSRect) frameRect
             inputArea:(gadget::InputAreaCocoa*) inputArea
   {
      mInputArea = inputArea;
      mTrackingRect = 0;
      return [self initWithFrame:frameRect];
   }

   /** @name NSResponder overrides */
   //@{
   /**
    * Indicates that instances of this class are willing and able to become
    * the first responder for the containing NSWindow instance.
    *
    * @return \c YES is always returned.
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
    * Removes the current tracking rectangle for this view before it gets
    * assigned to a new window.
    */
   -(void) viewWillMoveToWindow:(NSWindow*) newWindow
   {
      [super viewWillMoveToWindow:newWindow];
      [self clearTrackingRect];
   }

   /**
    * Creates a new tracking rectangle for this viwe after it gets assigned
    * to a new window.
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
            // XXX: Would it work better to use CGPostMouseEvent() here?
            [self mouseEntered:nil];
         }
      }
   }

   /**
    * Responds to the mouse entering the view area. The window is set to
    * accept all mouse move events while the mouse is within the view.
    */
   -(void) mouseEntered:(NSEvent*) theEvent
   {
      [[self window] setAcceptsMouseMovedEvents:YES];
      [super mouseEntered:theEvent];
   }

   /**
    * Responds to the mouse exiting the view area. The window is set to ignore
    * all mouse move events while the mouse is outside the view.
    */
   -(void) mouseExited:(NSEvent*) theEvent
   {
      [[self window] setAcceptsMouseMovedEvents:NO];
      [super mouseExited:theEvent];
   }

   /**
    * Responds to mouse movement.
    */
   -(void) mouseMoved:(NSEvent*) theEvent
   {
      mInputArea->addMouseMoveEvent(theEvent);
   }

   /**
    * Responds to mouse movement while the left button is pressed.
    */
   -(void) mouseDragged:(NSEvent*) theEvent
   {
      mInputArea->addMouseMoveEvent(theEvent);
   }

   /**
    * Responds to mouse left button press.
    */
   -(void) mouseDown:(NSEvent*) theEvent
   {
      mInputArea->addMouseButtonEvent(gadget::MBUTTON1,
                                      gadget::MouseButtonPressEvent,
                                      theEvent);
   }

   /**
    * Responds to mouse left button release.
    */
   -(void) mouseUp:(NSEvent*) theEvent
   {
      mInputArea->addMouseButtonEvent(gadget::MBUTTON1,
                                      gadget::MouseButtonReleaseEvent,
                                      theEvent);
   }

   /**
    * Responds to mouse movement while the right button is pressed.
    */
   -(void) rightMouseDragged:(NSEvent*) theEvent
   {
      mInputArea->addMouseMoveEvent(theEvent);
   }

   /**
    * Responds to mouse right button press.
    */
   -(void) rightMouseDown:(NSEvent*) theEvent
   {
      mInputArea->addMouseButtonEvent(gadget::MBUTTON2,
                                      gadget::MouseButtonPressEvent,
                                      theEvent);
   }

   /**
    * Responds to mouse right button release.
    */
   -(void) rightMouseUp:(NSEvent*) theEvent
   {
      mInputArea->addMouseButtonEvent(gadget::MBUTTON2,
                                      gadget::MouseButtonReleaseEvent,
                                      theEvent);
   }

   /**
    * Responds to mouse movement while a button other than the left or the
    * right is pressed.
    */
   -(void) otherMouseDragged:(NSEvent*) theEvent
   {
      mInputArea->addMouseMoveEvent(theEvent);
   }

   /**
    * Responds to mouse button press other than the right or the left.
    */
   -(void) otherMouseDown:(NSEvent*) theEvent
   {
      mInputArea->addMouseButtonEvent(gadget::MouseButtonPressEvent,
                                      theEvent);
   }

   /**
    * Responds to mouse button release other than the right or the left.
    */
   -(void) otherMouseUp:(NSEvent*) theEvent
   {
      mInputArea->addMouseButtonEvent(gadget::MouseButtonReleaseEvent,
                                      theEvent);
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
      mInputArea->addKeyEvent(gadget::KeyPressEvent, theEvent);
   }

   /**
    * Responds to a key release.
    */
   -(void) keyUp:(NSEvent*) theEvent
   {
      mInputArea->addKeyEvent(gadget::KeyReleaseEvent, theEvent);
   }

   /**
    * Responds to a change in the keyboard modifier flags.
    */
   -(void) flagsChanged:(NSEvent*) theEvent
   {
      mInputArea->flagsChanged(theEvent);
   }
   //@}

   /**
    * Invoked whenever the window containing this view is moved or resized.
    * The job of this method is to update the tracking rectangle for this
    * view.
    */
   -(void) resetCursorRects
   {
      [super resetCursorRects];

      // Clear the current tracking rectnagle and define a new one based on
      // our new bounds.
      [self resetTrackingRect];
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

      mTrackingRect = [self addTrackingRect:[self visibleRect]
                                      owner:self
                                   userData:NULL
                               assumeInside:NO];
   }
   //@}
@end
