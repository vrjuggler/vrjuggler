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

#ifndef _GADGET_SIM_SETABLE_POSITION_H
#define _GADGET_SIM_SETABLE_POSITION_H

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/InputMixer.h>
#include <gadget/Type/PositionInterface.h>
#include <jccl/Config/ConfigChunkPtr.h>


namespace gadget
{

/**
 * Simulated positional device whose data is set by the user (in postFrame).
 *
 * Should be set just before input manager updates data so it
 * can be distributed by the remote input manager.
 * The end of postFrame() is the logical place for the user to
 * set the data.  This prevents computers connected to the remote
 * input manager from having different copies of data for very long.
 */
//class SimSetablePosition : public Input, public Position
class SimSetablePosition : public InputMixer<Input,Position>
{
public:
   SimSetablePosition() {;}
   virtual ~SimSetablePosition() {;}

   virtual bool config(jccl::ConfigChunkPtr chunk);

   /** These functions don't do anything. */
   int startSampling() { return 1; }
   int stopSampling() { return 1; }
   int sample() { return 1; }

   /** Updates the data. */
   virtual void updateData(){;}

   void setData(const std::vector<PositionData> &pos_data_vec);
   void setData(const PositionData &pos_data);

   static std::string getChunkType() { return std::string( "SimSetablePosition" ); }

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   virtual void destroy()
   {
      delete this;
   }

private:
   PositionData            mPos;                   /**<  The current position */

};

} // End of gadget namespace

#endif
