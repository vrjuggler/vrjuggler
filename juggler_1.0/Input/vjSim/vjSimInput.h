//-------------------------------------
// vjSimInput.h
//------------------------------------

#ifndef _VJ_SIM_INPUT_H
#define _VJ_SIM_INPUT_H
#pragma once


#include <vjConfig.h>
#include <Input/InputManager/vjKeyboardInterface.h>


//: Base class for all simulated input devices
//
// Has the common routines for accessing and seting
// up keyboard interface
//
public class vjSimInput
{
protected:
   class vjKeyModPair
   {
   public:
      int mKey;
      int mModifier;
   }

public:
   void  vjSimInput(vjConfigChunk* chunk);

protected:
   //: Check the given keypair
   //! RETURNS: Number of times the key was pressed
   //           with the modifier alone held down
   int  checkKeyPair(vjKeyModPair& pair);

   //: Get number of time key pressed
   //! RETURNS: Number of time the key is pressed
   int   checkKey(int keyId);


private:
   vjKeyboardInterface mKeyboard;
}
