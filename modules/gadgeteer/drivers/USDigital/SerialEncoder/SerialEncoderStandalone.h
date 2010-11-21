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

#ifndef _GADGET_USDIGITAL_SERIAL_ENCODER_STANDALONE_H
#define _GADGET_USDIGITAL_SERIAL_ENCODER_STANDALONE_H

#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Guard.h>

#include "SEIBus.h"
#include <gmtl/gmtl.h>
#include <gmtl/Generate.h>

class SerialEncoderStandalone
{
public:
   SerialEncoderStandalone();

   void setPort(const std::string& port)
   {
      mPortStr = port;
   }
   void setBaudRate(const int& baud_rate)
   {
      mBaudRate = baud_rate;
   }
   const std::string getPort()
   {
      return mPortStr;
   }
   int getBaudRate()
   {
      return mBaudRate;
   }

   bool initializeDevice();
   gmtl::Matrix44f getSample();

private:
   bool  mActive;
   std::string mPortStr;
   long mBaudRate;
   SEIBus mBus;
};

#endif

