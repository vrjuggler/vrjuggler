/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <gadget/Devices/DriverConfig.h>

#include <vector>
#include <boost/bind.hpp>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>

#include <drivers/Microsoft/SpeechRecognition/MSFTSpeechRecogString.h>

namespace gadget
{

MSFTSpeechRecogString::MSFTSpeechRecogString()
   : mIsActive(false)
   , mSpeechManager(MSFTSpeechServerManager::instance())
{
}

bool MSFTSpeechRecogString::config(jccl::ConfigElementPtr c)
{
   if(! (Input::config(c) && String::config(c)))
   {
      return false;
   }

   mGrammarFileName = c->getProperty<std::string>("grammar_file");
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "mGrammarFileName: " << mGrammarFileName << std::endl
      << vprDEBUG_FLUSH;
   mSpeechManager->addGrammar(mGrammarFileName, this);

   return true;
}

MSFTSpeechRecogString::~MSFTSpeechRecogString()
{
   this->stopSampling();
}

// Main thread of control for this active object
void MSFTSpeechRecogString::controlLoop()
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

bool MSFTSpeechRecogString::startSampling()
{
   mIsInitializing = true;

   // make sure inertia cubes aren't already started
   if ( this->isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "gadget::MSFTSpeechRecogString was already started." << std::endl
         << vprDEBUG_FLUSH;

      mIsInitializing = false;
      return false;
   }

   // Has the thread actually started already
   if(mThread != NULL)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CONFIG_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " gadget::MSFTSpeechRecogString: startSampling called, when already sampling.\n"
         << vprDEBUG_FLUSH;
      vprASSERT(false);
   }
   else
   {
      // Create a new thread to handle the sampling control
      try
      {
         mThread =
            new vpr::Thread(boost::bind(&MSFTSpeechRecogString::controlLoop,
                                       this));
         mIsActive = true;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "gadget::MSFTSpeechRecogString sampling control thread created."
            << std::endl << vprDEBUG_FLUSH;
         mIsInitializing = false;
         return true;   // success
      }
      catch (vpr::Exception& ex)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR")
            << ": Failed to spawn thread for MS Speech String driver!\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << ex.what() << std::endl << vprDEBUG_FLUSH;

         mIsInitializing = false;
         return false;  // Fail
      }
   }
   mIsInitializing = false;

   return false;
}

bool MSFTSpeechRecogString::sample()
{
   if (this->isActive() == false)
   {
      return false;
   }

   vpr::Thread::yield();

   //Get the Speech Managers state and set it up as a sample
   std::vector< StringData > temp;
   StringData SpeechManagerStringState( mSpeechManager->getStringState(this) );

   temp.push_back( SpeechManagerStringState );

   addStringSample(temp);

   return true;
}

bool MSFTSpeechRecogString::stopSampling()
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

void MSFTSpeechRecogString::updateData()
{
   if (!isActive())
   {
      return;
   }

   mSpeechManager->resetData();
   swapStringBuffers();
}

}
