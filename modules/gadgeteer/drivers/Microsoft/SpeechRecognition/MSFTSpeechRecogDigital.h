/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

//===============================================================
// MSFTSpeechRecog (a Wrapper for MSFTSpeechRecogStandalone)
//===============================================================

#ifndef _GADGET_MSFTSPEECHRECOG_DIGITAL_H_
#define _GADGET_MSFTSPEECHRECOG_DIGITAL_H_

#include <gadget/Devices/DriverConfig.h>

#include <vector>
#include <string>

#include <vpr/Thread/Thread.h>
#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Command.h>

#include <drivers/Microsoft/SpeechRecognition/MSFTSpeechServerManager.h>

namespace gadget
{
   class InputManager;
}

namespace gadget
{

class MSFTSpeechRecogDigital
   : public input_command_t
{
public:
   MSFTSpeechRecogDigital();
   virtual ~MSFTSpeechRecogDigital();

   /** Configures the device with a config chunk. */
   virtual bool config(jccl::ConfigElementPtr c);

   /** Begins sampling. */
   bool startSampling();

   /** Main thread of control for this active object. */
   void controlLoop();

   /** Stops sampling. */
   bool stopSampling();

   /** Samples data. */
   bool sample();

   /** Updates to the sampled data. */
   void updateData();

   /** Returns what chunk type is associated with this class. */
   static std::string getElementType()
   {
      return std::string("msft_speech_recog_digital");
   }

   /** Checks if the device is active. */
   bool isActive()
   {
      return mIsActive;
   }

private:
   std::string mGrammarFileName;

   bool mIsActive;
   bool mIsInitializing;

   gadget::DigitalData mLastSampleToggle;

   MSFTSpeechServerManager* mSpeechManager;
};

} // End of gadget namespace

#endif
