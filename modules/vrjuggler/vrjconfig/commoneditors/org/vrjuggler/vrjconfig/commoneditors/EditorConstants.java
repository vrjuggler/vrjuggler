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

package org.vrjuggler.vrjconfig.commoneditors;


/**
 * String constants used for looking up config elements, config definitions,
 * and properties.
 */
public interface EditorConstants
{
   // Config element types (definition tokens).

   /** Config element type alias. */
   public static final String ALIAS_TYPE = "alias";

   /** Config element type analog_proxy. */
   public static final String ANALOG_PROXY_TYPE = "analog_proxy";

   /** Config element type digital_proxy. */
   public static final String DIGITAL_PROXY_TYPE = "digital_proxy";

   /** Config element type display_window. */
   public static final String DISPLAY_WINDOW_TYPE = "display_window";

   /** Config element type input_area. */
   public static final String INPUT_AREA_TYPE = "input_area";

   /** Config element type input_window. */
   public static final String INPUT_WINDOW_TYPE = "input_window";

   /** Config element type keyboard_mouse_device. */
   public static final String KEYBOARD_MOUSE_TYPE = "keyboard_mouse_device";

   /** Config element type keyboard_mouse_proxy. */
   public static final String KEYBOARD_MOUSE_PROXY_TYPE =
      "keyboard_mouse_proxy";

   /** Config element type opengl_frame_buffer_config. */
   public static final String OPENGL_FRAME_BUFFER_TYPE =
      "opengl_frame_buffer_config";

   /** Config element type position_proxy. */
   public static final String POSITION_PROXY_TYPE = "position_proxy";

   /** Config element type proxy. */
   public static final String PROXY_TYPE = "proxy";

   /** Config element type simulated_analog_device. */
   public static final String SIM_ANALOG_DEVICE_TYPE =
      "simulated_analog_device";

   /** Config element type simulated_digital_device. */
   public static final String SIM_DIGITAL_DEVICE_TYPE =
      "simulated_digital_device";

   /** Config element type simulated_positional_device. */
   public static final String SIM_POS_DEVICE_TYPE =
      "simulated_positional_device";

   /** Config element type simulated_relative_position. */
   public static final String SIM_RELATIVE_POS_DEVICE_TYPE =
      "simulated_relative_position";

   /** Config element type simulator_viewport. */
   public static final String SIMULATOR_VIEWPORT_TYPE = "simulator_viewport";

   /** Config element type surface_viewport. */
   public static final String SURFACE_VIEWPORT_TYPE = "surface_viewport";

   // Properties.

   /** Property type custom_scale. */
   public static final String CUSTOM_SCALE_PROPERTY = "custom_scale";

   /** Property type device. */
   public static final String DEVICE_PROPERTY = "device";

   /** Property type device_units. */
   public static final String DEVICE_UNITS_PROPERTY = "device_units";

   /** Property type keyboard_mouse_proxy. */
   public static final String KEYBOARD_MOUSE_PROXY_PTR_PROPERTY =
      "keyboard_mouse_proxy";

   /** Property type keyboard_mouse_device_name. */
   public static final String KEYBOARD_MOUSE_PTR_PROPERTY =
      "keyboard_mouse_device_name";

   /** Property type lock_key. */
   public static final String LOCK_KEY_PROPERTY = "lock_key";

   /** Property type origin. */
   public static final String ORIGIN_PROPERTY = "origin";

   /** Property type position_filters. */
   public static final String POSITION_FILTERS_PROPERTY = "position_filters";

   /** Property type position_transform_filter. */
   public static final String POSITION_TRANSFORM_FILTER_TYPE =
      "position_transform_filter";

   /** Property type proxy. */
   public static final String PROXY_PROPERTY = "proxy";

   /** Property type start_locked. */
   public static final String START_LOCKED_PROPERTY = "start_locked";

   /** Property type sleep_time. */
   public static final String SLEEP_TIME_PROPERTY = "sleep_time";

   /** Property type size. */
   public static final String SIZE_PROPERTY = "size";

   /** Property type pre_rotation. */
   public static final String PRE_ROTATION_PROPERTY = "pre_rotation";

   /** Property type pre_translation. */
   public static final String PRE_TRANSLATION_PROPERTY = "pre_translate";

   /** Property type post_rotation. */
   public static final String POST_ROTATION_PROPERTY = "post_rotation";

   /** Property type post_translation. */
   public static final String POST_TRANSLATION_PROPERTY = "post_translate";
}
