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
#include <vpr/Thread/Thread.h>
#include <vpr/System.h>
#include <vpr/IO/TimeoutException.h>

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

   while (!mDoneFlag)
   {
      mDataUpdated = sample();
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
IBox::~IBox() throw ()
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
   if (mThread != NULL)
   {
      return false;
   }

   try
   {
      mIBox = new IBoxStandalone();
      mIBox->connect(mPortName, mBaudRate);
      mActive = true;
   }
   catch (vpr::Exception& ex)
   {
      mActive = false;
      delete mIBox;
      mIBox = NULL;

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "Failed to connect to the IBox: " << mInstName << std::endl
         << "   port: " << mPortName << std::endl
         << "   baud: " << mBaudRate << std::endl
         << ex.getExtendedDescription() << std::endl
         << vprDEBUG_FLUSH;

      return false;
   }
   
   // Set exit flag and spawn sample thread
   mDoneFlag = false;
   vpr::ThreadMemberFunctor<IBox>* memberFunctor = 
      new vpr::ThreadMemberFunctor<IBox>(this, &IBox::controlLoop, NULL);
   mThread = new vpr::Thread(memberFunctor);

   if ( ! mThread->valid() )
   {
      mIBox->disconnect();
      delete mIBox;
      mIBox = NULL;

      return false;
   }
   else
   {
      return true;
   }
}

/**********************************************************
  bool IBox::sample()

  IBox Sampler function, tries to get a packet of new stuff,
  when it does it fills up the data and swaps swaps the
  progess/valid array indicies.

  Each call to this function is not guaranteed to result in new data.

*********************************************** ahimberg */
bool IBox::sample() //throw (vpr::IOException)
{
   try
   {
      mIBox->sendCommand(0,4,0);
      mIBox->checkForPacket();

      mButton[0] = mIBox->getButtonData(0);
      mButton[1] = mIBox->getButtonData(1);
      mButton[2] = mIBox->getButtonData(2);
      mButton[3] = mIBox->getButtonData(3);

      // we really need to do normalization here instead of in getData.
      // need this so we return consistent AnalogData. -cj
      float f;
      this->normalizeMinToMax( mIBox->getAnalogData(0), f );
      mAnalog[0] = f;
      this->normalizeMinToMax( mIBox->getAnalogData(1), f );
      mAnalog[1] = f;
      this->normalizeMinToMax( mIBox->getAnalogData(2), f );
      mAnalog[2] = f;
      this->normalizeMinToMax( mIBox->getAnalogData(3), f );
      mAnalog[3] = f;

      mButton[0].setTime();
      mButton[1].setTime( mButton[0].getTime() );
      mButton[2].setTime( mButton[0].getTime() );
      mButton[3].setTime( mButton[0].getTime() );
      mAnalog[0].setTime( mButton[0].getTime() );
      mAnalog[1].setTime( mButton[0].getTime() );
      mAnalog[2].setTime( mButton[0].getTime() );
      mAnalog[3].setTime( mButton[0].getTime() );

      addAnalogSample(mAnalog);
      addDigitalSample(mButton);
   }
   catch (vpr::TimeoutException&)
   {
      return false;
   }
   catch (IBoxException& ex)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << ex.getExtendedDescription()
         << vprDEBUG_FLUSH;

      return false;
   }
   return true;
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

      vpr::System::usleep(100);

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "Stopping the IBox."
         << vprDEBUG_FLUSH;

      mIBox->disconnect();
      delete mIBox;
      mIBox = NULL;
   }

  return true;
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
