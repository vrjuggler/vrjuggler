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

#import <AppKit/NSView.h>


namespace gadget
{

class InputAreaCocoa;

}

/**
 * A custom Cocoa view class used for dispatching keyboard and mouse events
 * to a gadget::InputAreaCocoa instance. This type should be used as the main
 * view for a window needing to deliver keyboard and mouse events to a
 * Gadgeteer input area. Subviews that do not need to handle input can be
 * added, though they probably should not try to become the first responder.
 *
 * @see gadget::InputAreaCocoa
 *
 * @since 1.1.25
 */
@interface InputViewCocoa : NSView
{
   gadget::InputAreaCocoa* mInputArea;  /**< The Gadgeteer input area */
   NSTrackingRectTag mTrackingRect;
}

   /**
    * The designated initializer for this class.
    *
    * @param frameFrect The frame rectangle for this view. In general, this
    *                   should be identifical to the frame rectangle used for
    *                   the containing NSWindow.
    * @param inputArea  The gadget::InputAreaCocoa instance to be associated
    *                   with this view.
    *
    * @return self
    */
   -(id) initWithFrame:(NSRect) frameRect
             inputArea:(gadget::InputAreaCocoa*) inputArea;
@end
