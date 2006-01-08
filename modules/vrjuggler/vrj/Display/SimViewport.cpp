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

#include <vrj/vrjConfig.h>

#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>

#include <vrj/Util/Debug.h>
#include <vrj/Display/CameraProjection.h>
#include <vrj/Display/Projection.h>
#include <gadget/Type/Position/PositionUnitConversion.h>

#include <vrj/Display/SimViewport.h>


namespace vrj
{

SimViewport::~SimViewport()
{
   if ( NULL != mLeftProj )
   {
      delete mLeftProj;
      mLeftProj = NULL;
   }

   if ( NULL != mRightProj )
   {
      delete mRightProj;
      mRightProj = NULL;
   }
}

   /**  Configure the simulator */
void SimViewport::config(jccl::ConfigElementPtr element)
{
   vprASSERT(element.get() != NULL);
   vprASSERT(element->getID() == "simulator_viewport");

   Viewport::config(element);

   mType = SIM;

   const float vert_fov = element->getProperty<float>("vertical_fov");

   if ( NULL != mLeftProj )
   {
      delete mLeftProj;
   }

   mLeftProj = new CameraProjection;
   ((CameraProjection*) mLeftProj)->setVerticalFOV(vert_fov);
   mLeftProj->config(element);
   mLeftProj->setEye(Projection::LEFT);
   mLeftProj->setViewport(this);

   if ( NULL != mRightProj )
   {
      delete mRightProj;
   }

   mRightProj = new CameraProjection;
   ((CameraProjection*) mRightProj)->setVerticalFOV(vert_fov);
   mRightProj->config(element);
   mRightProj->setEye(Projection::RIGHT);
   mRightProj->setViewport(this);

   //bool has_simulator(false);
   //has_simulator = element->getProperty<bool>("has_simulator_plugin");
   mSimulator = NULL;

   // Create the simulator stuff
   /*if(has_simulator)
   {
      jccl::ConfigElementPtr sim_element =
         element->getProperty<jccl::ConfigElementPtr>("simulator_plugin");

      vprDEBUG(vrjDBG_DISP_MGR, vprDBG_CONFIG_LVL)
         << "SimViewport::config() creating simulator of type '"
         << sim_element->getID() << "'\n" << vprDEBUG_FLUSH;
      mSimulator = DrawSimInterfaceFactory::instance()->createObject(sim_element->getID());

      // XXX: Change this to an error once the new simulator loading code is
      // more robust.  -PH (4/13/2003)
      vprASSERT(NULL != mSimulator && "Failed to create draw simulator");
      mSimulator->initialize(this);
      mSimulator->config(sim_element);
   }
   */
}

void SimViewport::updateProjections(const float positionScale)
{
   if(mSimulator != NULL)
   {
      mSimulator->updateProjectionData(positionScale, mLeftProj, mRightProj);
   }
}

}
