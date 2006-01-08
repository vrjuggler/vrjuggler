/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.util.HashMap;
import java.util.Map;

import org.vrjuggler.jccl.config.ConfigElement;


/**
 * A collection of general helper functions that may be of use to authors of
 * custom editors for VRJConfig.
 */
public abstract class EditorHelpers
   implements EditorConstants
{
   private static Map sJugglerToJavaMap = new HashMap();
   private static Map sJavaToJugglerMap = new HashMap();

   static
   {
      Integer java_NONE   = new Integer(KeyEvent.VK_UNDEFINED);
      Integer vj_MOD_NONE = new Integer(MODKEY_NONE_INT_VALUE);
      Integer vj_ANY      = new Integer(MODKEY_ANY_INT_VALUE);
      Integer vj_KEY_NONE = new Integer(KEY_NONE_INT_VALUE);

      Integer java_ALT   = new Integer(KeyEvent.VK_ALT);
      Integer java_CTRL  = new Integer(KeyEvent.VK_CONTROL);
      Integer java_META  = new Integer(KeyEvent.VK_META);
      Integer java_SHIFT = new Integer(KeyEvent.VK_SHIFT);
      Integer java_UP    = new Integer(KeyEvent.VK_UP);
      Integer java_DOWN  = new Integer(KeyEvent.VK_DOWN);
      Integer java_LEFT  = new Integer(KeyEvent.VK_LEFT);
      Integer java_RIGHT = new Integer(KeyEvent.VK_RIGHT);
      Integer vj_ALT     = new Integer(MODKEY_ALT_INT_VALUE);
      Integer vj_CTRL    = new Integer(MODKEY_CTRL_INT_VALUE);
      Integer vj_SHIFT   = new Integer(MODKEY_SHIFT_INT_VALUE);
      Integer vj_UP      = new Integer(KEY_UP_INT_VALUE);
      Integer vj_DOWN    = new Integer(KEY_DOWN_INT_VALUE);
      Integer vj_LEFT    = new Integer(KEY_LEFT_INT_VALUE);
      Integer vj_RIGHT   = new Integer(KEY_RIGHT_INT_VALUE);

      Integer java_ESC = new Integer(KeyEvent.VK_ESCAPE);
      Integer vj_ESC   = new Integer(KEY_ESC_INT_VALUE);

      Integer java_A = new Integer(KeyEvent.VK_A);
      Integer java_B = new Integer(KeyEvent.VK_B);
      Integer java_C = new Integer(KeyEvent.VK_C);
      Integer java_D = new Integer(KeyEvent.VK_D);
      Integer java_E = new Integer(KeyEvent.VK_E);
      Integer java_F = new Integer(KeyEvent.VK_F);
      Integer java_G = new Integer(KeyEvent.VK_G);
      Integer java_H = new Integer(KeyEvent.VK_H);
      Integer java_I = new Integer(KeyEvent.VK_I);
      Integer java_J = new Integer(KeyEvent.VK_J);
      Integer java_K = new Integer(KeyEvent.VK_K);
      Integer java_L = new Integer(KeyEvent.VK_L);
      Integer java_M = new Integer(KeyEvent.VK_M);
      Integer java_N = new Integer(KeyEvent.VK_N);
      Integer java_O = new Integer(KeyEvent.VK_O);
      Integer java_P = new Integer(KeyEvent.VK_P);
      Integer java_Q = new Integer(KeyEvent.VK_Q);
      Integer java_R = new Integer(KeyEvent.VK_R);
      Integer java_S = new Integer(KeyEvent.VK_S);
      Integer java_T = new Integer(KeyEvent.VK_T);
      Integer java_U = new Integer(KeyEvent.VK_U);
      Integer java_V = new Integer(KeyEvent.VK_V);
      Integer java_W = new Integer(KeyEvent.VK_W);
      Integer java_X = new Integer(KeyEvent.VK_X);
      Integer java_Y = new Integer(KeyEvent.VK_Y);
      Integer java_Z = new Integer(KeyEvent.VK_Z);
      Integer java_0 = new Integer(KeyEvent.VK_0);
      Integer java_1 = new Integer(KeyEvent.VK_1);
      Integer java_2 = new Integer(KeyEvent.VK_2);
      Integer java_3 = new Integer(KeyEvent.VK_3);
      Integer java_4 = new Integer(KeyEvent.VK_4);
      Integer java_5 = new Integer(KeyEvent.VK_5);
      Integer java_6 = new Integer(KeyEvent.VK_6);
      Integer java_7 = new Integer(KeyEvent.VK_7);
      Integer java_8 = new Integer(KeyEvent.VK_8);
      Integer java_9 = new Integer(KeyEvent.VK_9);
      Integer vj_A   = new Integer(KEY_A_INT_VALUE);
      Integer vj_B   = new Integer(KEY_B_INT_VALUE);
      Integer vj_C   = new Integer(KEY_C_INT_VALUE);
      Integer vj_D   = new Integer(KEY_D_INT_VALUE);
      Integer vj_E   = new Integer(KEY_E_INT_VALUE);
      Integer vj_F   = new Integer(KEY_F_INT_VALUE);
      Integer vj_G   = new Integer(KEY_G_INT_VALUE);
      Integer vj_H   = new Integer(KEY_H_INT_VALUE);
      Integer vj_I   = new Integer(KEY_I_INT_VALUE);
      Integer vj_J   = new Integer(KEY_J_INT_VALUE);
      Integer vj_K   = new Integer(KEY_K_INT_VALUE);
      Integer vj_L   = new Integer(KEY_L_INT_VALUE);
      Integer vj_M   = new Integer(KEY_M_INT_VALUE);
      Integer vj_N   = new Integer(KEY_N_INT_VALUE);
      Integer vj_O   = new Integer(KEY_O_INT_VALUE);
      Integer vj_P   = new Integer(KEY_P_INT_VALUE);
      Integer vj_Q   = new Integer(KEY_Q_INT_VALUE);
      Integer vj_R   = new Integer(KEY_R_INT_VALUE);
      Integer vj_S   = new Integer(KEY_S_INT_VALUE);
      Integer vj_T   = new Integer(KEY_T_INT_VALUE);
      Integer vj_U   = new Integer(KEY_U_INT_VALUE);
      Integer vj_V   = new Integer(KEY_V_INT_VALUE);
      Integer vj_W   = new Integer(KEY_W_INT_VALUE);
      Integer vj_X   = new Integer(KEY_X_INT_VALUE);
      Integer vj_Y   = new Integer(KEY_Y_INT_VALUE);
      Integer vj_Z   = new Integer(KEY_Z_INT_VALUE);
      Integer vj_0   = new Integer(KEY_0_INT_VALUE);
      Integer vj_1   = new Integer(KEY_1_INT_VALUE);
      Integer vj_2   = new Integer(KEY_2_INT_VALUE);
      Integer vj_3   = new Integer(KEY_3_INT_VALUE);
      Integer vj_4   = new Integer(KEY_4_INT_VALUE);
      Integer vj_5   = new Integer(KEY_5_INT_VALUE);
      Integer vj_6   = new Integer(KEY_6_INT_VALUE);
      Integer vj_7   = new Integer(KEY_7_INT_VALUE);
      Integer vj_8   = new Integer(KEY_8_INT_VALUE);
      Integer vj_9   = new Integer(KEY_9_INT_VALUE);

      Integer java_NUMPAD0 = new Integer(KeyEvent.VK_NUMPAD0);
      Integer java_NUMPAD1 = new Integer(KeyEvent.VK_NUMPAD1);
      Integer java_NUMPAD2 = new Integer(KeyEvent.VK_NUMPAD2);
      Integer java_NUMPAD3 = new Integer(KeyEvent.VK_NUMPAD3);
      Integer java_NUMPAD4 = new Integer(KeyEvent.VK_NUMPAD4);
      Integer java_NUMPAD5 = new Integer(KeyEvent.VK_NUMPAD5);
      Integer java_NUMPAD6 = new Integer(KeyEvent.VK_NUMPAD6);
      Integer java_NUMPAD7 = new Integer(KeyEvent.VK_NUMPAD7);
      Integer java_NUMPAD8 = new Integer(KeyEvent.VK_NUMPAD8);
      Integer java_NUMPAD9 = new Integer(KeyEvent.VK_NUMPAD9);

      Integer java_BUTTON1 = new Integer(MouseEvent.BUTTON1);
      Integer java_BUTTON2 = new Integer(MouseEvent.BUTTON3);
      Integer java_BUTTON3 = new Integer(MouseEvent.BUTTON3);
      Integer vj_BUTTON1   = new Integer(MOUSE_BUT1_INT_VALUE);
      Integer vj_BUTTON2   = new Integer(MOUSE_BUT2_INT_VALUE);
      Integer vj_BUTTON3   = new Integer(MOUSE_BUT3_INT_VALUE);

      sJugglerToJavaMap.put(vj_KEY_NONE, java_NONE);
      sJugglerToJavaMap.put(vj_MOD_NONE, java_NONE);
      sJugglerToJavaMap.put(vj_UP, java_UP);
      sJugglerToJavaMap.put(vj_DOWN, java_DOWN);
      sJugglerToJavaMap.put(vj_LEFT, java_LEFT);
      sJugglerToJavaMap.put(vj_RIGHT, java_RIGHT);
      sJugglerToJavaMap.put(vj_CTRL, java_CTRL);
      sJugglerToJavaMap.put(vj_SHIFT, java_SHIFT);
      sJugglerToJavaMap.put(vj_ALT, java_ALT);
      sJugglerToJavaMap.put(vj_1, java_1);
      sJugglerToJavaMap.put(vj_2, java_2);
      sJugglerToJavaMap.put(vj_3, java_3);
      sJugglerToJavaMap.put(vj_4, java_4);
      sJugglerToJavaMap.put(vj_5, java_5);
      sJugglerToJavaMap.put(vj_6, java_6);
      sJugglerToJavaMap.put(vj_7, java_7);
      sJugglerToJavaMap.put(vj_8, java_8);
      sJugglerToJavaMap.put(vj_9, java_9);
      sJugglerToJavaMap.put(vj_0, java_0);
      sJugglerToJavaMap.put(vj_A, java_A);
      sJugglerToJavaMap.put(vj_B, java_B);
      sJugglerToJavaMap.put(vj_C, java_C);
      sJugglerToJavaMap.put(vj_D, java_D);
      sJugglerToJavaMap.put(vj_E, java_E);
      sJugglerToJavaMap.put(vj_F, java_F);
      sJugglerToJavaMap.put(vj_G, java_G);
      sJugglerToJavaMap.put(vj_H, java_H);
      sJugglerToJavaMap.put(vj_I, java_I);
      sJugglerToJavaMap.put(vj_J, java_J);
      sJugglerToJavaMap.put(vj_K, java_K);
      sJugglerToJavaMap.put(vj_L, java_L);
      sJugglerToJavaMap.put(vj_M, java_M);
      sJugglerToJavaMap.put(vj_N, java_N);
      sJugglerToJavaMap.put(vj_O, java_O);
      sJugglerToJavaMap.put(vj_P, java_P);
      sJugglerToJavaMap.put(vj_Q, java_Q);
      sJugglerToJavaMap.put(vj_R, java_R);
      sJugglerToJavaMap.put(vj_S, java_S);
      sJugglerToJavaMap.put(vj_T, java_T);
      sJugglerToJavaMap.put(vj_U, java_U);
      sJugglerToJavaMap.put(vj_V, java_V);
      sJugglerToJavaMap.put(vj_W, java_W);
      sJugglerToJavaMap.put(vj_X, java_X);
      sJugglerToJavaMap.put(vj_Y, java_Y);
      sJugglerToJavaMap.put(vj_Z, java_Z);
      sJugglerToJavaMap.put(vj_BUTTON1, java_BUTTON1);
      sJugglerToJavaMap.put(vj_BUTTON2, java_BUTTON2);
      sJugglerToJavaMap.put(vj_BUTTON3, java_BUTTON3);
      sJugglerToJavaMap.put(vj_ESC, java_ESC);

      sJavaToJugglerMap.put(java_NONE, vj_KEY_NONE);
      sJavaToJugglerMap.put(java_UP, vj_UP);
      sJavaToJugglerMap.put(java_DOWN, vj_DOWN);
      sJavaToJugglerMap.put(java_LEFT, vj_LEFT);
      sJavaToJugglerMap.put(java_RIGHT, vj_RIGHT);
      sJavaToJugglerMap.put(java_CTRL, vj_CTRL);
      sJavaToJugglerMap.put(java_SHIFT, vj_SHIFT);
      sJavaToJugglerMap.put(java_ALT, vj_ALT);
      sJavaToJugglerMap.put(java_META, vj_ALT);
      sJavaToJugglerMap.put(java_1, vj_1);
      sJavaToJugglerMap.put(java_2, vj_2);
      sJavaToJugglerMap.put(java_3, vj_3);
      sJavaToJugglerMap.put(java_4, vj_4);
      sJavaToJugglerMap.put(java_5, vj_5);
      sJavaToJugglerMap.put(java_6, vj_6);
      sJavaToJugglerMap.put(java_7, vj_7);
      sJavaToJugglerMap.put(java_8, vj_8);
      sJavaToJugglerMap.put(java_9, vj_9);
      sJavaToJugglerMap.put(java_0, vj_0);
      sJavaToJugglerMap.put(java_A, vj_A);
      sJavaToJugglerMap.put(java_B, vj_B);
      sJavaToJugglerMap.put(java_C, vj_C);
      sJavaToJugglerMap.put(java_D, vj_D);
      sJavaToJugglerMap.put(java_E, vj_E);
      sJavaToJugglerMap.put(java_F, vj_F);
      sJavaToJugglerMap.put(java_G, vj_G);
      sJavaToJugglerMap.put(java_H, vj_H);
      sJavaToJugglerMap.put(java_I, vj_I);
      sJavaToJugglerMap.put(java_J, vj_J);
      sJavaToJugglerMap.put(java_K, vj_K);
      sJavaToJugglerMap.put(java_L, vj_L);
      sJavaToJugglerMap.put(java_M, vj_M);
      sJavaToJugglerMap.put(java_N, vj_N);
      sJavaToJugglerMap.put(java_O, vj_O);
      sJavaToJugglerMap.put(java_P, vj_P);
      sJavaToJugglerMap.put(java_Q, vj_Q);
      sJavaToJugglerMap.put(java_R, vj_R);
      sJavaToJugglerMap.put(java_S, vj_S);
      sJavaToJugglerMap.put(java_T, vj_T);
      sJavaToJugglerMap.put(java_U, vj_U);
      sJavaToJugglerMap.put(java_V, vj_V);
      sJavaToJugglerMap.put(java_W, vj_W);
      sJavaToJugglerMap.put(java_X, vj_X);
      sJavaToJugglerMap.put(java_Y, vj_Y);
      sJavaToJugglerMap.put(java_Z, vj_Z);
      sJavaToJugglerMap.put(java_BUTTON1, vj_BUTTON1);
      sJavaToJugglerMap.put(java_BUTTON2, vj_BUTTON2);
      sJavaToJugglerMap.put(java_BUTTON3, vj_BUTTON3);
   }

   /**
    * Converts the given Java virtual key or mouse button (as defined in
    * <code>java.awt.event.KeyEvent</code> and
    * <code>java.awt.event.MouseEvent</code> respectively) to the corresponding
    * VR Juggler key value.
    *
    * @param javaKey    the Java vritual key or mouse button value to convert
    *                   to a VR Juggler key value
    *
    * @return The VR Juggler version of the given Java virtual key or mouse
    *         button value or -1 if no such value is defined
    *
    * @see java.awt.event.KeyEvent
    * @see java.awt.event.MouseEvent
    */
   public static int convertJavaKeyToJuggler(int javaKey)
   {
      Integer vj_key = (Integer) sJavaToJugglerMap.get(new Integer(javaKey));
      return (vj_key == null ? -1 : vj_key.intValue());
   }

   /**
    * Converts the given VR Juggler key to the corresponding Java virtual
    * key or mouse button (as defined in <code>java.awt.event.KeyEvent</code>
    * and <code>java.awt.event.MouseEvent</code> respectively).
    *
    * @param jugglerKey the VR Juggler key value to convert to the Java
    *                   virtual key or mouse button value
    *
    * @return The Java version of the given VR Juggler key or -1 if no such
    *         Java virtual key or mouse button value is defined
    *
    * @see java.awt.event.KeyEvent
    * @see java.awt.event.MouseEvent
    */
   public static int convertJugglerKeyToJava(int jugglerKey)
   {
      Integer java_key =
         (Integer) sJugglerToJavaMap.get(new Integer(jugglerKey));
      return (java_key == null ? -1 : java_key.intValue());
   }

   /**
    * Converts the given modfier set (a composition of Java modifier masks
    * as defined in <code>java.awt.event.InputEvent</code>) to the equivalent
    * VR Juggler modifier interpretation.  If the modifier set contains
    * multiple modifiers, the value returned will be
    * <code>EditorConstants.MODKEY_ANY_INT_VALUE</code>.
    *
    * @param javaModifiers      the modifier set that is a bitmask of values
    *                           such as
    *                           <code>java.awt.event.InputEvent.CTRL_MASK</code>
    *                           and/or
    *                           <code>java.awt.event.InputEvent.ALT_MASK</code>
    *
    *
    * @return One of <code>EditorConstants.MODKEY_ANY_INT_VALUE</code>,
    *         <code>EditorConstants.MODKEY_ALT_INT_VALUE</code>,
    *         <code>EditorConstants.MODKEY_CTRL_INT_VALUE</code>,
    *         <code>EditorConstants.MODKEY_SHIFT_INT_VALUE</code>, or
    *         <code>EditorConstants.MODKEY_NONE_INT_VALUE</code>.
    *
    * @see java.awt.event.InputEvent
    * @see java.awt.event.InputEvent#getModifiers()
    */
   public static int convertJavaModifiersToJuggler(int javaModifiers)
   {
      int mod = MODKEY_ANY_INT_VALUE;

      switch (javaModifiers)
      {
         case InputEvent.ALT_MASK:
         case InputEvent.META_MASK:
            mod = MODKEY_ALT_INT_VALUE;
            break;
         case InputEvent.CTRL_MASK:
            mod = MODKEY_CTRL_INT_VALUE;
            break;
         case InputEvent.SHIFT_MASK:
            mod = MODKEY_SHIFT_INT_VALUE;
            break;
         case 0:
            mod = MODKEY_NONE_INT_VALUE;
            break;
      }

      return mod;
   }

   /**
    * Determines whether the given Java virtual key or mouse button value
    * (as defined in <code>java.awt.event.Keyevent</code> and
    * <code>java.awt.event.MouseEvent</code> respectively) is allowed to be
    * used with VR Juggler configurations.
    *
    * @see java.awt.event.KeyEvent
    * @see java.awt.event.MouseEvent
    * @see #convertJavaKeyToJuggler(int)
    * @see #convertJavaModifiersToJuggler(int)
    */
   public static boolean isAllowedJavaKey(int javaKey)
   {
      return (sJavaToJugglerMap.get(new Integer(javaKey)) != null);
   }

   /**
    * Determines whether the given value represents a VR Juggler mouse
    * button.
    */
   public static boolean isJugglerMouseButton(int v)
   {
      return v == MOUSE_BUT1_INT_VALUE || v == MOUSE_BUT2_INT_VALUE ||
             v == MOUSE_BUT3_INT_VALUE;
   }

   /**
    * Determines whether the given value represents a VR Juggler mouse
    * motion.
    */
   public static boolean isJugglerMouseMotion(int v)
   {
      return v == MOUSE_POSX_INT_VALUE || v == MOUSE_NEGX_INT_VALUE ||
             v == MOUSE_POSY_INT_VALUE || v == MOUSE_NEGY_INT_VALUE;
   }

   /**
    * Determines whether the given value represents a VR Juggler key (in other
    * words, not a VR Juggler mouse button or VR Juggler mouse motion).
    */
   public static boolean isJugglerKey(int v)
   {
      return ! isJugglerMouseButton(v) && ! isJugglerMouseMotion(v);
   }

   /**
    * Determines whether the given value represents a Java mouse button.
    *
    * @see java.awt.event.MouseEvent
    */
   public static boolean isJavaMouseButton(int v)
   {
      return v == MouseEvent.BUTTON1 || v == MouseEvent.BUTTON2 ||
             v == MouseEvent.BUTTON3;
   }

   /**
    * Determines whether the given value represents a Java virtual key.
    *
    * @see java.awt.event.KeyEvent
    */
   public static boolean isJavaKey(int v)
   {
      return ! isJavaMouseButton(v);
   }

   /**
    * Constructs a string describing the given config element of type
    * <code>EditorConstants.KEY_MODIFIER_PAIR_TYPE</code>.
    *
    * @param keypressElt        a config element of type
    *                           <code>EditorConstants.KEY_MODIFIER_PAIR_TYPE</code>
    *
    * @return A string describing the key+modifier combination of the form
    *         "Ctrl+Right", "Any Modifier+5", or "K".
    */
   public static String getKeyPressText(ConfigElement keypressElt)
   {
      Integer key = (Integer) keypressElt.getProperty(KEY_PROPERTY, 0);
      Integer mod = (Integer) keypressElt.getProperty(MODIFIER_KEY_PROPERTY,
                                                      0);

      return getKeyPressText(key.intValue(), mod.intValue());
   }

   /**
    * Constructs a string describing the given key+modifier combination.
    * The key and modifier values must be VR Juggler key values.
    *
    * @param key        a VR Juggler key value (including mouse buttons and
    *                   mouse motions)
    * @param mod        a VR Juggler modifier key value
    *
    * @return A string describing the key+modifier combination of the form
    *         "Ctrl+Right", "Any Modifier+5", or "K".
    *
    * @see #convertJavaKeyToJuggler(int)
    * @see #convertJavaModifiersToJuggler(int)
    */
   public static String getKeyPressText(int key, int mod)
   {
      String modifier_text = getModifierText(mod);
      String key_text = null;

      if ( isJugglerKey(key) )
      {
         if ( key == KEY_NONE_INT_VALUE )
         {
            key_text = "NONE";
         }
         else
         {
            int java_key = convertJugglerKeyToJava(key);
            key_text = KeyEvent.getKeyText(java_key);
         }
      }
      else if ( isJugglerMouseButton(key) )
      {
         int java_key = convertJugglerKeyToJava(key);

         switch (key)
         {
            case MOUSE_BUT1_INT_VALUE:
               key_text = "Button1";
               break;
            case MOUSE_BUT2_INT_VALUE:
               key_text = "Button2";
               break;
            default:
               key_text = "Button3";
               break;
         }
      }
      else
      {
         switch (key)
         {
            case MOUSE_POSX_INT_VALUE:
               key_text = "Mouse Right";
               break;
            case MOUSE_NEGX_INT_VALUE:
               key_text = "Mouse Left";
               break;
            case MOUSE_POSY_INT_VALUE:
               key_text = "Mouse Back";
               break;
            default:
               key_text = "Mouse Forward";
               break;
         }
      }

      return modifier_text + key_text;
   }

   /**
    * Constructs a string describing the given modifier value.  The modifier
    * value must be a VR Juggler modifier key value.
    *
    * @param mod        a VR Juggler modifier key value
    *
    * @return A string describing the key+modifier combination of the form
    *         "Ctrl+", "Any Modifier+", etc.
    *
    * @see #convertJavaModifiersToJuggler(int)
    */
   public static String getModifierText(int mod)
   {
      String modifier_text = "";

      switch (mod)
      {
         case MODKEY_ALT_INT_VALUE:
            modifier_text = "Alt+";
            break;
         case MODKEY_CTRL_INT_VALUE:
            modifier_text = "Ctrl+";
            break;
         case MODKEY_SHIFT_INT_VALUE:
            modifier_text = "Shift+";
            break;
         case MODKEY_ANY_INT_VALUE:
            modifier_text = "Any Modifier+";
            break;
      }

      return modifier_text;
   }
}
