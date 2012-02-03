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

#include <gadget/Devices/DriverConfig.h>

#include <boost/bind.hpp>

#include <vpr/vprConfig.h>
#include <vpr/System.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Quat.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <drivers/Open/TUIO/Tuio.h>

extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::Tuio>(inputMgr);
}

}

namespace gadget
{

/**
* Constructor.
*/
Tuio::Tuio()
 : mDone(false)
{
   mAnalog.push_back(0.0f);
   mDigital.push_back(gadget::DigitalState::OFF);
}

/**
* Destructor.
*/
Tuio::~Tuio()
{
   this->stopSampling();
}

std::string Tuio::getElementType()
{
   return "TUIO_drv";
}

bool Tuio::config(jccl::ConfigElementPtr e)
{
   if (!gadget::Digital::config(e)) return false;
   if (!gadget::Analog::config(e))  return false;
   if (!gadget::Input::config(e))   return false;

   // Get configuration attribute(s).
   mPort = e->getProperty<int>("port");
   mAnalog.resize(2*e->getProperty<int>("points"));
   mDigital.resize(e->getProperty<int>("points"));

   setMin(0.0f);
   setMax(1.0f);

   return true;
}

/** Main thread of control for this active object. */
void Tuio::controlLoop()
{  
   // Loop through and keep sampling until stopSampling is called.
   while (!mDone)
   {
      this->sample();

      // Limit sampling to 100 Hz
      vpr::System::msleep(10);
   }
}

bool Tuio::startSampling()
{
   // Ensure that we have not already started sampling.
   if (this->isActive())
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "Tuio driver was already started." << std::endl
         << vprDEBUG_FLUSH;
      return false;
   }

   // Ensure that the thread has not already started.
   if (mThread != NULL)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " gadget::Tuio::startSampling() called, when already "
         << "sampling.\n" << vprDEBUG_FLUSH;
      return false;
   }

   // Initialize data
   for (unsigned int i = 0; i < mDigital.size(); i++)
   {
      mDigital[i].setValue(gadget::DigitalState::OFF);
      mDigital[i].setTime();
      
      mAnalog[2*i].setValue(0.0f);
      mAnalog[2*i].setTime();
      mAnalog[2*i + 1].setValue(0.0f);
      mAnalog[2*i + 1].setTime();
   }
   addDigitalSample(mDigital);
   addAnalogSample(mAnalog);

   // Open the connection to the tracker
   mTracker.open(mPort);
   if (!this->isActive())
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " [gadget::Tuio::startSampling()] mTracker.open() "
         << "failed to connect to tracker.\n" << vprDEBUG_FLUSH;
      return false;
   }

   // Set flag that will later allow us to stop the control loop.
   mDone = false;

   bool started(true);

   // Create a new thread to handle the control
   try
   {
      mThread = new vpr::Thread(boost::bind(&Tuio::controlLoop, this));
   }
   catch (vpr::Exception& ex)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": Failed to spawn thread for Tuio driver!\n"
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << ex.what() << std::endl << vprDEBUG_FLUSH;
      started = false;
   }

   return started;
}

bool Tuio::sample()
{
   // If we are not active, then don't try to sample.
   if (!isActive())
   {
      return false;
   }

   // Check to see if we have new data to pull
   bool receivedData = false;
   while (mTracker.updateData())
   {
      receivedData = true;
   }
   
   if (!receivedData)
   {
      return false;
   }

   std::list<TuioPoint*>points = mTracker.points();

   for (unsigned int i = 0; i < mDigital.size(); i++)
   {
      mDigital[i] = gadget::DigitalState::OFF;
      mDigital[i].setTime();

      mAnalog[2*i].setValue(0.0f);
      mAnalog[2*i].setTime();

      mAnalog[2*i + 1].setValue(0.0f);
      mAnalog[2*i + 1].setTime();
   }

   for (std::list<TuioPoint*>::iterator pointIter = points.begin();
        pointIter != points.end();
        pointIter++)

   {
      unsigned int cursorID = (*pointIter)->getPointID();

      // Check if we have more points than configured for
      if (cursorID >= mDigital.size())
      {
         break;
      }
	  
      mDigital[cursorID] = gadget::DigitalState::ON;
      mDigital[cursorID].setTime();
      
      // Set first analog to x value
      mAnalog[2*cursorID].setValue((*pointIter)->getXpos());
      mAnalog[2*cursorID].setTime();

      // Set second analog to y value
      mAnalog[2*cursorID + 1].setValue((*pointIter)->getYpos());
      mAnalog[2*cursorID + 1].setTime();
   }

   addDigitalSample(mDigital);
   addAnalogSample(mAnalog);
    
   return true;
}

bool Tuio::stopSampling()
{  
   // Make sure that we are sampling in the first place.
   if (!isActive())
   {
      mDone = true; // just to be safe 
      return false;
   }

   if (mThread != NULL)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "[gadget::Tuio::stopSampling()] Stopping the Tuio "
         << "thread...\n" << vprDEBUG_FLUSH;

      // Set the done flag to attempt to cause the control loop to exit
      // naturally.
      mDone = true;

      mThread->join();
      delete mThread;
      mThread = NULL;

      // Close the connection to the tracker
      mTracker.close();

      // If the tracker failed to stop
      if (isActive() == true)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED,"\nERROR:")
            << " [gadget::Tuio::stopSampling()] Tuio tracker "
            << "failed to stop.\n" << vprDEBUG_FLUSH;
         return false;
      }

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "*** Tuio has been shutdown. ***" << std::endl
         << vprDEBUG_FLUSH;
   }
   
   return true;
}

void Tuio::updateData()
{  
   if (isActive())
   {
      swapAnalogBuffers();
      swapDigitalBuffers();
   }
}

} // End of gadget namespace
