/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
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

// Includes ====================================================================
#include <boost/python.hpp>
#include <gadget/Type/EventWindow/Keys.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_gadget_Type_EventWindow_Keys_h()
{
    enum_< gadget::Keys >("Keys")
        .value("KEY_E", gadget::KEY_E)
        .value("KEY_D", gadget::KEY_D)
        .value("KEY_G", gadget::KEY_G)
        .value("KEY_F", gadget::KEY_F)
        .value("KEY_A", gadget::KEY_A)
        .value("KEY_EXCLAM", gadget::KEY_EXCLAM)
        .value("KEY_C", gadget::KEY_C)
        .value("KEY_B", gadget::KEY_B)
        .value("KEY_M", gadget::KEY_M)
        .value("KEY_L", gadget::KEY_L)
        .value("KEY_O", gadget::KEY_O)
        .value("KEY_N", gadget::KEY_N)
        .value("KEY_I", gadget::KEY_I)
        .value("KEY_H", gadget::KEY_H)
        .value("KEY_K", gadget::KEY_K)
        .value("KEY_J", gadget::KEY_J)
        .value("KEY_U", gadget::KEY_U)
        .value("KEY_T", gadget::KEY_T)
        .value("KEY_W", gadget::KEY_W)
        .value("KEY_V", gadget::KEY_V)
        .value("KEY_Q", gadget::KEY_Q)
        .value("KEY_P", gadget::KEY_P)
        .value("KEY_S", gadget::KEY_S)
        .value("KEY_R", gadget::KEY_R)
        .value("KEY_AT", gadget::KEY_AT)
        .value("KEY_Y", gadget::KEY_Y)
        .value("KEY_X", gadget::KEY_X)
        .value("KEY_Z", gadget::KEY_Z)
        .value("KEY_ASCII_CIRCUM", gadget::KEY_ASCII_CIRCUM)
        .value("KEY_UP", gadget::KEY_UP)
        .value("KEY_UNKNOWN", gadget::KEY_UNKNOWN)
        .value("KEY_BRACKET_RIGHT", gadget::KEY_BRACKET_RIGHT)
        .value("MOUSE_NEGX", gadget::MOUSE_NEGX)
        .value("KEY_NUMBER_SIGN", gadget::KEY_NUMBER_SIGN)
        .value("KEY_DELETE", gadget::KEY_DELETE)
        .value("MOUSE_NEGY", gadget::MOUSE_NEGY)
        .value("KEY_SCROLL_LOCK", gadget::KEY_SCROLL_LOCK)
        .value("KEY_MENU", gadget::KEY_MENU)
        .value("KEY_APOSTROPHE", gadget::KEY_APOSTROPHE)
        .value("KEY_BAR", gadget::KEY_BAR)
        .value("KEY_DOLLAR", gadget::KEY_DOLLAR)
        .value("KEY_SPACE", gadget::KEY_SPACE)
        .value("KEY_SUPER_R", gadget::KEY_SUPER_R)
        .value("KEY_PAREN_LEFT", gadget::KEY_PAREN_LEFT)
        .value("KEY_RETURN", gadget::KEY_RETURN)
        .value("KEY_HELP", gadget::KEY_HELP)
        .value("KEY_PERCENT", gadget::KEY_PERCENT)
        .value("KEY_SYSREQ", gadget::KEY_SYSREQ)
        .value("KEY_F23", gadget::KEY_F23)
        .value("KEY_BACKTAB", gadget::KEY_BACKTAB)
        .value("KEY_BACKSPACE", gadget::KEY_BACKSPACE)
        .value("KEY_MINUS", gadget::KEY_MINUS)
        .value("KEY_PRINT", gadget::KEY_PRINT)
        .value("NO_MBUTTON", gadget::NO_MBUTTON)
        .value("KEY_SUPER_L", gadget::KEY_SUPER_L)
        .value("KEY_DOWN", gadget::KEY_DOWN)
        .value("KEY_SLASH", gadget::KEY_SLASH)
        .value("KEY_ESC", gadget::KEY_ESC)
        .value("KEY_SEMICOLON", gadget::KEY_SEMICOLON)
        .value("KEY_F34", gadget::KEY_F34)
        .value("KEY_F35", gadget::KEY_F35)
        .value("KEY_BRACKET_LEFT", gadget::KEY_BRACKET_LEFT)
        .value("KEY_NUM_LOCK", gadget::KEY_NUM_LOCK)
        .value("KEY_NONE", gadget::KEY_NONE)
        .value("KEY_END", gadget::KEY_END)
        .value("KEY_5", gadget::KEY_5)
        .value("KEY_4", gadget::KEY_4)
        .value("KEY_7", gadget::KEY_7)
        .value("KEY_6", gadget::KEY_6)
        .value("KEY_1", gadget::KEY_1)
        .value("KEY_0", gadget::KEY_0)
        .value("KEY_3", gadget::KEY_3)
        .value("KEY_2", gadget::KEY_2)
        .value("KEY_F30", gadget::KEY_F30)
        .value("KEY_9", gadget::KEY_9)
        .value("KEY_8", gadget::KEY_8)
        .value("KEY_F31", gadget::KEY_F31)
        .value("KEY_HYPER_L", gadget::KEY_HYPER_L)
        .value("KEY_PERIOD", gadget::KEY_PERIOD)
        .value("KEY_F32", gadget::KEY_F32)
        .value("KEY_QUOTE_LEFT", gadget::KEY_QUOTE_LEFT)
        .value("KEY_F33", gadget::KEY_F33)
        .value("KEY_PAUSE", gadget::KEY_PAUSE)
        .value("KEY_COLON", gadget::KEY_COLON)
        .value("KEY_F18", gadget::KEY_F18)
        .value("KEY_F19", gadget::KEY_F19)
        .value("KEY_F16", gadget::KEY_F16)
        .value("KEY_F17", gadget::KEY_F17)
        .value("KEY_F14", gadget::KEY_F14)
        .value("KEY_ENTER", gadget::KEY_ENTER)
        .value("KEY_F12", gadget::KEY_F12)
        .value("KEY_F13", gadget::KEY_F13)
        .value("KEY_F10", gadget::KEY_F10)
        .value("KEY_F11", gadget::KEY_F11)
        .value("KEY_F4", gadget::KEY_F4)
        .value("KEY_F5", gadget::KEY_F5)
        .value("KEY_F6", gadget::KEY_F6)
        .value("KEY_F7", gadget::KEY_F7)
        .value("LAST_KEY", gadget::LAST_KEY)
        .value("KEY_F1", gadget::KEY_F1)
        .value("KEY_F2", gadget::KEY_F2)
        .value("KEY_F3", gadget::KEY_F3)
        .value("KEY_GREATER", gadget::KEY_GREATER)
        .value("KEY_F8", gadget::KEY_F8)
        .value("KEY_F9", gadget::KEY_F9)
        .value("KEY_RIGHT", gadget::KEY_RIGHT)
        .value("KEY_BRACE_LEFT", gadget::KEY_BRACE_LEFT)
        .value("KEY_HYPER_R", gadget::KEY_HYPER_R)
        .value("KEY_ALT", gadget::KEY_ALT)
        .value("KEY_EQUAL", gadget::KEY_EQUAL)
        .value("KEY_CTRL", gadget::KEY_CTRL)
        .value("KEY_ASTERISK", gadget::KEY_ASTERISK)
        .value("KEY_SHIFT", gadget::KEY_SHIFT)
        .value("KEY_ASCII_TILDE", gadget::KEY_ASCII_TILDE)
        .value("KEY_CAPS_LOCK", gadget::KEY_CAPS_LOCK)
        .value("KEY_LESS", gadget::KEY_LESS)
        .value("KEY_AMPERSAND", gadget::KEY_AMPERSAND)
        .value("KEY_F27", gadget::KEY_F27)
        .value("KEY_F26", gadget::KEY_F26)
        .value("KEY_F25", gadget::KEY_F25)
        .value("KEY_F24", gadget::KEY_F24)
        .value("KEY_F15", gadget::KEY_F15)
        .value("KEY_HOME", gadget::KEY_HOME)
        .value("KEY_F21", gadget::KEY_F21)
        .value("KEY_F20", gadget::KEY_F20)
        .value("KEY_COMMA", gadget::KEY_COMMA)
        .value("KEY_ANY", gadget::KEY_ANY)
        .value("KEY_PAREN_RIGHT", gadget::KEY_PAREN_RIGHT)
        .value("KEY_F29", gadget::KEY_F29)
        .value("KEY_F28", gadget::KEY_F28)
        .value("KEY_UNDERSCORE", gadget::KEY_UNDERSCORE)
        .value("KEY_NEXT", gadget::KEY_NEXT)
        .value("MOUSE_POSX", gadget::MOUSE_POSX)
        .value("MOUSE_POSY", gadget::MOUSE_POSY)
        .value("KEY_QUESTION", gadget::KEY_QUESTION)
        .value("KEY_BACKSLASH", gadget::KEY_BACKSLASH)
        .value("KEY_INSERT", gadget::KEY_INSERT)
        .value("KEY_BRACE_RIGHT", gadget::KEY_BRACE_RIGHT)
        .value("KEY_TAB", gadget::KEY_TAB)
        .value("KEY_LEFT", gadget::KEY_LEFT)
        .value("KEY_QUOTE_DBL", gadget::KEY_QUOTE_DBL)
        .value("KEY_PLUS", gadget::KEY_PLUS)
        .value("KEY_PRIOR", gadget::KEY_PRIOR)
        .value("MBUTTON5", gadget::MBUTTON5)
        .value("MBUTTON4", gadget::MBUTTON4)
        .value("MBUTTON3", gadget::MBUTTON3)
        .value("MBUTTON2", gadget::MBUTTON2)
        .value("MBUTTON1", gadget::MBUTTON1)
        .value("KEY_F22", gadget::KEY_F22)
    ;

    enum_< gadget::ModiferMask >("ModiferMask")
        .value("ALT_MASK", gadget::ALT_MASK)
        .value("SHIFT_MASK", gadget::SHIFT_MASK)
        .value("CTRL_MASK", gadget::CTRL_MASK)
    ;

    enum_< gadget::ButtonMask >("ButtonMask")
        .value("BUTTON5_MASK", gadget::BUTTON5_MASK)
        .value("BUTTON1_MASK", gadget::BUTTON1_MASK)
        .value("BUTTON2_MASK", gadget::BUTTON2_MASK)
        .value("BUTTON3_MASK", gadget::BUTTON3_MASK)
        .value("BUTTON4_MASK", gadget::BUTTON4_MASK)
    ;

}
