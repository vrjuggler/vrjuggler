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
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/Devices/DriverConfig.h>
#include <vpr/Thread/Thread.h>
#include <vpr/System.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>
#include <gadget/Type/DeviceConstructor.h>

#include <drivers/Immersion/IBox/IBox.h>


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::IBox>(inputMgr);
}

}

namespace gadget
{

/**********************************************************
  void controlLoop(void*)

  The spawned thread just loops from here

*********************************************** ahimberg */
void IBox::controlLoop(void* nullParam)
{
   boost::ignore_unused_variable_warning(nullParam);

   while(!mDoneFlag)
   {
     sample();
   }
}


std::string IBox::getElementType()
{
   return "ibox";
}

/**
 * Configures the IBox.
 */
bool IBox::config(jccl::ConfigElementPtr e)
{
   if(! (Input::config(e) && Analog::config(e) && Digital::config(e)) )
   {
      return false;
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "   IBox::config:" << std::endl
      << vprDEBUG_FLUSH;
   mPortName = e->getProperty<std::string>("port");
   mBaudRate = e->getProperty<int>("baud");

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "   Creating an IBox.. params: " << std::endl
      << "    portnum: " << mPortName << std::endl
      << "        baud   : " << mBaudRate << std::endl
      << "   instanceName: " << mInstName << std::endl << std::endl
      << vprDEBUG_FLUSH;

   return true;
}

/**********************************************************
  IBox::~IBox()

  IBox Destructor, just stop sampling, let other destructors
  do the rest.

*********************************************** ahimberg */
IBox::~IBox()
{
  stopSampling();
}

/**********************************************************
  int IBox::startSampling()

  Starts the Ibox sampling, spawns a thread to query for
  data.

  returns 1 if successful, 0 if it failed or was already sampling

*********************************************** ahimberg */
bool IBox::startSampling()
{
   vpr::ReturnStatus result;

   if (mThread == NULL)
   {
      result = mPhysicalIbox.connect(mPortName, mBaudRate);
      if (result == vpr::ReturnStatus::Succeed)
      {
         mActive = true;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "     Connected to IBox.\n" << std::flush << vprDEBUG_FLUSH;
      } else
      {
         mActive = false;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "   FAILED TO CONNECT to the Ibox named " << mInstName
            << std::endl << "     Ibox settings were: " << std::endl
            << "      port : " << mPortName << std::endl
            << "   mBaudRate: " << mBaudRate << std::endl << std::endl
            << vprDEBUG_FLUSH;
         return 0;
      }
      mPhysicalIbox.std_cmd(0,4,0);
      
      // Set exit flag and spawn sample thread
      mDoneFlag = false;
      vpr::ThreadMemberFunctor<IBox>* memberFunctor = 
         new vpr::ThreadMemberFunctor<IBox>(this, &IBox::controlLoop, NULL);
      mThread = new vpr::Thread(memberFunctor);

      if ( ! mThread->valid() )
      {
         return 0; //fail
      }
      else
      {
         return 1;
      }
   }
   else
      return 0; // already sampling
}

/**********************************************************
  bool IBox::sample()

  IBox Sampler function, tries to get a packet of new stuff,
  when it does it fills up the data and swaps swaps the
  progess/valid array indicies.

  Each call to this function is not guaranteed to result in new data.

*********************************************** ahimberg */
bool IBox::sample()
{
   //struct timeval tv;
   // double start_time, stop_time;
   vpr::ReturnStatus result;
   //int tmp;
   //static int c = 0;
   IboxData cur_reading;

   result = mPhysicalIbox.check_packet();
   if (result == vpr::ReturnStatus::InProgress)
   {
   }
   else if (result == vpr::ReturnStatus::Succeed)
   {
      mPhysicalIbox.std_cmd(0,4,0);
      //    if (c == 0) {
      //      vpr::System::gettimeofday(&tv,0);
      //      start_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;
      //    }
      //    c++;
      //    if (c == 60) {
      //      vpr::System::gettimeofday(&tv,0);
      //      stop_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;
      //      std::cout << 1/((stop_time-start_time) / 60)
      //                << "  " << std::endl;
      //      c = 0;
      //    }

      cur_reading.button[0] = mPhysicalIbox.buttonFunc(0);
      cur_reading.button[1] = mPhysicalIbox.buttonFunc(1);
      cur_reading.button[2] = mPhysicalIbox.buttonFunc(2);
      cur_reading.button[3] = mPhysicalIbox.buttonFunc(3);

      // we really need to do normalization here instead of in getData.
      // need this so we return consistent AnalogData. -cj
      float f;
      this->normalizeMinToMax( mPhysicalIbox.analogFunc(0), f );
      cur_reading.analog[0] = f;
      this->normalizeMinToMax( mPhysicalIbox.analogFunc(1), f );
      cur_reading.analog[1] = f;
      this->normalizeMinToMax( mPhysicalIbox.analogFunc(2), f );
      cur_reading.analog[2] = f;
      this->normalizeMinToMax( mPhysicalIbox.analogFunc(3), f );
      cur_reading.analog[3] = f;

      cur_reading.setTime();

      addAnalogSample(cur_reading.analog);
      addDigitalSample(cur_reading.button);
   }
   return 1;
}

/**********************************************************
  bool IBox::stopSampling()

  Kill the sampling thread if it exists, disconnect from
  the ibox.

*********************************************** ahimberg */
bool IBox::stopSampling()
{
   if( mThread != NULL )
   {
      // Set flag to exit and wait for it exit
      mDoneFlag = true;
      mThread->join();
      delete(mThread);
      mThread = NULL;

      vpr::System::usleep(100);        // 100 uSec pause

      mPhysicalIbox.disconnect();
      std::cout << "stopping the ibox.." << std::endl;
   }

  return 1;
}

/**********************************************************
  void IBox::updateData()

  Swap the current/valid array indicies

*********************************************** ahimberg */
void IBox::updateData()
{
   swapAnalogBuffers();
   swapDigitalBuffers();
   return;
}

} // End of gadget namespace
