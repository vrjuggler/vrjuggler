/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.display_window;


interface EditorConstants
{
   static final String resourceBase =
      "org/vrjuggler/vrjconfig/customeditors/display_window";

   static final String imageBase = resourceBase + "/images";

   static final String inputAreaType = "input_area";

   static final String displayWindowType = "display_window";

   static final String simulatorViewportType = "simulator_viewport";

   static final String surfaceViewportType = "surface_viewport";

   static final String inputWindowType = "input_window";

   static final String keyboardMouseType = "keyboard_mouse_device";

   static final String keyboardMouseProxyType = "keyboard_mouse_proxy";

   static final String openglFrameBufferType = "opengl_frame_buffer_config";

   static final String simAnalogDeviceType = "simulated_analog_device";

   static final String simDigitalDeviceType = "simulated_digital_device";

   static final String simPosDeviceType = "simulated_positional_device";

   static final String keyboardMousePtrProperty = "keyboard_mouse_device_name";

   static final String lockKeyProperty = "lock_key";

   static final String startLockedProperty = "start_locked";

   static final String sleepTimeProperty = "sleep_time";

   static final String originProperty = "origin";

   static final String sizeProperty = "size";
}
