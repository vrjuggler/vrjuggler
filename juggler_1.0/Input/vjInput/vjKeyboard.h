#ifndef _VJ_KEYBOARD_H_
#define _VJ_KEYBOARD_H_

#include <vjConfig.h>
#include <Input/vjInput/vjInput.h>


// --------- VJKEY's ------------ //
// Names of defined keys
const int VJKEY_NONE   = 0;
const int VJKEY_UP     = 1; //65362;
const int VJKEY_DOWN   = 2; // 65361;
const int VJKEY_LEFT   = 3; //65363;
const int VJKEY_RIGHT  = 4; //65364;
const int VJKEY_SHIFT  = 5;
const int VJKEY_CTRL   = 6;
const int VJKEY_ALT    = 7;
const int VJKEY_1      = 8;
const int VJKEY_2      = 9;
const int VJKEY_3      = 10;
const int VJKEY_4      = 11;
const int VJKEY_5      = 12;
const int VJKEY_6      = 13;
const int VJKEY_7      = 14;
const int VJKEY_8      = 15;
const int VJKEY_9      = 16;
const int VJKEY_0      = 17;
const int VJKEY_A      = 18;
const int VJKEY_B      = 19;
const int VJKEY_C      = 20;
const int VJKEY_D      = 21;
const int VJKEY_E      = 22;
const int VJKEY_F      = 23;
const int VJKEY_G      = 24;
const int VJKEY_H      = 25;
const int VJKEY_I      = 26;
const int VJKEY_J      = 27;
const int VJKEY_K      = 28;
const int VJKEY_L      = 29;
const int VJKEY_M      = 30;
const int VJKEY_N      = 31;
const int VJKEY_O      = 32;
const int VJKEY_P      = 33;
const int VJKEY_Q      = 34;
const int VJKEY_R      = 35;
const int VJKEY_S      = 36;
const int VJKEY_T      = 37;
const int VJKEY_U      = 38;
const int VJKEY_V      = 39;
const int VJKEY_W      = 40;
const int VJKEY_X      = 41;
const int VJKEY_Y      = 42;
const int VJKEY_Z      = 43;
const int VJKEY_ESC    = 44;

#define LAST_KEY 44  // Don't forget to change this when new keys are added

const int VJMOUSE_POSX = LAST_KEY + 1;
const int VJMOUSE_NEGX = LAST_KEY + 2;
const int VJMOUSE_POSY = LAST_KEY + 3;
const int VJMOUSE_NEGY = LAST_KEY + 4;
const int VJMBUTTON1   = LAST_KEY + 5;
const int VJMBUTTON2   = LAST_KEY + 6;
const int VJMBUTTON3   = LAST_KEY + 7;



//----------------------------------------------------------------------------
//: vjKeyboard is an abstract class for interfacing with keyboard (and other
//+ keybased) devices.
//
// Informally, a keyboard can be thought of as map of keys to number of times
// prseed since last update.
//
// That is to say, that vjKeyboard counts the number of keypresses between
// updates.  Updates in Juggler occur once per frame.
//-----------------------------------------------------------------------------
//!PUBLIC_API:
class vjKeyboard : virtual public vjInput
{
public:
   vjKeyboard()
   { deviceAbilities = deviceAbilities | DEVICE_KEYBOARD; }

   virtual bool config(vjConfigChunk* chunk)
   { return vjInput::config(chunk); }

      //: Is the given key pressed.
   //! RETURNS: The number of times the key was pressed since last update.
   virtual int keyPressed(int keyId) =0;

   //:Check for the given modifier key pressed only.
   //! RETURNS: true - key pressed exclusively
   virtual bool modifierOnly(int modKey) =0;

   virtual char* GetDeviceName()
   { return "vjKeyboard"; }
};

#endif

