/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#ifndef _GADGET_SIM_RELATIVE_POSITION_H
#define _GADGET_SIM_RELATIVE_POSITION_H

#include <gadget/gadgetConfig.h>
#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/PositionInterface.h>
#include <jccl/Config/ConfigElementPtr.h>


namespace gadget
{

/** \class SimRelativePosition SimRelativePosition.h gadget/Devices/Sim/SimRelativePosition.h
 *
 * Simulates a position device by making an existing device behave relative to
 * another one.  One device is the base coordinate system frame of reference.
 * The other device has its position information transformed to be relative to
 * that frame (there is a matrix multiply).
 *
 * @note This class should not be used directly by the user.
 */
class SimRelativePosition
   : public input_position_t
{
public:
   SimRelativePosition() {;}
   virtual ~SimRelativePosition()
   {
      /* Do nothing. */ ;
   }

   virtual bool config(jccl::ConfigElementPtr element);

   /** These functions don't do anything. */
   bool startSampling() { return true; }
   bool stopSampling() { return true; }
   bool sample() { return true; }

   /** Updates the data. */
   virtual void updateData();

   static std::string getElementType();

private:
   PositionData      mPos;         /**< The current position being simulated */
   PositionInterface mBaseFrame;   /**< The base frame of reference */
   PositionInterface mRelativePos; /**< the relative position */
};

} // End of gadget namespace

#endif
