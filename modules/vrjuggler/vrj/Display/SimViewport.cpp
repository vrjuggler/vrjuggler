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

   /**  Configure the simulator */
void SimViewport::config(jccl::ConfigChunkPtr chunk)
{
   vprASSERT(chunk.get() != NULL);
   vprASSERT(chunk->getDescToken() == std::string("simViewport"));

   Viewport::config(chunk);

   mType = SIM;

   const float vert_fov = chunk->getProperty<float>("vert_fov");

   mLeftProj = new CameraProjection;
   ((CameraProjection*) mLeftProj)->setVerticalFOV(vert_fov);
   mLeftProj->config(chunk);
   mLeftProj->setEye(Projection::LEFT);
   mLeftProj->setViewport(this);

   mRightProj = new CameraProjection;
   ((CameraProjection*) mRightProj)->setVerticalFOV(vert_fov);
   mRightProj->config(chunk);
   mRightProj->setEye(Projection::RIGHT);
   mRightProj->setViewport(this);

   //bool has_simulator(false);
   //has_simulator = chunk->getProperty<bool>("hasSimPlugin");
   mSimulator = NULL;

   // Create the simulator stuff
   /*if(has_simulator)
   {
      jccl::ConfigChunkPtr sim_chunk =
         chunk->getProperty<jccl::ConfigChunkPtr>("simPlugIn");

      vprDEBUG(vrjDBG_DISP_MGR, vprDBG_CONFIG_LVL)
         << "SimViewport::config() creating simulator of type '"
         << sim_chunk->getDescToken() << "'\n" << vprDEBUG_FLUSH;
      mSimulator = DrawSimInterfaceFactory::instance()->createObject(sim_chunk->getDescToken());

      // XXX: Change this to an error once the new simulator loading code is
      // more robust.  -PH (4/13/2003)
      vprASSERT(NULL != mSimulator && "Failed to create draw simulator");
      mSimulator->initialize(this);
      mSimulator->config(sim_chunk);
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
