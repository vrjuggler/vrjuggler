/////////////////////////////////////////////////////////////////////////////
// vjDummyKeyboard.h
//
// Dummy Keyboard device
//
// History:
//
// Allen Bierbaum:    v0.0 - 2-1-99 - Inital version
////////////////////////////////////////////////////////////////////////////


#ifndef _VJ_DUMMYKEYBOARD_H_
#define _VJ_DUMMYKEYBOARD_H_

#include <vjConfig.h>
#include <Input/vjInput/vjKeyboard.h>

//: A dummy device for the Keyboard proxies to connect to
class vjDummyKeyboard : public vjKeyboard
{
public:
   vjDummyKeyboard() : vjKeyboard()
   { active = 1;}

   vjDummyKeyboard(vjConfigChunk *chunk) : vjKeyboard(chunk)
   { active = 1; }

   //: Is the given key pressed.
   //! RETURNS: The number of times the key was pressed since last update.
   int keyPressed(int keyId)
   { return 0; }

   //:Check for the given modifier key pressed only.
   //! RETURNS: true - key pressed exclusively
   bool modifierOnly(int modKey)
   { return false; }

   char* GetDeviceName()
   { return "vjDummyKeyboard"; }

   /* These functions don't do anything */
   int StartSampling() { return 1; }
	int StopSampling() { return 0; }
	int Sample() { return 1;}
	void UpdateData() { return ; }
};


#endif
