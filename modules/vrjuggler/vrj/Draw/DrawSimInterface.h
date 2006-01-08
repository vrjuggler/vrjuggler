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

#ifndef _VRJ_DRAW_SIM_INTERFACE_H_
#define _VRJ_DRAW_SIM_INTERFACE_H_

#include <vrj/vrjConfig.h>

#include <gadget/Type/KeyboardMouseProxy.h>
#include <gadget/Type/DeviceInterface.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <vrj/Display/Projection.h>

#include <boost/concept_check.hpp>

namespace vrj
{
   class SimViewport;

   /** \class DrawSimInterface DrawSimInterface.h vrj/Draw/DrawSimInterface.h
    *
    * Base class for all simulator interface across all Draw Manager types.
    */
   class DrawSimInterface
   {
   public:
      virtual ~DrawSimInterface()
      {
         /* Do nothing. */ ;
      }

      /**
       * Configures the simulator interface.
       *
       * @pre element is a valid configuration element.
       * @post It should be configured.
       */
      virtual bool config(jccl::ConfigElementPtr element) = 0;

      /** Called as part of the viewports updateProjection call. */
      virtual void updateProjectionData(const float positionScale,
                                        Projection* leftProj,
                                        Projection* rightProj)
      {
         boost::ignore_unused_variable_warning(positionScale);
         boost::ignore_unused_variable_warning(leftProj);
         boost::ignore_unused_variable_warning(rightProj);
      }

      virtual void initialize(SimViewport* simVp) = 0;
   };
}

#endif
