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

#include <string>
#include <boost/bind.hpp>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/AxisAngle.h>

#include <vpr/System.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/gadgetParam.h>
#include <drivers/EssentialReality/P5Glove/P5GloveStandalone.h> /* standalone dataglove driver */
#include <drivers/EssentialReality/P5Glove/P5GloveWrapper.h> /* Gadgeteer dataglove driver */


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::P5GloveWrapper>(inputMgr);
}

}

namespace gadget
{

P5GloveWrapper::P5GloveWrapper()
   : mGlove(NULL)
   , mAnalogP5(5, 0.0f)
   , mDigitalP5(3)
   , mPositionP5(1)
   , mExitFlag(false)
{
   /* Do nothing. */ ;
}

P5GloveWrapper::~P5GloveWrapper()
{
   stopSampling();      // Stop the glove

   if ( NULL != mGlove )
   {
      delete mGlove;       // Delete the glove
      mGlove = NULL;
   }
}

bool P5GloveWrapper::config(jccl::ConfigElementPtr e)
{
   if ( ! (Input::config(e) /*&& Glove::config(e)*/ ) )
   {
      return false;
   }

   mGloveNumber = e->getProperty<int>("number");

   vprASSERT(mThread == NULL);      // This should have been set by Input(c)
   mGlove = new P5GloveStandalone();
   return true;
}

std::string P5GloveWrapper::getElementType()
{
   return "p5_glove";
}

bool P5GloveWrapper::startSampling()
{
   bool started(false);

   if ( mThread == NULL )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "[p5glove] Begin sampling\n" << vprDEBUG_FLUSH;
   
      int max_attempts(0);
      bool result(false);

      while ( result == false && max_attempts < 5 )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
            << "[p5glove] Connecting to glove number " << mGloveNumber
            << "...\n" << vprDEBUG_FLUSH;
         result = mGlove->connectToHardware(mGloveNumber);

         if ( result == false )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "[p5glove] ERROR: Can't open or it is already opened."
               << vprDEBUG_FLUSH;
            vpr::System::usleep(14500);
            ++max_attempts;
         }
      }

      if ( result )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
            << "[p5glove] Successfully connected, Now sampling dataglove "
            << "data.\n" << vprDEBUG_FLUSH;

         // Create a new thread to handle the control and set exit flag to
         // false.
         mExitFlag = false;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
            << "[p5glove] Spawning control thread.\n" << vprDEBUG_FLUSH;

         try
         {
            mThread =
               new vpr::Thread(boost::bind(&P5GloveWrapper::controlLoop,
                                           this));
            mActive = true;
            started = true;

            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
               << "[p5glove] P5Glove is active.\n" << vprDEBUG_FLUSH;
         }
         catch (vpr::Exception& ex)
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << clrOutBOLD(clrRED, "ERROR")
               << ": Failed to spawn thread for P5 Glove driver!\n"
               << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << ex.what() << std::endl << vprDEBUG_FLUSH;
            started = false;
         }
      }
   }

   return started;
}

void P5GloveWrapper::controlLoop()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "[p5glove] Entered control thread\n" << vprDEBUG_FLUSH;

   // Go until mExitFlag is set to true
   while ( ! mExitFlag )
   {
      sample();
   }
}

bool P5GloveWrapper::sample()
{
   P5GloveStandalone::Record rec;
   bool sample_read = mGlove->readRecordsFromHardware(rec);

   if ( sample_read )
   {
// Furst, we set the flexion of the gloves
      mAnalogP5[0] = rec.thumb;     // Thumb (0.0 - 1.0)
      mAnalogP5[1] = rec.index;     // Index
      mAnalogP5[2] = rec.middle;    // Middle
      mAnalogP5[3] = rec.ring;     // Ring
      mAnalogP5[4] = rec.pinky;    // Pinky
      addAnalogSample(mAnalogP5);
      swapAnalogBuffers();

// Then, we define the buttons of the glove
      mDigitalP5[0] = rec.buttonA;
      mDigitalP5[1] = rec.buttonB;
      mDigitalP5[2] = rec.buttonC;
      addDigitalSample(mDigitalP5);
      swapDigitalBuffers();

// Finally, we set the position of the glove ...
      const gmtl::AxisAnglef rotation(rec.rotationAngle, rec.rotationAxis[0],
                                      rec.rotationAxis[1],
                                      rec.rotationAxis[2]);
      const gmtl::Vec3f translation(static_cast<float>(rec.position[0]),
                                    static_cast<float>(rec.position[1]),
                                    static_cast<float>(rec.position[2]));
      gmtl::Matrix44f position = gmtl::makeTrans<gmtl::Matrix44f>(translation);
      position = position * gmtl::make<gmtl::Matrix44f>(rotation);
      mPositionP5[0].mPosData = position; 
      addPositionSample(mPositionP5);
      swapPositionBuffers();
   }

   return true;
}

void P5GloveWrapper::updateData()
{
   swapAnalogBuffers();
   swapDigitalBuffers();
   swapPositionBuffers();
}

bool P5GloveWrapper::stopSampling()
{
   if ( mThread != NULL )
   {
      mExitFlag = true;
   
      //Wait for thread to exit normally after setting exit flag
      mThread->join();
      delete mThread;
      mThread = NULL;

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "[p5glove] stopping P5Glove.." << std::endl << vprDEBUG_FLUSH;
   }

   return true;
}

} // End of gadget namespace
