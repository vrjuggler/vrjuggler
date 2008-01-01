/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _VRJ_DRAW_SIM_INTERFACE_H_
#define _VRJ_DRAW_SIM_INTERFACE_H_

#include <vrj/vrjConfig.h>

#include <jccl/Config/ConfigElementPtr.h>

#include <vrj/Display/SimViewportPtr.h>
#include <vrj/Display/ProjectionPtr.h>
#include <vrj/Draw/DrawSimInterface.h>


namespace vrj
{

   /** \class DrawSimInterface DrawSimInterface.h vrj/Draw/DrawSimInterface.h
    *
    * Base class for all simulator interface across all Draw Manager types.
    */
   class VJ_CLASS_API DrawSimInterface
   {
   public:
      virtual ~DrawSimInterface();

      /**
       * Configures the simulator interface.
       *
       * @pre element is a valid configuration element.
       * @post It should be configured.
       */
      virtual bool config(jccl::ConfigElementPtr element) = 0;

      /** Called as part of the viewports updateProjection call. */
      virtual void updateProjectionData(const float positionScale,
                                        ProjectionPtr leftProj,
                                        ProjectionPtr rightProj);

      virtual void initialize(SimViewportPtr simVp) = 0;
   };
}


#endif
