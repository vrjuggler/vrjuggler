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
   // Miscellaneous constants.

   /** Location of images in the CommonEditors distribution. */
   public static final String COMMON_IMG_ROOT =
      "org/vrjuggler/vrjconfig/commoneditors/images";

   // Config element types (definition tokens).

   /** Config element type alias. */
   public static final String ALIAS_TYPE = "alias";

   /** Config element type analog_device. */
   public static final String ANALOG_DEVICE_TYPE = "analog_device";

   /** Config element type analog_proxy. */
   public static final String ANALOG_PROXY_TYPE = "analog_proxy";

   /** Config element type command_device. */
   public static final String COMMAND_DEVICE_TYPE = "command_device";

   /** Config element type data_glove (5DT). */
   public static final String DATA_GLOVE_TYPE = "data_glove";

   /** Config element type default_simulator. */
   public static final String DEFAULT_SIMULATOR_TYPE = "default_simulator";

   /** Config element type digital_device. */
   public static final String DIGITAL_DEVICE_TYPE = "digital_device";

   /** Config element type digital_proxy. */
   public static final String DIGITAL_PROXY_TYPE = "digital_proxy";

   /** Config element type display_window. */
   public static final String DISPLAY_WINDOW_TYPE = "display_window";

   /** Config element type directx_joystick. */
   public static final String DIRECTX_JOYSTICK_TYPE = "directx_joystick";

   /** Config element type dtk. */
   public static final String DTK_TYPE = "dtk";

   /** Config element type fastrak. */
   public static final String FASTRAK_TYPE = "fastrak";

   /** Config element type flock. */
   public static final String FLOCK_TYPE = "flock";

   /** Config element type glove_device. */
   public static final String GLOVE_DEVICE_TYPE = "glove_device";

   /** Config element type ibox. */
   public static final String IBOX_TYPE = "ibox";

   /** Config element type input_area. */
   public static final String INPUT_AREA_TYPE = "input_area";

   /** Config element type input_device. */
   public static final String INPUT_DEVICE_TYPE = "input_device";

   /** Config element type input_window. */
   public static final String INPUT_WINDOW_TYPE = "input_window";

   /** Config element type intersense. */
   public static final String INTERSENSE_TYPE = "intersense";

   /** Config element type intersense_api. */
   public static final String INTERSENSE_API_TYPE = "intersense_api";

   /** Config element type intersense_station. */
   public static final String INTERSENSE_STATION_TYPE = "intersense_station";

   /** Config element type keyboard_mouse_device. */
   public static final String KEYBOARD_MOUSE_DEVICE_TYPE =
      "keyboard_mouse_device";

   /** Config element type keyboard_mouse_proxy. */
   public static final String KEYBOARD_MOUSE_PROXY_TYPE =
      "keyboard_mouse_proxy";

   /** Config element type key_modifier_pair. */
   public static final String KEY_MODIFIER_PAIR_TYPE = "key_modifier_pair";

   /** Config element type linux_joydev. */
   public static final String LINUX_JOYDEV_TYPE = "linux_joydev";

   /** Config element type motion_star. */
   public static final String MOTION_STAR_TYPE = "motion_star";

   /** Config element type opengl_frame_buffer_config. */
   public static final String OPENGL_FRAME_BUFFER_TYPE =
      "opengl_frame_buffer_config";

   /** Config element type pinch_glove. */
   public static final String PINCH_GLOVE_TYPE = "pinch_glove";

   /** Config element type positional_device. */
   public static final String POSITIONAL_DEVICE_TYPE = "positional_device";

   /** Config element type position_proxy. */
   public static final String POSITION_PROXY_TYPE = "position_proxy";

   /** Config element type proxy. */
   public static final String PROXY_TYPE = "proxy";

   /** Config element type puck_device. */
   public static final String PUCK_DEVICE_TYPE = "puck_device";

   /**
    * Config element type serial_encoder (US Digital Serial Encoder on a
    * Barco Baron).
    */
   public static final String SERIAL_ENCODER_TYPE = "serial_encoder";

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

   /** Config element type string_device. */
   public static final String STRING_DEVICE_TYPE = "string_device";

   /** Config element type surface_viewport. */
   public static final String SURFACE_VIEWPORT_TYPE = "surface_viewport";

   /** Config element type trackd_controller. */
   public static final String TRACKD_CONTROLLER_TYPE = "trackd_controller";

   /** Config element type trackd_sensor. */
   public static final String TRACKD_SENSOR_TYPE = "trackd_sensor";

   /** Config element type trackd_api_controller. */
   public static final String TRACKD_API_CONTROLLER_TYPE =
      "trackd_api_controller";

   /** Config element type trackd_api_sensor. */
   public static final String TRACKD_API_SENSOR_TYPE = "trackd_api_sensor";

   /** Config element type vrpn. */
   public static final String VRPN_TYPE = "vrpn";

   // Properties.

   /** Property type accum_red_size. */
   public static final String ACCUM_RED_SIZE_PROPERTY = "accum_red_size";

   /** Property type accum_green_size. */
   public static final String ACCUM_GREEN_SIZE_PROPERTY = "accum_green_size";

   /** Property type accum_blue_size. */
   public static final String ACCUM_BLUE_SIZE_PROPERTY = "accum_blue_size";

   /** Property type accum_alpha_size. */
   public static final String ACCUM_ALPHA_SIZE_PROPERTY = "accum_alpha_size";

   /** Property type active. */
   public static final String ACTIVE_PROPERTY = "active";

   /** Property type analog_count. */
   public static final String ANALOG_COUNT_PROPERTY = "analog_count";

   /** Property type analog_first. */
   public static final String ANALOG_FIRST_PROPERTY = "analog_first";

   /** Property type auxiliary_buffer_count. */
   public static final String AUX_BUFFER_COUNT_PROPERTY =
      "auxiliary_buffer_count";

   /** Property type border. */
   public static final String BORDER_PROPERTY = "border";

   /** Property type button_count. */
   public static final String BUTTON_COUNT_PROPERTY = "button_count";

   /** Property type camera_pos. */
   public static final String CAMERA_POS_PROPERTY = "camera_pos";

   /** Property type red_size. */
   public static final String COLOR_RED_SIZE_PROPERTY = "red_size";

   /** Property type green_size. */
   public static final String COLOR_GREEN_SIZE_PROPERTY = "green_size";

   /** Property type blue_size. */
   public static final String COLOR_BLUE_SIZE_PROPERTY = "blue_size";

   /** Property type alpha_size. */
   public static final String COLOR_ALPHA_SIZE_PROPERTY = "alpha_size";

   /** Property type custom_scale. */
   public static final String CUSTOM_SCALE_PROPERTY = "custom_scale";

   /** Property type decrement_keypress. */
   public static final String DECREMENT_KEYPRESS_PROPERTY =
      "decrement_keypress";

   /** Property type depth_buffer_size. */
   public static final String DEPTH_BUFFER_SIZE_PROPERTY = "depth_buffer_size";

   /** Property type device. */
   public static final String DEVICE_PROPERTY = "device";

   /** Property type device_units. */
   public static final String DEVICE_UNITS_PROPERTY = "device_units";

   /** Property type digital_count. */
   public static final String DIGITAL_COUNT_PROPERTY = "digital_count";

   /** Property type digital_first. */
   public static final String DIGITAL_FIRST_PROPERTY = "digital_first";

   /** Property type frame_buffer_config. */
   public static final String FRAME_BUFFER_PROPERTY = "frame_buffer_config";

   /** Property type fsaa_enable. */
   public static final String FSAA_ENABLE_PROPERTY = "fsaa_enable";

   /** Property type hide_mouse. */
   public static final String HIDE_MOUSE_PROPERTY = "hide_mouse";

   /** Property type increment_keypress. */
   public static final String INCREMENT_KEYPRESS_PROPERTY =
      "increment_keypress";

   /** Property type key_pair. */
   public static final String KEY_PAIR_PROPERTY = "key_pair";

   /** Property type key. */
   public static final String KEY_PROPERTY = "key";

   /** Property type keyboard_mouse_proxy. */
   public static final String KEYBOARD_MOUSE_PROXY_PTR_PROPERTY =
      "keyboard_mouse_proxy";

   /** Property type keyboard_mouse_device_name. */
   public static final String KEYBOARD_MOUSE_PTR_PROPERTY =
      "keyboard_mouse_device_name";

   /** Property type lock_key. */
   public static final String LOCK_KEY_PROPERTY = "lock_key";

   /** Property type lower_left_corner. */
   public static final String LOWER_LEFT_CORNER_PROPERTY = "lower_left_corner";

   /** Property type lower_right_corner. */
   public static final String LOWER_RIGHT_CORNER_PROPERTY =
      "lower_right_corner";

   /** Property type modifier_key. */
   public static final String MODIFIER_KEY_PROPERTY = "modifier_key";

   /** Property type origin. */
   public static final String ORIGIN_PROPERTY = "origin";

   /** Property type position_filters. */
   public static final String POSITION_FILTERS_PROPERTY = "position_filters";

   /** Property type position_transform_filter. */
   public static final String POSITION_TRANSFORM_FILTER_TYPE =
      "position_transform_filter";

   /** Property type pre_rotation. */
   public static final String PRE_ROTATION_PROPERTY = "pre_rotation";

   /** Property type pre_translation. */
   public static final String PRE_TRANSLATION_PROPERTY = "pre_translate";

   /** Property type post_rotation. */
   public static final String POST_ROTATION_PROPERTY = "post_rotation";

   /** Property type post_translation. */
   public static final String POST_TRANSLATION_PROPERTY = "post_translate";

   /** Property type proxy. */
   public static final String PROXY_PROPERTY = "proxy";

   /** Property type simulator_plugin. */
   public static final String SIMULATOR_PLUGIN_PROPERTY = "simulator_plugin";

   /** Property type simulator_viewports. */
   public static final String SIMULATOR_VIEWPORTS_PROPERTY =
      "simulator_viewports";

   /** Property type size. */
   public static final String SIZE_PROPERTY = "size";

   /** Property type sleep_time. */
   public static final String SLEEP_TIME_PROPERTY = "sleep_time";

   /** Property type stations. */
   public static final String STATIONS_PROPERTY = "stations";

   /** Property type start_locked. */
   public static final String START_LOCKED_PROPERTY = "start_locked";

   /** Property type stencil_buffer_size. */
   public static final String STENCIL_BUFFER_SIZE_PROPERTY =
      "stencil_buffer_size";

   /** Property type stereo. */
   public static final String STEREO_PROPERTY = "stereo";

   /** Property type surface_viewports. */
   public static final String SURFACE_VIEWPORTS_PROPERTY = "surface_viewports";

   /** Property type tracked. */
   public static final String TRACKED_PROPERTY = "tracked";

   /** Property type tracker_count. */
   public static final String TRACKER_COUNT_PROPERTY = "tracker_count";

   /** Property type tracker_proxy. */
   public static final String TRACKER_PROXY_PROPERTY = "tracker_proxy";

   /** Property type unit. */
   public static final String UNIT_PROPERTY = "unit";

   /** Property type upper_left_corner. */
   public static final String UPPER_LEFT_CORNER_PROPERTY = "upper_left_corner";

   /** Property type upper_right_corner. */
   public static final String UPPER_RIGHT_CORNER_PROPERTY =
      "upper_right_corner";

   /** Property type user. */
   public static final String USER_PROPERTY = "user";

   /** Property type use_analog. */
   public static final String USE_ANALOG_PROPERTY = "use_analog";

   /** Property type use_digital. */
   public static final String USE_DIGITAL_PROPERTY = "use_digital";

   /** Property type vertical_fov. */
   public static final String VERTICAL_FOV_PROPERTY = "vertical_fov";

   /** Property type view. */
   public static final String VIEW_PROPERTY = "view";

   /** Property type visual_id. */
   public static final String VISUAL_ID_PROPERTY = "visual_id";

   /** Property type wand_pos. */
   public static final String WAND_POS_PROPERTY = "wand_pos";
}
