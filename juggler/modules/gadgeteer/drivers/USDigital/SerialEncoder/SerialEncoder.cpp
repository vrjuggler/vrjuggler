/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <gmtl/Generate.h>
#include <gadget/Type/Position/PositionUnitConversion.h>

#include "SerialEncoder.h"

void initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<SerialEncoder>(inputMgr);
}

SerialEncoder::SerialEncoder()   
{
   mExitFlag = false;
   mSampleThread=0;
   mBaudRate=0;
}

SerialEncoder::~SerialEncoder()   
{
   this->stopSampling();
}

bool SerialEncoder::config(jccl::ConfigElementPtr e)
{
   if ( !(gadget::Input::config(e) && gadget::Position::config(e)) )
   {
      return false;
   }

   mPortStr=e->getProperty<std::string>("port");
   mBaudRate=e->getProperty<long>("baud");

   return true;
}

bool SerialEncoder::startSampling()
{
   mBus.initializeSEI(mPortStr.c_str());
   mExitFlag = false;
   mSampleThread=new vpr::Thread(threadedSampleFunction,(void*)this);

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
   unsigned int tiltDevice=1;
   unsigned int heightDevice=2;

   std::vector<gadget::PositionData> positionSample(1);

   positionSample[0].setTime();

   static float tableRotation=0;
   mBus.ED2GetPosition1(tiltDevice,tableRotation);
   gmtl::EulerAngleXYZf eulerRot(gmtl::Math::deg2Rad(tableRotation),0,0);
   gmtl::Matrix44f rotation;
   gmtl::setRot(rotation,eulerRot);

   float tableHeight=0;
   mBus.ED2GetPosition1(heightDevice,tableHeight);
   tableHeight=tableHeight/gadget::PositionUnitConversion::ConvertToInches;
   gmtl::Vec3f vecTrans;
   vecTrans[1]+=tableHeight;
   gmtl::Matrix44f translation;
   gmtl::setTrans(translation,vecTrans);

   positionSample[0].mPosData=translation*rotation;

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

void SerialEncoder::threadedSampleFunction(void* classPointer)
{
   SerialEncoder *this_ptr=static_cast<SerialEncoder*>(classPointer);

   while ( !mExitFlag )
   {
      this_ptr->sample();
      vpr::System::msleep(1);
   }
}

