/////////////////////////////////////////////////////////////////////////////
// vjXWinKeyboard.h
//
// Keyboard input device, simulates 2 positional, 4 digital, and 4 analog
//          devices.
//
// History:
//
// Andy Himberger:    v0.0 - 12-1-97 - Inital version
////////////////////////////////////////////////////////////////////////////
#ifndef _VJ_XWIN_KEYBOARD_H_
#define _VJ_XWIN_KEYBOARD_H_

#include <vjConfig.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include <Input/vjInput/vjKeyboard.h>
class vjConfigChunk;

//---------------------------------------------------------------
//: XWin Keyboard class
// Converts keyboard input into simulated input devices
//
// This device is a source of keyboard events.  The device should not be
// used directly, it should be referenced through proxies.
//
// See also: vjKeyboard vjKeyboardProxy
//--------------------------------------------------------------
class vjXWinKeyboard : public vjKeyboard
{
public:

   vjXWinKeyboard()
   {
      //myThread = NULL; -- Should be done in base constructore
      oldMouseX = 0; oldMouseY = 0;

      // -1 is used to indicate that these are not grabbed.  A value of
      // GrabSuccess (returned by XGrab{Pointer,Keyboard} upon successful
      // grabbing) is used to indicate that they are grabbed.  BadValue might
      // be a better choice for this.
      m_keyboard_grabbed = -1;
      m_pointer_grabbed = -1;
   }
   ~vjXWinKeyboard() { StopSampling();}

   virtual bool config(vjConfigChunk* c);

   /* Pure Virtuals required by vjInput */
   int StartSampling();
   int StopSampling();
   int Sample() { return 1;}
   void UpdateData();

   char* GetDeviceName() { return "vjXwinKeyboard";}
   static std::string getChunkType() { return std::string("Keyboard");}

   // returns the number of times the key was pressed during the
   // last frame, so you can put this in an if to check if was
   // pressed at all, or if you are doing processing based on this
   // catch the actual number..
   int IsKeyPressed(int vjKey)
   {  return m_keys[vjKey];}

   virtual int keyPressed(int keyId)
   { return IsKeyPressed(keyId); }

   virtual bool modifierOnly(int modKey)
   { return OnlyModifier(modKey); }

private:
   /* Private functions for processing input data */
   int OnlyModifier(int);

   //: Update the keys.
   void UpdKeys();

   /* X-Windows utility functions */
   //: Convert XKey to vjKey
   //! NOTE: Keypad keys are transformed ONLY to number keys
   int XKeyTovjKey(KeySym xKey);
   int FilterEvent( XEvent* event, int want_exposes,
                    int width, int height);
   char* CheckArgs(char* look_for);
   void CheckGeometry();
   Window CreateWindow (Window parent, unsigned int border, unsigned long
                        fore, unsigned long back, unsigned long event_mask);
   void SetHints(Window window, char*  window_name, char*  icon_name,
                 char* class_name, char* class_type);


   Window       m_window;
   XVisualInfo* m_visual;
   Display*     m_display;
   XSetWindowAttributes m_swa;
   int          m_screen, m_x, m_y;
   unsigned int m_width,m_height;

   /* Keyboard state holders */
   int m_keys[256];     // The keyboard state during an UpdateData, without KeyUp events included
   int m_realkeys[256]; // The real keyboard state, all events processed

   /*
    * State holders for whether or not the keyboard and/or mouse are grabbed.
    */
   int m_keyboard_grabbed;	// The keyboard grab state
   int m_pointer_grabbed;	// The mouse pointer grab state

   float m_mouse_sensitivity;
   int   oldMouseX, oldMouseY;
};

#endif
