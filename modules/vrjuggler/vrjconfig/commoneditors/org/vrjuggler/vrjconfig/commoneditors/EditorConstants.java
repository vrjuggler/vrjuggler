/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

   /** Config element type <code>alias</code>. */
   public static final String ALIAS_TYPE = "alias";
   
   /** Config element type <code>user</code>. */
   public static final String USER_TYPE = "user";

   /** Config element type <code>analog_device</code>. */
   public static final String ANALOG_DEVICE_TYPE = "analog_device";

   /** Config element type <code>analog_proxy</code>. */
   public static final String ANALOG_PROXY_TYPE = "analog_proxy";

   /** Config element type <code>command_device</code>. */
   public static final String COMMAND_DEVICE_TYPE = "command_device";

   /** Config element type <code>data_glove (5DT)</code>. */
   public static final String DATA_GLOVE_TYPE = "data_glove";

   /** Config element type <code>default_simulator</code>. */
   public static final String DEFAULT_SIMULATOR_TYPE = "default_simulator";

   /** Config element type <code>digital_device</code>. */
   public static final String DIGITAL_DEVICE_TYPE = "digital_device";

   /** Config element type <code>digital_proxy</code>. */
   public static final String DIGITAL_PROXY_TYPE = "digital_proxy";

   /** Config element type <code>display_window</code>. */
   public static final String DISPLAY_WINDOW_TYPE = "display_window";

   /** Config element type <code>directx_joystick</code>. */
   public static final String DIRECTX_JOYSTICK_TYPE = "directx_joystick";

   /** Config element type <code>dtk</code>. */
   public static final String DTK_TYPE = "dtk";

   /** Config element type <code>fastrak</code>. */
   public static final String FASTRAK_TYPE = "fastrak";

   /** Config element type <code>flock</code>. */
   public static final String FLOCK_TYPE = "flock";

   /** Config element type <code>glove_device</code>. */
   public static final String GLOVE_DEVICE_TYPE = "glove_device";

   /** Config element type <code>ibox</code>. */
   public static final String IBOX_TYPE = "ibox";

   /** Config element type <code>input_area</code>. */
   public static final String INPUT_AREA_TYPE = "input_area";

   /** Config element type <code>input_device</code>. */
   public static final String INPUT_DEVICE_TYPE = "input_device";

   /** Config element type <code>input_window</code>. */
   public static final String INPUT_WINDOW_TYPE = "input_window";

   /** Config element type <code>intersense</code>. */
   public static final String INTERSENSE_TYPE = "intersense";

   /** Config element type <code>intersense_api</code>. */
   public static final String INTERSENSE_API_TYPE = "intersense_api";

   /** Config element type <code>intersense_station</code>. */
   public static final String INTERSENSE_STATION_TYPE = "intersense_station";

   /** Config element type <code>keyboard_mouse_device</code>. */
   public static final String KEYBOARD_MOUSE_DEVICE_TYPE =
      "keyboard_mouse_device";

   /** Config element type <code>keyboard_mouse_proxy</code>. */
   public static final String KEYBOARD_MOUSE_PROXY_TYPE =
      "keyboard_mouse_proxy";

   /** Config element type <code>key_modifier_pair</code>. */
   public static final String KEY_MODIFIER_PAIR_TYPE = "key_modifier_pair";

   /** Config element type <code>linux_joydev</code>. */
   public static final String LINUX_JOYDEV_TYPE = "linux_joydev";

   /** Config element type <code>motion_star</code>. */
   public static final String MOTION_STAR_TYPE = "motion_star";

   /** Config element type <code>opengl_frame_buffer_config</code>. */
   public static final String OPENGL_FRAME_BUFFER_TYPE =
      "opengl_frame_buffer_config";

   /** Config element type <code>pinch_glove</code>. */
   public static final String PINCH_GLOVE_TYPE = "pinch_glove";

   /** Config element type <code>positional_device</code>. */
   public static final String POSITIONAL_DEVICE_TYPE = "positional_device";

   /** Config element type <code>position_proxy</code>. */
   public static final String POSITION_PROXY_TYPE = "position_proxy";

   /** Config element type <code>proxy</code>. */
   public static final String PROXY_TYPE = "proxy";

   /** Config element type <code>puck_device</code>. */
   public static final String PUCK_DEVICE_TYPE = "puck_device";

   /**
    * Config element type <code>serial_encoder</code> (US Digital Serial
    * Encoder on a Barco Baron).
    */
   public static final String SERIAL_ENCODER_TYPE = "serial_encoder";

   /** Config element type <code>simulated_analog_device</code>. */
   public static final String SIM_ANALOG_DEVICE_TYPE =
      "simulated_analog_device";

   /** Config element type <code>simulated_digital_device</code>. */
   public static final String SIM_DIGITAL_DEVICE_TYPE =
      "simulated_digital_device";

   /** Config element type <code>simulated_positional_device</code>. */
   public static final String SIM_POS_DEVICE_TYPE =
      "simulated_positional_device";

   /** Config element type <code>simulated_relative_position</code>. */
   public static final String SIM_RELATIVE_POS_DEVICE_TYPE =
      "simulated_relative_position";

   /** Config element type <code>simulator_viewport</code>. */
   public static final String SIMULATOR_VIEWPORT_TYPE = "simulator_viewport";

   /** Config element type <code>string_device</code>. */
   public static final String STRING_DEVICE_TYPE = "string_device";

   /** Config element type <code>surface_viewport</code>. */
   public static final String SURFACE_VIEWPORT_TYPE = "surface_viewport";

   /** Config element type <code>trackd_controller</code>. */
   public static final String TRACKD_CONTROLLER_TYPE = "trackd_controller";

   /** Config element type <code>trackd_sensor</code>. */
   public static final String TRACKD_SENSOR_TYPE = "trackd_sensor";

   /** Config element type <code>trackd_api_controller</code>. */
   public static final String TRACKD_API_CONTROLLER_TYPE =
      "trackd_api_controller";

   /** Config element type <code>trackd_api_sensor</code>. */
   public static final String TRACKD_API_SENSOR_TYPE = "trackd_api_sensor";

   /** Config element type <code>vrpn</code>. */
   public static final String VRPN_TYPE = "vrpn";
   
   /** Config element type <code>cluster_node</code>. */
   public static final String CLUSTER_NODE_TYPE = "cluster_node";

   // Properties.

   /** Property type <code>accum_red_size</code>. */
   public static final String HEAD_POSITION_PROPERTY = "head_position";
   
   /** Property type <code>accum_red_size</code>. */
   public static final String INTEROCULAR_DISTANCE_PROPERTY = "interocular_distance";
   
   /** Property type <code>accum_red_size</code>. */
   public static final String ACCUM_RED_SIZE_PROPERTY = "accum_red_size";

   /** Property type <code>accum_green_size</code>. */
   public static final String ACCUM_GREEN_SIZE_PROPERTY = "accum_green_size";

   /** Property type <code>accum_blue_size</code>. */
   public static final String ACCUM_BLUE_SIZE_PROPERTY = "accum_blue_size";

   /** Property type <code>accum_alpha_size</code>. */
   public static final String ACCUM_ALPHA_SIZE_PROPERTY = "accum_alpha_size";

   /** Property type <code>active</code>. */
   public static final String ACTIVE_PROPERTY = "active";

   /** Property type <code>analog_count</code>. */
   public static final String ANALOG_COUNT_PROPERTY = "analog_count";

   /** Property type <code>analog_first</code>. */
   public static final String ANALOG_FIRST_PROPERTY = "analog_first";

   /** Property type <code>auxiliary_buffer_count</code>. */
   public static final String AUX_BUFFER_COUNT_PROPERTY =
      "auxiliary_buffer_count";

   /** Property type <code>base_frame_proxy</code>. */
   public static final String BASE_FRAME_PROXY_PROPERTY = "base_frame_proxy";

   /** Property type <code>border</code>. */
   public static final String BORDER_PROPERTY = "border";

   /** Property type <code>button_count</code>. */
   public static final String BUTTON_COUNT_PROPERTY = "button_count";

   /** Property type <code>camera_pos</code>. */
   public static final String CAMERA_POS_PROPERTY = "camera_pos";

   /** Property type <code>red_size</code>. */
   public static final String COLOR_RED_SIZE_PROPERTY = "red_size";

   /** Property type <code>green_size</code>. */
   public static final String COLOR_GREEN_SIZE_PROPERTY = "green_size";

   /** Property type <code>blue_size</code>. */
   public static final String COLOR_BLUE_SIZE_PROPERTY = "blue_size";

   /** Property type <code>alpha_size</code>. */
   public static final String COLOR_ALPHA_SIZE_PROPERTY = "alpha_size";

   /** Property type <code>custom_scale</code>. */
   public static final String CUSTOM_SCALE_PROPERTY = "custom_scale";

   /** Property type <code>decrement_keypress</code>. */
   public static final String DECREMENT_KEYPRESS_PROPERTY =
      "decrement_keypress";

   /** Property type <code>depth_buffer_size</code>. */
   public static final String DEPTH_BUFFER_SIZE_PROPERTY = "depth_buffer_size";

   /** Property type <code>device</code>. */
   public static final String DEVICE_PROPERTY = "device";

   /** Property type <code>device_units</code>. */
   public static final String DEVICE_UNITS_PROPERTY = "device_units";

   /** Property type <code>digital_count</code>. */
   public static final String DIGITAL_COUNT_PROPERTY = "digital_count";

   /** Property type <code>digital_first</code>. */
   public static final String DIGITAL_FIRST_PROPERTY = "digital_first";

   /** Property type <code>frame_buffer_config</code>. */
   public static final String FRAME_BUFFER_PROPERTY = "frame_buffer_config";

   /** Property type <code>fsaa_enable</code>. */
   public static final String FSAA_ENABLE_PROPERTY = "fsaa_enable";

   /** Property type <code>hide_mouse</code>. */
   public static final String HIDE_MOUSE_PROPERTY = "hide_mouse";

   /** Property type <code>increment_keypress</code>. */
   public static final String INCREMENT_KEYPRESS_PROPERTY =
      "increment_keypress";

   /** Property type <code>key_pair</code>. */
   public static final String KEY_PAIR_PROPERTY = "key_pair";

   /** Property type <code>key</code>. */
   public static final String KEY_PROPERTY = "key";

   /** Property type <code>keyboard_mouse_proxy</code>. */
   public static final String KEYBOARD_MOUSE_PROXY_PTR_PROPERTY =
      "keyboard_mouse_proxy";

   /** Property type <code>keyboard_mouse_device_name</code>. */
   public static final String KEYBOARD_MOUSE_PTR_PROPERTY =
      "keyboard_mouse_device_name";

   /** Property type <code>lock_key</code>. */
   public static final String LOCK_KEY_PROPERTY = "lock_key";

   /** Property type <code>lower_left_corner</code>. */
   public static final String LOWER_LEFT_CORNER_PROPERTY = "lower_left_corner";

   /** Property type <code>lower_right_corner</code>. */
   public static final String LOWER_RIGHT_CORNER_PROPERTY =
      "lower_right_corner";

   /** Property type <code>modifier_key</code>. */
   public static final String MODIFIER_KEY_PROPERTY = "modifier_key";

   /** Property type <code>origin</code>. */
   public static final String ORIGIN_PROPERTY = "origin";

   /** Property type <code>position_filters</code>. */
   public static final String POSITION_FILTERS_PROPERTY = "position_filters";

   /** Property type <code>position_transform_filter</code>. */
   public static final String POSITION_TRANSFORM_FILTER_TYPE =
      "position_transform_filter";

   /** Property type <code>pre_rotation</code>. */
   public static final String PRE_ROTATION_PROPERTY = "pre_rotation";

   /** Property type <code>pre_translation</code>. */
   public static final String PRE_TRANSLATION_PROPERTY = "pre_translate";

   /** Property type <code>post_rotation</code>. */
   public static final String POST_ROTATION_PROPERTY = "post_rotation";

   /** Property type <code>post_translation</code>. */
   public static final String POST_TRANSLATION_PROPERTY = "post_translate";

   /** Property type <code>proxy</code>. */
   public static final String PROXY_PROPERTY = "proxy";

   /** Property type <code>relative_proxy</code>. */
   public static final String RELATIVE_PROXY_PROPERTY = "relative_proxy";

   /** Property type <code>simulator_plugin</code>. */
   public static final String SIMULATOR_PLUGIN_PROPERTY = "simulator_plugin";

   /** Property type <code>simulator_viewports</code>. */
   public static final String SIMULATOR_VIEWPORTS_PROPERTY =
      "simulator_viewports";

   /** Property type <code>size</code>. */
   public static final String SIZE_PROPERTY = "size";

   /** Property type <code>sleep_time</code>. */
   public static final String SLEEP_TIME_PROPERTY = "sleep_time";

   /** Property type <code>stations</code>. */
   public static final String STATIONS_PROPERTY = "stations";

   /** Property type <code>start_locked</code>. */
   public static final String START_LOCKED_PROPERTY = "start_locked";

   /** Property type <code>stencil_buffer_size</code>. */
   public static final String STENCIL_BUFFER_SIZE_PROPERTY =
      "stencil_buffer_size";

   /** Property type <code>stereo</code>. */
   public static final String STEREO_PROPERTY = "stereo";

   /** Property type <code>surface_viewports</code>. */
   public static final String SURFACE_VIEWPORTS_PROPERTY = "surface_viewports";

   /** Property type <code>tracked</code>. */
   public static final String TRACKED_PROPERTY = "tracked";

   /** Property type <code>tracker_count</code>. */
   public static final String TRACKER_COUNT_PROPERTY = "tracker_count";

   /** Property type <code>tracker_proxy</code>. */
   public static final String TRACKER_PROXY_PROPERTY = "tracker_proxy";

   /** Property type <code>unit</code>. */
   public static final String UNIT_PROPERTY = "unit";

   /** Property type <code>upper_left_corner</code>. */
   public static final String UPPER_LEFT_CORNER_PROPERTY = "upper_left_corner";

   /** Property type <code>upper_right_corner</code>. */
   public static final String UPPER_RIGHT_CORNER_PROPERTY =
      "upper_right_corner";

   /** Property type <code>user</code>. */
   public static final String USER_PROPERTY = "user";

   /** Property type <code>use_analog</code>. */
   public static final String USE_ANALOG_PROPERTY = "use_analog";

   /** Property type <code>use_digital</code>. */
   public static final String USE_DIGITAL_PROPERTY = "use_digital";

   /** Property type <code>vertical_fov</code>. */
   public static final String VERTICAL_FOV_PROPERTY = "vertical_fov";

   /** Property type <code>view</code>. */
   public static final String VIEW_PROPERTY = "view";

   /** Property type <code>visual_id</code>. */
   public static final String VISUAL_ID_PROPERTY = "visual_id";

   /** Property type <code>wand_pos</code>. */
   public static final String WAND_POS_PROPERTY = "wand_pos";
   
   /** Property type <code>display_windows</code>. */
   public static final String DISPLAY_WINDOWS_PROPERTY = "display_windows";

   // Values.

   /**
    * Index for the "forward" value in the property
    * <code>KEY_PAIR_PROPERTY</code> of the config elmeent type
    * <code>SIM_POS_DEVICE_TYPE</code>.
    *
    * @see #SIM_POS_DEVICE_TYPE
    * @see #KEY_PAIR_PROPERTY
    */
   public static final int FORWARD_VALUE_INDEX = 0;

   /**
    * Index for the "back" value in the property <code>KEY_PAIR_PROPERTY</code>
    * of the config elmeent type <code>SIM_POS_DEVICE_TYPE</code>.
    *
    * @see #SIM_POS_DEVICE_TYPE
    * @see #KEY_PAIR_PROPERTY
    */
   public static final int BACK_VALUE_INDEX = 1;

   /**
    * Index for the "left" value in the property <code>KEY_PAIR_PROPERTY</code>
    * of the config elmeent type <code>SIM_POS_DEVICE_TYPE</code>.
    *
    * @see #SIM_POS_DEVICE_TYPE
    * @see #KEY_PAIR_PROPERTY
    */
   public static final int LEFT_VALUE_INDEX = 2;

   /**
    * Index for the "right" value in the property
    * <code>KEY_PAIR_PROPERTY</code> of the config elmeent type
    * <code>SIM_POS_DEVICE_TYPE</code>.
    *
    * @see #SIM_POS_DEVICE_TYPE
    * @see #KEY_PAIR_PROPERTY
    */
   public static final int RIGHT_VALUE_INDEX = 3;

   /**
    * Index for the "up" value in the property <code>KEY_PAIR_PROPERTY</code>
    * of the config elmeent type <code>SIM_POS_DEVICE_TYPE</code>.
    *
    * @see #SIM_POS_DEVICE_TYPE
    * @see #KEY_PAIR_PROPERTY
    */
   public static final int UP_VALUE_INDEX = 4;

   /**
    * Index for the "down" value in the property <code>KEY_PAIR_PROPERTY</code>
    * of the config elmeent type <code>SIM_POS_DEVICE_TYPE</code>.
    *
    * @see #SIM_POS_DEVICE_TYPE
    * @see #KEY_PAIR_PROPERTY
    */
   public static final int DOWN_VALUE_INDEX = 5;

   /**
    * Index for the "rotate right" value in the property
    * <code>KEY_PAIR_PROPERTY</code> of the config elmeent type
    * <code>SIM_POS_DEVICE_TYPE</code>.
    *
    * @see #SIM_POS_DEVICE_TYPE
    * @see #KEY_PAIR_PROPERTY
    */
   public static final int ROTATE_RIGHT_VALUE_INDEX = 6;

   /**
    * Index for the "rotate left" value in the property
    * <code>KEY_PAIR_PROPERTY</code> of the config elmeent type
    * <code>SIM_POS_DEVICE_TYPE</code>.
    *
    * @see #SIM_POS_DEVICE_TYPE
    * @see #KEY_PAIR_PROPERTY
    */
   public static final int ROTATE_LEFT_VALUE_INDEX = 7;

   /**
    * Index for the "rotate up" value in the property
    * <code>KEY_PAIR_PROPERTY</code> of the config elmeent type
    * <code>SIM_POS_DEVICE_TYPE</code>.
    *
    * @see #SIM_POS_DEVICE_TYPE
    * @see #KEY_PAIR_PROPERTY
    */
   public static final int ROTATE_UP_VALUE_INDEX = 8;

   /**
    * Index for the "rotate down" value in the property
    * <code>KEY_PAIR_PROPERTY</code> of the config elmeent type
    * <code>SIM_POS_DEVICE_TYPE</code>.
    *
    * @see #SIM_POS_DEVICE_TYPE
    * @see #KEY_PAIR_PROPERTY
    */
   public static final int ROTATE_DOWN_VALUE_INDEX = 9;

   /**
    * Index for the "rotate counter-clockwise" value in the property
    * <code>KEY_PAIR_PROPERTY</code> of the config elmeent type
    * <code>SIM_POS_DEVICE_TYPE</code>.
    *
    * @see #SIM_POS_DEVICE_TYPE
    * @see #KEY_PAIR_PROPERTY
    */
   public static final int ROTATE_COUNTER_CLOCKWISE_VALUE_INDEX = 10;

   /**
    * Index for the "rotate clockwise" value in the property
    * <code>KEY_PAIR_PROPERTY</code> of the config elmeent type
    * <code>SIM_POS_DEVICE_TYPE</code>.
    *
    * @see #SIM_POS_DEVICE_TYPE
    * @see #KEY_PAIR_PROPERTY
    */
   public static final int ROTATE_CLOCKWISE_VALUE_INDEX = 11;

   /** Integer form of <code>KEY_NONE</code> value. */
   public static final int KEY_NONE_INT_VALUE = 0;

   /** String form of <code>KEY_NONE</code> value. */
   public static final String KEY_NONE_STRING_VALUE = "KEY_NONE";

   /** Integer form of <code>KEY_UP</code> value. */
   public static final int KEY_UP_INT_VALUE = 1;

   /** String form of <code>KEY_UP</code> value. */
   public static final String KEY_UP_STRING_VALUE = "KEY_UP";

   /** Integer form of <code>KEY_DOWN</code> value. */
   public static final int KEY_DOWN_INT_VALUE = 2;

   /** String form of <code>KEY_DOWN</code> value. */
   public static final String KEY_DOWN_STRING_VALUE = "KEY_DOWN";

   /** Integer form of <code>KEY_LEFT</code> value. */
   public static final int KEY_LEFT_INT_VALUE = 3;

   /** String form of <code>KEY_LEFT</code> value. */
   public static final String KEY_LEFT_STRING_VALUE = "KEY_LEFT";

   /** Integer form of <code>KEY_RIGHT</code> value. */
   public static final int KEY_RIGHT_INT_VALUE = 4;

   /** String form of <code>KEY_RIGHT</code> value. */
   public static final String KEY_RIGHT_STRING_VALUE = "KEY_RIGHT";

   /** Integer form of modifier <code>NONE</code> value. */
   public static final int MODKEY_NONE_INT_VALUE = 0;

   /** String form of modifier <code>NONE</code> value. */
   public static final String MODKEY_NONE_STRING_VALUE = "NONE";

   /** Integer form of modifier <code>CTRL</code> value. */
   public static final int MODKEY_CTRL_INT_VALUE = 5;

   /** String form of modifier <code>CTRL</code> value. */
   public static final String MODKEY_CTRL_STRING_VALUE = "CTRL";

   /** Integer form of modifier <code>SHIFT</code> value. */
   public static final int MODKEY_SHIFT_INT_VALUE = 6;

   /** String form of modifier <code>SHIFT</code> value. */
   public static final String MODKEY_SHIFT_STRING_VALUE = "SHIFT";

   /** Integer form of modifier <code>ALT</code> value. */
   public static final int MODKEY_ALT_INT_VALUE = 7;

   /** String form of modifier <code>ALT</code> value. */
   public static final String MODKEY_ALT_STRING_VALUE = "ALT";

   /** Integer form of modifier <code>ANY</code> value. */
   public static final int MODKEY_ANY_INT_VALUE = -1;

   /** String form of modifier <code>ANY</code> value. */
   public static final String MODKEY_ANY_STRING_VALUE = "ANY";

   /** Integer form of <code>KEY_1</code> value. */
   public static final int KEY_1_INT_VALUE = 8;

   /** String form of <code>KEY_1</code> value. */
   public static final String KEY_1_STRING_VALUE = "KEY_1";

   /** Integer form of <code>KEY_2</code> value. */
   public static final int KEY_2_INT_VALUE = 9;

   /** String form of <code>KEY_2</code> value. */
   public static final String KEY_2_STRING_VALUE = "KEY_2";

   /** Integer form of <code>KEY_3</code> value. */
   public static final int KEY_3_INT_VALUE = 10;

   /** String form of <code>KEY_3</code> value. */
   public static final String KEY_3_STRING_VALUE = "KEY_3";

   /** Integer form of <code>KEY_4</code> value. */
   public static final int KEY_4_INT_VALUE = 11;

   /** String form of <code>KEY_4</code> value. */
   public static final String KEY_4_STRING_VALUE = "KEY_4";

   /** Integer form of <code>KEY_5</code> value. */
   public static final int KEY_5_INT_VALUE = 12;

   /** String form of <code>KEY_5</code> value. */
   public static final String KEY_5_STRING_VALUE = "KEY_5";

   /** Integer form of <code>KEY_6</code> value. */
   public static final int KEY_6_INT_VALUE = 13;

   /** String form of <code>KEY_6</code> value. */
   public static final String KEY_6_STRING_VALUE = "KEY_6";

   /** Integer form of <code>KEY_7</code> value. */
   public static final int KEY_7_INT_VALUE = 14;

   /** String form of <code>KEY_7</code> value. */
   public static final String KEY_7_STRING_VALUE = "KEY_7";

   /** Integer form of <code>KEY_8</code> value. */
   public static final int KEY_8_INT_VALUE = 15;

   /** String form of <code>KEY_8</code> value. */
   public static final String KEY_8_STRING_VALUE = "KEY_8";

   /** Integer form of <code>KEY_9</code> value. */
   public static final int KEY_9_INT_VALUE = 16;

   /** String form of <code>KEY_9</code> value. */
   public static final String KEY_9_STRING_VALUE = "KEY_9";

   /** Integer form of <code>KEY_0</code> value. */
   public static final int KEY_0_INT_VALUE = 17;

   /** String form of <code>KEY_0</code> value. */
   public static final String KEY_0_STRING_VALUE = "KEY_0";

   /** Integer form of <code>KEY_A</code> value. */
   public static final int KEY_A_INT_VALUE = 18;

   /** String form of <code>KEY_A</code> value. */
   public static final String KEY_A_STRING_VALUE = "KEY_A";

   /** Integer form of <code>KEY_B</code> value. */
   public static final int KEY_B_INT_VALUE = 19;

   /** String form of <code>KEY_B</code> value. */
   public static final String KEY_B_STRING_VALUE = "KEY_B";

   /** Integer form of <code>KEY_C</code> value. */
   public static final int KEY_C_INT_VALUE = 20;

   /** String form of <code>KEY_C</code> value. */
   public static final String KEY_C_STRING_VALUE = "KEY_C";

   /** Integer form of <code>KEY_D</code> value. */
   public static final int KEY_D_INT_VALUE = 21;

   /** String form of <code>KEY_D</code> value. */
   public static final String KEY_D_STRING_VALUE = "KEY_D";

   /** Integer form of <code>KEY_E</code> value. */
   public static final int KEY_E_INT_VALUE = 22;

   /** String form of <code>KEY_E</code> value. */
   public static final String KEY_E_STRING_VALUE = "KEY_E";

   /** Integer form of <code>KEY_F</code> value. */
   public static final int KEY_F_INT_VALUE = 23;

   /** String form of <code>KEY_F</code> value. */
   public static final String KEY_F_STRING_VALUE = "KEY_F";

   /** Integer form of <code>KEY_G</code> value. */
   public static final int KEY_G_INT_VALUE = 24;

   /** String form of <code>KEY_G</code> value. */
   public static final String KEY_G_STRING_VALUE = "KEY_G";

   /** Integer form of <code>KEY_H</code> value. */
   public static final int KEY_H_INT_VALUE = 25;

   /** String form of <code>KEY_H</code> value. */
   public static final String KEY_H_STRING_VALUE = "KEY_H";

   /** Integer form of <code>KEY_I</code> value. */
   public static final int KEY_I_INT_VALUE = 26;

   /** String form of <code>KEY_I</code> value. */
   public static final String KEY_I_STRING_VALUE = "KEY_I";

   /** Integer form of <code>KEY_J</code> value. */
   public static final int KEY_J_INT_VALUE = 27;

   /** String form of <code>KEY_J</code> value. */
   public static final String KEY_J_STRING_VALUE = "KEY_J";

   /** Integer form of <code>KEY_K</code> value. */
   public static final int KEY_K_INT_VALUE = 28;

   /** String form of <code>KEY_K</code> value. */
   public static final String KEY_K_STRING_VALUE = "KEY_K";

   /** Integer form of <code>KEY_L</code> value. */
   public static final int KEY_L_INT_VALUE = 29;

   /** String form of <code>KEY_L</code> value. */
   public static final String KEY_L_STRING_VALUE = "KEY_L";

   /** Integer form of <code>KEY_M</code> value. */
   public static final int KEY_M_INT_VALUE = 30;

   /** String form of <code>KEY_M</code> value. */
   public static final String KEY_M_STRING_VALUE = "KEY_M";

   /** Integer form of <code>KEY_N</code> value. */
   public static final int KEY_N_INT_VALUE = 31;

   /** String form of <code>KEY_N</code> value. */
   public static final String KEY_N_STRING_VALUE = "KEY_N";

   /** Integer form of <code>KEY_O</code> value. */
   public static final int KEY_O_INT_VALUE = 32;

   /** String form of <code>KEY_O</code> value. */
   public static final String KEY_O_STRING_VALUE = "KEY_O";

   /** Integer form of <code>KEY_P</code> value. */
   public static final int KEY_P_INT_VALUE = 33;

   /** String form of <code>KEY_P</code> value. */
   public static final String KEY_P_STRING_VALUE = "KEY_P";

   /** Integer form of <code>KEY_Q</code> value. */
   public static final int KEY_Q_INT_VALUE = 34;

   /** String form of <code>KEY_Q</code> value. */
   public static final String KEY_Q_STRING_VALUE = "KEY_Q";

   /** Integer form of <code>KEY_R</code> value. */
   public static final int KEY_R_INT_VALUE = 35;

   /** String form of <code>KEY_R</code> value. */
   public static final String KEY_R_STRING_VALUE = "KEY_R";

   /** Integer form of <code>KEY_S</code> value. */
   public static final int KEY_S_INT_VALUE = 36;

   /** String form of <code>KEY_S</code> value. */
   public static final String KEY_S_STRING_VALUE = "KEY_S";

   /** Integer form of <code>KEY_T</code> value. */
   public static final int KEY_T_INT_VALUE = 37;

   /** String form of <code>KEY_T</code> value. */
   public static final String KEY_T_STRING_VALUE = "KEY_T";

   /** Integer form of <code>KEY_U</code> value. */
   public static final int KEY_U_INT_VALUE = 38;

   /** String form of <code>KEY_U</code> value. */
   public static final String KEY_U_STRING_VALUE = "KEY_U";

   /** Integer form of <code>KEY_V</code> value. */
   public static final int KEY_V_INT_VALUE = 39;

   /** String form of <code>KEY_V</code> value. */
   public static final String KEY_V_STRING_VALUE = "KEY_V";

   /** Integer form of <code>KEY_W</code> value. */
   public static final int KEY_W_INT_VALUE = 40;

   /** String form of <code>KEY_W</code> value. */
   public static final String KEY_W_STRING_VALUE = "KEY_X";

   /** Integer form of <code>KEY_X</code> value. */
   public static final int KEY_X_INT_VALUE = 41;

   /** String form of <code>KEY_X</code> value. */
   public static final String KEY_X_STRING_VALUE = "KEY_X";

   /** Integer form of <code>KEY_Y</code> value. */
   public static final int KEY_Y_INT_VALUE = 42;

   /** String form of <code>KEY_Y</code> value. */
   public static final String KEY_Y_STRING_VALUE = "KEY_Y";

   /** Integer form of <code>KEY_Z</code> value. */
   public static final int KEY_Z_INT_VALUE = 43;

   /** String form of <code>KEY_Z</code> value. */
   public static final String KEY_Z_STRING_VALUE = "KEY_Z";

   /** Integer form of <code>KEY_ESC</code> value. */
   public static final int KEY_ESC_INT_VALUE = 44;

   /** String form of <code>KEY_ESC</code> value. */
   public static final String KEY_ESC_STRING_VALUE = "KEY_ESC";

   /** Integer form of <code>MOUSE_POSX</code> value. */
   public static final int MOUSE_POSX_INT_VALUE = 45;

   /** String form of <code>MOUSE_POSX</code> value. */
   public static final String MOUSE_POSX_STRING_VALUE = "MOUSE_POSX";

   /** Integer form of <code>MOUSE_NEGX</code> value. */
   public static final int MOUSE_NEGX_INT_VALUE = 46;

   /** String form of <code>MOUSE_NEGX</code> value. */
   public static final String MOUSE_NEGX_STRING_VALUE = "MOUSE_NEGX";

   /** Integer form of <code>MOUSE_POSY</code> value. */
   public static final int MOUSE_POSY_INT_VALUE = 47;

   /** String form of <code>MOUSE_POSY</code> value. */
   public static final String MOUSE_POSY_STRING_VALUE = "MOUSE_POSY";

   /** Integer form of <code>MOUSE_NEGY</code> value. */
   public static final int MOUSE_NEGY_INT_VALUE = 48;

   /** String form of <code>MOUSE_NEGY</code> value. */
   public static final String MOUSE_NEGY_STRING_VALUE = "MOUSE_NEGY";

   /** Integer form of <code>MOUSE_BUT1</code> value. */
   public static final int MOUSE_BUT1_INT_VALUE = 49;

   /** String form of <code>MOUSE_BUT1</code> value. */
   public static final String MOUSE_BUT1_STRING_VALUE = "MOUSE_BUT1";

   /** Integer form of <code>MOUSE_BUT2</code> value. */
   public static final int MOUSE_BUT2_INT_VALUE = 50;

   /** String form of <code>MOUSE_BUT2</code> value. */
   public static final String MOUSE_BUT2_STRING_VALUE = "MOUSE_BUT2";

   /** Integer form of <code>MOUSE_BUT3</code> value. */
   public static final int MOUSE_BUT3_INT_VALUE = 51;

   /** String form of <code>MOUSE_BUT3</code> value. */
   public static final String MOUSE_BUT3_STRING_VALUE = "MOUSE_BUT3";

   /**
    * The first allowed VR Juggler mouse button value.  Note that this is
    * not a key value (such as <code>MOUSE_BUT3_INT_VALUE</code>) but rather
    * the start of the range of allowed mouse buttons in
    * [<code>FIRST_MOUSE_BUTTON</code>,&nbsp;<code>LAST_MOUSE_BUTTON</code>].
    *
    * @see #LAST_MOUSE_BUTTON
    */
   public static final int FIRST_MOUSE_BUTTON = 1;

   /**
    * The last allowed VR Juggler mouse button value.  Note that this is
    * not a key value (such as <code>MOUSE_BUT3_INT_VALUE</code>) but rather
    * the end of the range of allowed mouse buttons in
    * [<code>FIRST_MOUSE_BUTTON</code>,&nbsp;<code>LAST_MOUSE_BUTTON</code>].
    *
    * @see #FIRST_MOUSE_BUTTON
    */
   public static final int LAST_MOUSE_BUTTON  = 3;
}
