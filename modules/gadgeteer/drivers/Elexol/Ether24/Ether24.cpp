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
 * File:          $RCSfile: Ether24.cpp,v $
 * Date modified: $Date: 2006/02/16 02:26:45 $
 * Version:       $Revision: 1.37 $
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/Devices/DriverConfig.h>

#include <boost/bind.hpp>

#include <vpr/vprConfig.h>
#include <vpr/System.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <boost/algorithm/string.hpp>

#include <drivers/Elexol/Ether24/Ether24.h>

extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::Ether24>(inputMgr);
}

}

namespace gadget
{

vpr::Uint8 getByteValue(jccl::ConfigElementPtr elt, std::string base_name)
{
   vpr::Uint8 result(0);
   for (unsigned int i = 0; i < 8; i++)
   {
      std::stringstream ss;
      ss << base_name << i;
      vpr::Uint8 val = elt->getProperty<vpr::Uint8>(ss.str());
      result = result | ((val & 0x1) << i);
   }
   return result;
}

Ether24::Ether24()
   : mDone(false)
   , mInvertA(0)
   , mInvertB(0)
   , mInvertC(0)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "=== Ether24::Ether24() ===" << std::endl << vprDEBUG_FLUSH;
}

std::string Ether24::getElementType()
{
   return "elexol_ether24";
}

bool Ether24::config(jccl::ConfigElementPtr e)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "*** Ether24::config() ***" << std::endl << vprDEBUG_FLUSH;

   // Save config element to use in startSampling()
   mConfigElement = e;

   // Configure the subclasses
   if (!(Input::config(e) && Digital::config(e)))
   {
      return false;
   }
   
   const std::string mac_address = e->getProperty<std::string>("mac_address");
   const std::string ip_address = e->getProperty<std::string>("ip_address");
   const std::string config_method = e->getProperty<std::string>("config_method");

   // If we have a static IP we can just use it directly.
   if ("static_ip" == config_method)
   {
      mAddress.setAddress(ip_address, 2424);
   } // Use MAC address to find device's IP address.
   else if("find_mac" == config_method)
   {
      // Get a map containing all Elexol devices that respond to a
      // broadcast command on port 2424.
      Elexol::device_map_t devices = Ether24Standalone::getDevicesByMacAddress();
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_STATUS_LVL)
         << "Trying to find device with MAC [" << mac_address << "]" << std::endl << vprDEBUG_FLUSH;

      // Try to find the device with the correct MAC address.
      Elexol::device_map_t::const_iterator found = devices.find(mac_address);

      // If we can't find the device.
      if (devices.end() == found)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "ERROR: Could not find Elexol Ether I/O 24 device with MAC address: ["
            << mac_address << "]" << std::endl << vprDEBUG_FLUSH;
         return false;
      }
      else
      {
         mAddress = (*found).second;
      }
   }
   else
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "ERROR: Trying to use unknown config method: [" << config_method
         << "]" << std::endl << vprDEBUG_FLUSH;
      return false;
   }


   return true;
}

Ether24::~Ether24() throw ()
{
   if (isActive())
   {
      stopSampling();
   }
}

// Main thread of control for this active object
void Ether24::controlLoop(void* nullParam)
{
   boost::ignore_unused_variable_warning(nullParam);

   // Loop through and keep sampling until stopSampleing is called.
   while(!mDone)
   {
      this->sample();
   }
}

bool Ether24::startSampling()
{
   // Ensure that we have not already started sampling.
   if (isActive())
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "Ether24 driver was already started." << std::endl
         << vprDEBUG_FLUSH;

      return false;
   }

   // Ensure that the thread has not already started.
   if (NULL != mThread)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " gadget::Ether24::startSampling() called, when already "
         << "sampling.\n" << vprDEBUG_FLUSH;

      return false;
   }

   try
   {
      mDevice.open(mAddress);
      mActive = true;
   }
   catch (Elexol::ElexolException&)
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " [gadget::Ether24::startSampling()] "
         << "Failed to open device." << std::endl << vprDEBUG_FLUSH;

      return false;
   }

   // Set flag that will later allow us to stop the control loop.
   mDone = false;

   // Create a new thread to handle the control
   vpr::ThreadMemberFunctor<Ether24>* run_thread =
      new vpr::ThreadMemberFunctor<Ether24>(this, &Ether24::controlLoop, NULL);

   mThread = new vpr::Thread(run_thread);

   if ( ! mThread->valid() )
   { return false; }


   jccl::ConfigElementPtr porta_elt
      = mConfigElement->getProperty<jccl::ConfigElementPtr>("port_a");
   jccl::ConfigElementPtr portb_elt
      = mConfigElement->getProperty<jccl::ConfigElementPtr>("port_b");
   jccl::ConfigElementPtr portc_elt
      = mConfigElement->getProperty<jccl::ConfigElementPtr>("port_c");

   writeInitialValues(Elexol::PortA, porta_elt);
   writeInitialValues(Elexol::PortB, portb_elt);
   writeInitialValues(Elexol::PortC, portc_elt);

   mInvertA = getByteValue(porta_elt, "invert");
   mInvertB = getByteValue(portb_elt, "invert");
   mInvertC = getByteValue(portc_elt, "invert");

   return true;
}


void Ether24::writeInitialValues(Elexol::Port port, jccl::ConfigElementPtr elt)
{
   // Get the values out of the configuration.
   vpr::Uint8 initial_value = getByteValue(elt, "initial_value");
   vpr::Uint8 direction = getByteValue(elt, "direction");
   vpr::Uint8 pullup = getByteValue(elt, "pullup");
   vpr::Uint8 threshold = getByteValue(elt, "threshold");
   vpr::Uint8 schmitt_trigger = getByteValue(elt, "schmitt_trigger");

   // Write them to the device.
   mDevice.setValue(port, initial_value);
   mDevice.setDirection(port, direction);
   mDevice.setPullUp(port, pullup);
   mDevice.setThreshold(port, threshold);
   mDevice.setSchmittTrigger(port, schmitt_trigger);
}

bool Ether24::sample()
{
   // If we are not active, then don't try to sample.
   if (!isActive())
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << clrSetNORM(clrRED) << "Ether24 ("
         << mInstName << ") NOT ACTIVE IN SAMPLE\n"
         << clrRESET << vprDEBUG_FLUSH;
      return false;
   }

   // Create the data buffers to put the new data into.
   std::vector<gadget::DigitalData>  cur_digital_samples;

   std::vector<vpr::Uint8> port_values(0);
   try
   {
      // Use XOR to invert specified bits.
      port_values.push_back(mInvertA ^ mDevice.getValue(Elexol::PortA));
      port_values.push_back(mInvertB ^ mDevice.getValue(Elexol::PortB));
      port_values.push_back(mInvertC ^ mDevice.getValue(Elexol::PortC));
   }
   catch (Elexol::ElexolTimeoutException& ex)
   {
      return false;
   }
   catch (Elexol::ElexolException& ex)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "gadget::Ether24::sample() caught exception: ")
         << ex.what() << std::endl << vprDEBUG_FLUSH;
      return false;
   }
   catch (...)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "gadget::Ether24::sample() caught unknown exception")
         << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   for (unsigned int i = 0; i < port_values.size(); i++)
   {
      for (unsigned int j = 0 ; j < 8 ; j++)
      {
         cur_digital_samples.push_back(DigitalData(port_values[i] & (1<<j)));
      }
   }

   // Set the time for each digital value to be the same.
   if ( ! cur_digital_samples.empty() )
   {
      cur_digital_samples[0].setTime();

      for ( unsigned int i = 0; i < cur_digital_samples.size(); ++i )
      {
         cur_digital_samples[i].setTime( cur_digital_samples[0].getTime() );
      }
   }

   // Lock and then swap the buffers.
   addDigitalSample(cur_digital_samples);

   return true;
}

bool Ether24::stopSampling()
{
   // Make sure that we are sampling in the first place.
   if (!this->isActive())
   {
      return false;
   }

   if (mThread != NULL)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "[gadget::Ether24::stopSampling()] Stopping the control thread..."
         << std::endl << vprDEBUG_FLUSH;

      // Set the done flag to attempt to cause the control loop to exit
      // naturally.
      mDone = true;

      mThread->join();
      delete mThread;
      mThread = NULL;

      // Close connection to device
      mDevice.close();

      // Driver is no longer active.
      mActive = false;

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "*** Ether24 has been shutdown. ***" << std::endl
         << vprDEBUG_FLUSH;
   }

   return true;
}

void Ether24::updateData()
{
   if (isActive())
   {
      swapDigitalBuffers();
   }
}

} // End of gadget namespace
