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

#include <boost/concept_check.hpp>
#include <gadget/Type/Position/PositionUnitConversion.h>

#include "SerialEncoderStandalone.h"

SerialEncoderStandalone::SerialEncoderStandalone()   
   :mPortStr("/dev/ttyS0")
   ,mBaudRate(38400)
{
   /* Do nothing. */
}

bool SerialEncoderStandalone::initializeDevice()
{
   return mBus.initializeSEI(mPortStr.c_str());
}

gmtl::Matrix44f SerialEncoderStandalone::getSample()
{
   unsigned int tiltDevice=1;
   unsigned int heightDevice=2;

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

   return translation*rotation;
}
