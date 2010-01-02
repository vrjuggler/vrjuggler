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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_SIM_SETABLE_POSITION_H
#define _GADGET_SIM_SETABLE_POSITION_H

#include <gadget/gadgetConfig.h>
#include <vector>
#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/PositionInterface.h>
#include <jccl/Config/ConfigElementPtr.h>


namespace gadget
{

/** \class SimSetablePosition SimSetablePosition.h gadget/Devices/Sim/SimSetablePosition.h
 *
 * Simulated positional device whose data is set by the user (in postFrame).
 *
 * Should be set just before input manager updates data so it
 * can be distributed by the Remote Input Manager.
 * The end of postFrame() is the logical place for the user to
 * set the data.  This prevents computers connected to the remote
 * input manager from having different copies of data for very long.
 */
//class SimSetablePosition : public Input, public Position
class GADGET_CLASS_API SimSetablePosition
   : public input_position_t
{
public:
   SimSetablePosition()
   {;}

   virtual ~SimSetablePosition()
   {;}

   virtual bool config(jccl::ConfigElementPtr element);

   /** These functions don't do anything. */
   bool startSampling()
   {
      return true;
   }

   bool stopSampling()
   {
      return true;
   }

   bool sample()
   {
      return true;
   }

   /** Updates the data. */
   virtual void updateData()
   {;}

   void setData(const std::vector<PositionData>& posDataVec);
   void setData(const PositionData& posData);

   static std::string getElementType();

private:
   PositionData            mPos;                   /**<  The current position */
};

} // End of gadget namespace


#endif
