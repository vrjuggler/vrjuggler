/////////////////////////////////////////////////////////////////////////////
// vjKeyboard.h
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

#include <Input/vjPosition/vjPosition.h>
#include <Input/vjInput/vjDigital.h>
#include <Input/vjInput/vjAnalog.h>
#include <Input/vjInput/vjKeyboard.h>
#include <Threads/vjThread.h>
class vjConfigChunk;



/** @name type-checked constants for the array indicies to the positional
 *          keyboard controls.
 */
// XXX: This should be #defines so that we don't clutter up global variable space ??
//@{
const int FORWARD = 0;
const int BACK    = 1;
const int LEFT    = 2;
const int RIGHT   = 3;
const int UP      = 4;
const int DOWN    = 5;
const int ROTR    = 6;
const int ROTL    = 7;
const int ROTU    = 8;
const int ROTD    = 9;
const int ROT_ROLL_CCW = 10;
const int ROT_ROLL_CW = 11;
#define NUM_POS_CONTROLS 12      // How many do we need space for??
//@}

//: Keyboard class
// Converts keyboard input into simulated input devices
class vjXWinKeyboard : public vjPosition, public vjDigital, public vjAnalog, public vjKeyboard
{
public:

   vjXWinKeyboard(vjConfigChunk *c);
   vjXWinKeyboard()
   {
      myThread = NULL;
      for (int i =0; i < 256; i++)
         m_realkeys[i] = m_keys[i] = 0;
      m_realkeys[0] = m_keys[0] = 1;
      m_dtrans = 0.1; m_drot = 1;
   }
   ~vjXWinKeyboard() { StopSampling();}

   /* Pure Virtuals required by vjInput */
   int StartSampling();
   int StopSampling();
   int Sample() { return 1;}
   void UpdateData();

   /* vjInput virtual functions
    *
    *  virtual functions that inherited members should
    *  override but are not required to
    */
   char* GetDeviceName() { return "vjKeyboard";}

   /* vjAnalog pure virtual functions */
   int GetAnalogData(int devNum = 0) { return m_anadata[devNum];}

   /* vjDigital pure virtual functions */
   int GetDigitalData(int devNum = 0) { return m_digdata[devNum];}

   /* vjPosition pure virtual functions */
   vjMatrix* GetPosData(int devNum = 0) { return &(m_posdata[devNum]);}

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
   void UpdKeys();
   void MoveFor(float, int);
   void MoveLeft(float, int);
   void MoveUp(float, int);
   void RotUp(float, int);
   void RotLeft(float, int);
   void RotRollCCW(float amt, int n);

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

   vjMatrix     m_posdata[2];       // Local position data
   int          m_anadata[4];       // Local analog data
   int          m_digdata[4];       // Local digital data

   vjThread*  myThread;
   Window       m_window;
   XVisualInfo* m_visual;
   Display*     m_display;
   XSetWindowAttributes m_swa;
   int          m_screen, m_x, m_y;
   unsigned int m_width,m_height;

   /* Keyboard state holders */
   int m_keys[256];     // The keyboard state during an UpdateData, without KeyUp events included
   int m_realkeys[256]; // The real keyboard state, all events processed

   /* Control key holders */
   int m_pos0key[NUM_POS_CONTROLS];
   int m_pos0mod[NUM_POS_CONTROLS];

   int m_pos1key[NUM_POS_CONTROLS];
   int m_pos1mod[NUM_POS_CONTROLS];

   int m_digkeys[4];
   int m_anakeysup[4];
   int m_anakeysdn[4];
   int m_digmods[4];
   int m_anamodsup[4];
   int m_anamodsdn[4];

   /* Movement stuff */
   int m_anastep;

   float m_dtrans, m_drot;
   float m_mouse_sensitivity;
   int m_toggleoff;  // digital data is toggled or just while key is down
};

#endif
