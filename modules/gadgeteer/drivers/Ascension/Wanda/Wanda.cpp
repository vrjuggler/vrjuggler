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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/Devices/DriverConfig.h>

#include <boost/bind.hpp>

#include <vpr/Thread/Thread.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>

#include <jccl/Config/ConfigElement.h>

#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>
#include <gadget/Type/DeviceConstructor.h>

#include <drivers/Ascension/Wanda/WandaExceptions.h>
#include <drivers/Ascension/Wanda/Wanda.h>


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::Wanda>(inputMgr);
}

}

namespace gadget
{

Wanda::Wanda(const char* portName)
   : mThread(NULL)
   , mWanda(portName)
{
   /* Do nothing. */ ;
}

Wanda::~Wanda() throw ()
{
   stopSampling();
}

std::string Wanda::getElementType()
{
   return "wanda";
}

bool Wanda::config(jccl::ConfigElementPtr elt)
{
   vprASSERT(elt->getID() == getElementType() &&
             "Invalid config element type given");

   if ( ! (Input::config(elt) && Analog::config(elt) && Digital::config(elt)) )
   {
      return false;
   }

   setMin(wanda::WandaStandalone::ANALOG_MIN);
   setMax(wanda::WandaStandalone::ANALOG_MAX);

   const unsigned int cur_version(1);
   bool status(true);

   if ( elt->getVersion() < cur_version )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR") << " [Wanda::config()] Element named '"
         << elt->getName() << "'" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "is version " << elt->getVersion()
         << ", but we require at least version " << cur_version << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "Ignoring this element and moving on." << std::endl
         << vprDEBUG_FLUSH;
      status = false;
   }
   else
   {
      mWanda.setPortName(elt->getProperty<std::string>("port"));
      const unsigned int timeout =
         elt->getProperty<unsigned int>("read_timeout");

      if ( timeout > 0 )
      {
         mWanda.setTimeout(vpr::Interval(timeout, vpr::Interval::Msec));
      }
   }

   return status;
}

bool Wanda::startSampling()
{
   if ( isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::Wanda was already started." << std::endl
         << vprDEBUG_FLUSH;
      return false;
   }

   bool status(false);

   if ( NULL == mThread )
   {
      try
      {
         mWanda.start();

         mExitFlag = false;
         mThread   = new vpr::Thread(boost::bind(&Wanda::controlLoop, this));
         status    = mThread->valid();
      }
      catch (vpr::Exception& ex)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR") << ": Failed to start Wanda:\n"
            << ex.what() << std::endl << vprDEBUG_FLUSH;
      }
   }

   return status;
}

bool Wanda::sample()
{
   if ( ! isActive() )
   {
      return false;
   }

   bool status(false);

   try
   {
      mWanda.sample();

      std::vector<gadget::AnalogData>  analog_samples(2);
      std::vector<gadget::DigitalData> digital_samples(3);

      // Get an initial timestamp for the entire sample. We will copy it into
      // each sample buffer later.
      analog_samples[0].setTime();

      float x_axis, y_axis;
      normalizeMinToMax(mWanda.getXAxis(), x_axis);
      normalizeMinToMax(mWanda.getYAxis(), y_axis);

      analog_samples[0].setAnalog(x_axis);
      analog_samples[1].setAnalog(y_axis);
      analog_samples[1].setTime(analog_samples[0].getTime());

      for ( size_t i = 0; i < 3; ++i )
      {
         digital_samples[i].setDigital(mWanda.getButton(i));
         digital_samples[i].setTime(analog_samples[0].getTime());
      }

      addAnalogSample(analog_samples);
      addDigitalSample(digital_samples);

      status = true;
   }
   catch (vpr::Exception& ex)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "Caught exception while trying to sample Wanda:\n" << ex.what()
         << std::endl << vprDEBUG_FLUSH;
   }

   return status;
}

bool Wanda::stopSampling()
{
   bool status(false);

   if ( isActive() )
   {
      if ( NULL != mThread )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "Stopping the Wanda sample thread...\n" << vprDEBUG_FLUSH;

         mExitFlag = true;
         mThread->join();

         delete mThread;
         mThread = NULL;
      }

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "Stopping the Wanda device... " << vprDEBUG_FLUSH;

      try
      {
         mWanda.stop();

         // Make sure that the Wanda driver really did stop.
         if ( isActive() )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "Wanda did not stop!" << std::endl << vprDEBUG_FLUSH;
         }
         else
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
               << "stopped.\n" << vprDEBUG_FLUSH;
            status = true;
         }
      }
      catch (wanda::WandaException& ex)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "Wanda did not stop:\n" << ex.what() << std::endl
            << vprDEBUG_FLUSH;
      }
   }

   return status;
}

void Wanda::updateData()
{
   if ( isActive() )
   {
      swapAnalogBuffers();
      swapDigitalBuffers();
   }
}

void Wanda::controlLoop()
{
   while ( ! mExitFlag )
   {
      sample();
   }
}

}
