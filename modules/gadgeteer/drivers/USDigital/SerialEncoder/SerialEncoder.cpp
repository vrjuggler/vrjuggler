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
#include <gmtl/Generate.h>
#include <gadget/gadgetParam.h>
#include <gadget/Type/Position/PositionUnitConversion.h>

#include "SerialEncoder.h"


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<SerialEncoder>(inputMgr);
}

}

SerialEncoder::SerialEncoder()   
   : mSampleThread(NULL)
   , mExitFlag(false)
   , mBaudRate(0)
{
   mSerialEncoder = new SerialEncoderStandalone;
   /* Do nothing. */
}

SerialEncoder::~SerialEncoder() throw ()
{
   this->stopSampling();
   delete mSerialEncoder;
}

bool SerialEncoder::config(jccl::ConfigElementPtr e)
{
   if ( !(gadget::Input::config(e) && gadget::Position::config(e)) )
   {
      return false;
   }

   mPortStr=e->getProperty<std::string>("port");
   mBaudRate=e->getProperty<long>("baud");

   mSerialEncoder->setPort(mPortStr);
   mSerialEncoder->setBaudRate(mBaudRate);
   return mSerialEncoder->initializeDevice();
}

bool SerialEncoder::startSampling()
{
   mExitFlag = false;
   mSampleThread =
      new vpr::Thread(boost::bind(&SerialEncoder::threadedSampleFunction,
                                  this));

   if ( !mSampleThread->valid() )
   {
      return false; // thread creation failed
   }
   else
   {
      return true; // thread creation success
   }
}

bool SerialEncoder::sample()
{
   std::vector<gadget::PositionData> positionSample(1);

   positionSample[0].setTime();

   positionSample[0].mPosData = mSerialEncoder->getSample();

   addPositionSample(positionSample);

   return true;
}

bool SerialEncoder::stopSampling()
{
   if ( mSampleThread!=NULL )
   {
      mExitFlag = true;
      mSampleThread->join();
      delete mSampleThread;
      mSampleThread=NULL;
   }

   return true;
}

void SerialEncoder::threadedSampleFunction()
{
   while ( !mExitFlag )
   {
      sample();
      vpr::System::msleep(1);
   }
}
