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

#include <gadget/Devices/DriverConfig.h>

#include <boost/bind.hpp>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>

#include <drivers/Microsoft/SpeechRecognition/MSFTSpeechRecogDigital.h>

namespace gadget
{

MSFTSpeechRecogDigital::MSFTSpeechRecogDigital()
   : mIsActive(false)
   , mSpeechManager(MSFTSpeechServerManager::instance())
   , mLastSampleToggle(0)
{
}

bool MSFTSpeechRecogDigital::config(jccl::ConfigElementPtr c)
{
   if(! (Input::config(c) && Command::config(c)))
   {
      return false;
   }

   mGrammarFileName = c->getProperty<std::string>("grammar_file");
   mSpeechManager->addGrammar(mGrammarFileName, this);

   return true;
}

MSFTSpeechRecogDigital::~MSFTSpeechRecogDigital() throw ()
{
   this->stopSampling();
}

// Main thread of control for this active object
void MSFTSpeechRecogDigital::controlLoop()
{
   //Wait for initialization to finish
   // XXX: This should use a condition variable.  -PH 7/10/2004
   while( mIsInitializing )
   {
      vpr::Thread::yield();
   }

   mSpeechManager->startUpdating();

   // Loop through and keep sampling
   while ( this->isActive() )
   {
      this->sample();
   }
}

bool MSFTSpeechRecogDigital::startSampling()
{
   mIsInitializing = true;

   // make sure inertia cubes aren't already started
   if ( this->isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "gadget::MSFTSpeechRecogDigital was already started." << std::endl
         << vprDEBUG_FLUSH;

      mIsInitializing = false;
      return false;
   }

	// Has the thread actually started already
   if(mThread != NULL)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CONFIG_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "gadget::MSFTSpeechRecogDigital: startSampling called, when already sampling.\n"
         << vprDEBUG_FLUSH;
      vprASSERT(false);
   }
   else
   {
      // Create a new thread to handle the sampling control
      mThread =
         new vpr::Thread(boost::bind(&MSFTSpeechRecogDigital::controlLoop,
                                     this));

      if ( ! mThread->valid() )
      {
         mIsInitializing = false;
         return false;  // Fail
      }
      else
      {
         mIsActive = true;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "gadget::MSFTSpeechRecogDigital sampling control thread created."
            << std::endl << vprDEBUG_FLUSH;
         mIsInitializing = false;
         return true;  // Fail
      }

   }
   mIsInitializing = false;

   return false;
}

bool MSFTSpeechRecogDigital::sample()
{
   if (this->isActive() == false)
   {
      return false;
   }

   vpr::Thread::yield();

   //Get the Speech Managers state and set it up as a sample
   std::vector< DigitalData > temp;
   int SpeechManagerStringState( mSpeechManager->getIntegerState(this) );

   mLastSampleToggle.setDigital( SpeechManagerStringState );
   temp.push_back( mLastSampleToggle );

   addCommandSample(temp);

   return true;
}

bool MSFTSpeechRecogDigital::stopSampling()
{
   if (this->isActive() == false)
   {
      return false;
   }

   if (mThread != NULL)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "gadget::MSFTSpeechRecogDigital::stopSampling(): Stopping the sampling thread... "
         << vprDEBUG_FLUSH;

      mIsActive = false;
      mThread->join();
      //mThread->kill();
      delete mThread;
      mThread = NULL;
   }
   mSpeechManager->stopUpdating();

   return true;
}

void MSFTSpeechRecogDigital::updateData()
{
   if (!isActive())
   {
      return;
   }

   mSpeechManager->resetData();
   swapCommandBuffers();
}

}
