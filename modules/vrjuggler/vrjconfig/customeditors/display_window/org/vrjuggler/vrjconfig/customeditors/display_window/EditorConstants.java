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
   static final String RESOURCE_BASE =
      "org/vrjuggler/vrjconfig/customeditors/display_window";

   static final String IMAGE_BASE = RESOURCE_BASE + "/images";

   static final String ALIAS_TYPE = "alias";

   static final String ANALOG_PROXY_TYPE = "analog_proxy";

   static final String DIGITAL_PROXY_TYPE = "digital_proxy";

   static final String DISPLAY_WINDOW_TYPE = "display_window";

   static final String INPUT_AREA_TYPE = "input_area";

   static final String SIMULATOR_VIEWPORT_TYPE = "simulator_viewport";

   static final String SURFACE_VIEWPORT_TYPE = "surface_viewport";

   static final String INPUT_WINDOW_TYPE = "input_window";

   static final String KEYBOARD_MOUSE_TYPE = "keyboard_mouse_device";

   static final String KEYBOARD_MOUSE_PROXY_TYPE = "keyboard_mouse_proxy";

   static final String OPENGL_FRAME_BUFFER_TYPE = "opengl_frame_buffer_config";

   static final String POSITION_PROXY_TYPE = "position_proxy";

   static final String PROXY_TYPE = "proxy";

   static final String SIM_ANALOG_DEVICE_TYPE = "simulated_analog_device";

   static final String SIM_DIGITAL_DEVICE_TYPE = "simulated_digital_device";

   static final String SIM_POS_DEVICE_TYPE = "simulated_positional_device";

   static final String DEVICE_PROPERTY = "device";

   static final String KEYBOARD_MOUSE_PROXY_PTR_PROPERTY =
      "keyboard_mouse_proxy";

   static final String KEYBOARD_MOUSE_PTR_PROPERTY =
      "keyboard_mouse_device_name";

   static final String LOCK_KEY_PROPERTY = "lock_key";

   static final String ORIGIN_PROPERTY = "origin";

   static final String PROXY_PROPERTY = "proxy";

   static final String START_LOCKED_PROPERTY = "start_locked";

   static final String SLEEP_TIME_PROPERTY = "sleep_time";

   static final String SIZE_PROPERTY = "size";
}
