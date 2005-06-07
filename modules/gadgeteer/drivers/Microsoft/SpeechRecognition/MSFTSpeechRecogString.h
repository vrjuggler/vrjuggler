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
 * File:          MSFTSpeechRecog.h,v
 * Date modified: 2004/03/05 16:53:41
 * Version:       1.32
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

//===============================================================
// MSFTSpeechRecog (a Wrapper for MSFTSpeechRecogStandalone)
//===============================================================

#ifndef _GADGET_MSFTSPEECHRECOG_STRING_H_
#define _GADGET_MSFTSPEECHRECOG_STRING_H_

#include <gadget/Devices/DriverConfig.h>

#include <string>

#include <vpr/Thread/Thread.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/String.h>
#include <gadget/Type/InputMixer.h>

#include <drivers/Microsoft/SpeechRecognition/MSFTSpeechServerManager.h>

namespace gadget
{
   class InputManager;
}

namespace gadget
{

class MSFTSpeechRecogString
   : public gadget::InputMixer<gadget::Input, gadget::String>
{
public:
   MSFTSpeechRecogString();
   virtual ~MSFTSpeechRecogString();

   /** Configures the device with a config chunk. */
   virtual bool config(jccl::ConfigElementPtr c);

   /** Begins sampling. */
   bool startSampling();

   /** Main thread of control for this active object. */
   void controlLoop(void* nullParam);

   /** Stops sampling. */
   bool stopSampling();

   /** Samples data. */
   bool sample();

   /** Updates to the sampled data. */
   void updateData();

   /** Returns what chunk type is associated with this class. */
   static std::string getElementType()
   {
      return std::string("msft_speech_recog_string");
   }

   const StringData getStringData(int devNum = 0)
   {
      std::cout << "MSFTSpeechRecogString getStringData()" << std::endl;
   }

   /** Checks if the device is active. */
   bool isActive()
   {
      return mIsActive;
   }

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   virtual void destroy()
   {
      delete this;
   }

private:
   std::string mGrammarFileName;

   bool mIsActive;
   bool mIsInitializing;

   MSFTSpeechServerManager* mSpeechManager;
};

} // End of gadget namespace

#endif
