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

#ifndef _GADGET_KEYS_H_
#define _GADGET_KEYS_H_

namespace gadget
{

/// Names of defined keys.  This list is based on Qt's list of recognized keys.
enum Keys
{
   KEY_NONE,
   KEY_UP,
   KEY_DOWN,
   KEY_LEFT,
   KEY_RIGHT,
   KEY_SHIFT,   /**< Shift key (either left or right) */
   KEY_CTRL,    /**< Control key */
   KEY_ALT,     /**< Alt or Option key */
   KEY_COMMAND, /**< Apple Command key */
   KEY_1,
   KEY_2,
   KEY_3,
   KEY_4,
   KEY_5,
   KEY_6,
   KEY_7,
   KEY_8,
   KEY_9,
   KEY_0,
   KEY_A,
   KEY_B,
   KEY_C,
   KEY_D,
   KEY_E,
   KEY_F,
   KEY_G,
   KEY_H,
   KEY_I,
   KEY_J,
   KEY_K,
   KEY_L,
   KEY_M,
   KEY_N,
   KEY_O,
   KEY_P,
   KEY_Q,
   KEY_R,
   KEY_S,
   KEY_T,
   KEY_U,
   KEY_V,
   KEY_W,
   KEY_X,
   KEY_Y,
   KEY_Z,
   KEY_ESC,

   // XXX: Mouse information probably shouldn't be here in the long term.
   MOUSE_POSX,          /**< Mouse movement in the positive X axis. */
   MOUSE_NEGX,          /**< Mouse movement in the negative X axis. */
   MOUSE_POSY,          /**< Mouse movement in the positive Y axis. */
   MOUSE_NEGY,          /**< Mouse movement in the negative Y axis. */
   MBUTTON1,            /**< Mouse button 1. */
   MBUTTON2,            /**< Mouse button 2. */
   MBUTTON3,            /**< Mouse button 3. */
   MBUTTON4,            /**< Mouse button 4. */
   MBUTTON5,            /**< Mouse button 5. */
   MBUTTON6,            /**< Mouse button 6. */
   MBUTTON7,            /**< Mouse button 7. */
   MBUTTON8,            /**< Mouse button 8. */
   MBUTTON9,            /**< Mouse button 9. */
   NO_MBUTTON,          /**< No mouse button. */

   MOUSE_SCROLL_UP,     /**< Mouse scroll up. */
   MOUSE_SCROLL_DOWN,   /**< Mouse scroll down. */
   MOUSE_SCROLL_LEFT,   /**< Mouse scroll left. */
   MOUSE_SCROLL_RIGHT,  /**< Mouse scroll right. */

   KEY_TAB,
   KEY_BACKTAB,
   KEY_BACKSPACE,
   KEY_RETURN,
   KEY_ENTER,
   KEY_INSERT,
   KEY_DELETE,
   KEY_PAUSE,
   KEY_PRINT,
   KEY_SYSREQ,
   KEY_HOME,
   KEY_END,
   KEY_PRIOR,           /**< Page up */
   KEY_NEXT,            /**< Page down */
   KEY_CAPS_LOCK,
   KEY_NUM_LOCK,
   KEY_SCROLL_LOCK,
   KEY_F1,
   KEY_F2,
   KEY_F3,
   KEY_F4,
   KEY_F5,
   KEY_F6,
   KEY_F7,
   KEY_F8,
   KEY_F9,
   KEY_F10,
   KEY_F11,
   KEY_F12,
   KEY_F13,
   KEY_F14,
   KEY_F15,
   KEY_F16,
   KEY_F17,
   KEY_F18,
   KEY_F19,
   KEY_F20,
   KEY_F21,
   KEY_F22,
   KEY_F23,
   KEY_F24,
   KEY_F25,
   KEY_F26,
   KEY_F27,
   KEY_F28,
   KEY_F29,
   KEY_F30,
   KEY_F31,
   KEY_F32,
   KEY_F33,
   KEY_F34,
   KEY_F35,
   KEY_SUPER_L,
   KEY_SUPER_R,
   KEY_MENU,
   KEY_HYPER_L,
   KEY_HYPER_R,
   KEY_HELP,
   KEY_SPACE,
   KEY_ANY,
   KEY_EXCLAM,          /**< ! */
   KEY_QUOTE_DBL,       /**< " */
   KEY_NUMBER_SIGN,     /**< # */
   KEY_DOLLAR,          /**< $ */
   KEY_PERCENT,         /**< % */
   KEY_AMPERSAND,       /**< & */
   KEY_APOSTROPHE,      /**< ' */
   KEY_PAREN_LEFT,      /**< ( */
   KEY_PAREN_RIGHT,     /**< ) */
   KEY_ASTERISK,        /**< * */
   KEY_PLUS,            /**< + */
   KEY_COMMA,           /**< , */
   KEY_MINUS,           /**< - */
   KEY_PERIOD,          /**< . */
   KEY_SLASH,           /**< / */
   KEY_COLON,           /**< : */
   KEY_SEMICOLON,       /**< ; */
   KEY_LESS,            /**< < */
   KEY_EQUAL,           /**< = */
   KEY_GREATER,         /**< > */
   KEY_QUESTION,        /**< ? */
   KEY_AT,              /**< @ */
   KEY_BRACKET_LEFT,    /**< [ */
   KEY_BACKSLASH,       /**< \ */
   KEY_BRACKET_RIGHT,   /**< ] */
   KEY_ASCII_CIRCUM,    /**< ^ */
   KEY_UNDERSCORE,      /**< _ */
   KEY_QUOTE_LEFT,
   KEY_BRACE_LEFT,      /**< { */
   KEY_BAR,             /**< | */
   KEY_BRACE_RIGHT,     /**< } */
   KEY_ASCII_TILDE,     /**< ~ */

/*
   KEY_EXCLAM_DOWN,
   KEY_CENT,
   KEY_STERLING,
   KEY_CURRENCY,
   KEY_YEN,

   KEY_brokenbar ,
   KEY_section ,
   KEY_diaeresis ,
   KEY_copyright ,
   KEY_ordfeminine ,
   KEY_guillemotleft ,
   KEY_notsign ,
   KEY_hyphen ,
   KEY_registered ,
   KEY_macron ,
   KEY_degree ,
   KEY_plusminus ,
   KEY_twosuperior ,
   KEY_threesuperior ,
   KEY_acute ,
   KEY_mu ,
   KEY_paragraph ,
   KEY_periodcentered ,
   KEY_cedilla ,
   KEY_onesuperior ,
   KEY_masculine ,
   KEY_guillemotright ,
   KEY_onequarter ,
   KEY_onehalf ,
   KEY_threequarters ,
   KEY_questiondown ,
   KEY_Agrave ,
   KEY_Aacute ,
   KEY_Acircumflex ,
   KEY_Atilde ,
   KEY_Adiaeresis ,
   KEY_Aring ,
   KEY_AE ,
   KEY_Ccedilla ,
   KEY_Egrave ,
   KEY_Eacute ,
   KEY_Ecircumflex ,
   KEY_Ediaeresis ,
   KEY_Igrave ,
   KEY_Iacute ,
   KEY_Icircumflex ,
   KEY_Idiaeresis ,
   KEY_ETH ,
   KEY_Ntilde ,
   KEY_Ograve ,
   KEY_Oacute ,
   KEY_Ocircumflex ,
   KEY_Otilde ,
   KEY_Odiaeresis ,
   KEY_multiply ,
   KEY_Ooblique ,
   KEY_Ugrave ,
   KEY_Uacute ,
   KEY_Ucircumflex ,
   KEY_Udiaeresis ,
   KEY_Yacute ,
   KEY_THORN ,
   KEY_ssharp ,
   KEY_agrave ,
   KEY_aacute ,
   KEY_acircumflex ,
   KEY_atilde ,
   KEY_adiaeresis ,
   KEY_aring ,
   KEY_ae ,
   KEY_ccedilla ,
   KEY_egrave ,
   KEY_eacute ,
   KEY_ecircumflex ,
   KEY_ediaeresis ,
   KEY_igrave ,
   KEY_iacute ,
   KEY_icircumflex ,
   KEY_idiaeresis ,
   KEY_eth ,
   KEY_ntilde ,
   KEY_ograve ,
   KEY_oacute ,
   KEY_ocircumflex ,
   KEY_otilde ,
   KEY_odiaeresis ,
   KEY_division ,
   KEY_oslash ,
   KEY_ugrave ,
   KEY_uacute ,
   KEY_ucircumflex ,
   KEY_udiaeresis ,
   KEY_yacute ,
   KEY_thorn ,
   KEY_ydiaeresis ,
*/
   KEY_UNKNOWN,

   LAST_KEY    /**< Maximum value to provide enumeration size. */
};

/// Mask values for keyboard modifier keys.
enum ModiferMask
{
   SHIFT_MASK   = (1 << 0),     /**< Shift key mask */
   ALT_MASK     = (1 << 1),     /**< Alt/Option key mask */
   CTRL_MASK    = (1 << 2),     /**< Control (CTRL) key mask */
   COMMAND_MASK = (1 << 3)      /**< Command key mask */
};

/// Mask values for up to seven mouse buttons.
enum ButtonMask
{
   BUTTON1_MASK = (1 << 4),     /**< Value for mouse button 1 */
   BUTTON2_MASK = (1 << 5),     /**< Value for mouse button 2 */
   BUTTON3_MASK = (1 << 6),     /**< Value for mouse button 3 */
   BUTTON4_MASK = (1 << 7),     /**< Value for mouse button 4 */
   BUTTON5_MASK = (1 << 8),     /**< Value for mouse button 5 */
   BUTTON6_MASK = (1 << 9),     /**< Value for mouse button 6 */
   BUTTON7_MASK = (1 << 10)     /**< Value for mouse button 7 */
};

} // End of gadget namespace

#endif /* _GADGET_KEYS_H_ */
