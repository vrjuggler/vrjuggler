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

#ifndef _GADGET_USDIGITAL_SERIAL_ENCODER_H
#define _GADGET_USDIGITAL_SERIAL_ENCODER_H

#include <gadget/Devices/DriverConfig.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Guard.h>

#include <gadget/Type/Input.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/DeviceConstructor.h>
#include <jccl/Config/ConfigElementPtr.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/InputMixer.h>

#include "SerialEncoderStandalone.h"
#include "SEIBus.h"


class SerialEncoder:public gadget::InputMixer<gadget::Input,gadget::Position>
{
public:
   SerialEncoder();
   virtual ~SerialEncoder();

   static std::string getElementType()
   {
      return std::string("serial_encoder");
   }

   bool config(jccl::ConfigElementPtr e);

   bool startSampling();
   bool sample();
   bool stopSampling();

   void updateData()
   {
      swapPositionBuffers();
   }

   void threadedSampleFunction();

   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   virtual void destroy()
   {
      delete this;
   }

private:
   vpr::Thread* mSampleThread;
   bool  mExitFlag;
   std::string mPortStr;
   long mBaudRate;
   SerialEncoderStandalone* mSerialEncoder;
};

#endif

