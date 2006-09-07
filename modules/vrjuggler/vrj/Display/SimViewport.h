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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_SIM_DISPLAY_H_
#define _VRJ_SIM_DISPLAY_H_
//#pragma once

#include <vrj/vrjConfig.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Viewport.h>

#include <jccl/Config/ConfigElementPtr.h>
#include <vrj/Draw/DrawSimInterface.h>


namespace vrj
{

/** \class SimViewport SimViewport.h vrj/Display/SimViewport.h
 *
 * Simulator viewport.
 */
class VJ_CLASS_API SimViewport : public Viewport
{
public:
   SimViewport()
      : Viewport()
   {
      ;
   }

   SimViewport(const SimViewport& sv)
      : Viewport(sv)
      , mSimulator(sv.mSimulator)
   {
      ;
   }

   virtual ~SimViewport();

public:
   /** Configures the simulator. */
   virtual bool config(jccl::ConfigElementPtr element);

   /**
    * Updates the projections.
    *
    * @param positionScale Scale value for converting from Juggler units
    *                      (meters) to the display units.
    */
   virtual void updateProjections(const float positionScale);

   DrawSimInterface* getDrawSimInterface()
   {
      return mSimulator;
   }

   void setDrawSimInterface(DrawSimInterface* draw_sim_i)
   {
      mSimulator = draw_sim_i;
   }

protected:
   DrawSimInterface*    mSimulator;    /**< The simulator that we are using here */
};

}

#endif
