/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#include <gadget/gadgetConfig.h>
#include <jccl/Config/ConfigChunk.h>

#include <gadget/Type/PositionFilter.h>
#include <gadget/Type/LinearSigmoidPositionFilter.h>
#include <gadget/Util/Debug.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>

#include <gadget/Type/PositionProxy.h>


namespace gadget
{

/**
 * Sets the transform for this PositionProxy.
 * Sets the transformation matrix to:
 * <code>
 *    mMatrixTransform = M<sub>trans</sub>.post(M<sub>rot</sub>)
 * </code>
 *
 * @note This means that to set transform, you specific the translation
 *       followed by rotation that takes the device from where it physically
 *       is in space to where you want it to be.
 */
void PositionProxy::setTransform(float xoff, float yoff, float zoff,    // Translate
                                 float xrot, float yrot, float zrot)   // Rotate
{
   mETrans = true;
   gmtl::identity(mMatrixTransform);

   gmtl::Matrix44f trans_mat;
   gmtl::Matrix44f rot_mat;

   if((xoff != 0.0f) || (yoff != 0.0f) || (zoff != 0.0f))
   {
      //trans_mat .makeTrans(xoff, yoff, zoff);
      gmtl::setTrans(trans_mat, gmtl::Vec3f(xoff, yoff, zoff));
   }
   if((xrot != 0.0f) || (yrot != 0.0f) || (zrot != 0.0f))
   {
      //rot_mat.makeXYZEuler(xrot, yrot, zrot);
      gmtl::EulerAngleXYZf euler( gmtl::Math::deg2Rad(xrot),
                                  gmtl::Math::deg2Rad(yrot),
                                  gmtl::Math::deg2Rad(zrot) );
      gmtl::setRot( rot_mat, euler );
   }

   mMatrixTransform = (trans_mat * rot_mat);
}



bool PositionProxy::config(jccl::ConfigChunkPtr chunk)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
      std::string("------------------ Position PROXY config() -----------------\n"),
      std::string("\n"));
   vprASSERT(chunk->getDescToken() == "PosProxy");

   // if we are going to be receiving remote data, we need to connect to a vjNetInput
   std::string location = chunk->getProperty<std::string>("location");
   if(location.size() > 0)
   {
      mDeviceName = chunk->getFullName();
      mDeviceName += "_NET_";   // input device we'll point to
      mUnitNum = 0;
      setTransform(0,0,0, 0,0,0 );
   }
   else
   {
      mUnitNum = chunk->getProperty<int>("unit");
      mDeviceName = chunk->getProperty<std::string>("device");

      if (true == chunk->getProperty<bool>("etrans") )
      {
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR,3)
            << "Position Transform enabled..." << std::endl << vprDEBUG_FLUSH;
         setTransform
            ( chunk->getProperty<float>("translate",0) , // xtrans
            chunk->getProperty<float>("translate",1) , // ytrans
            chunk->getProperty<float>("translate",2) , // ztrans
            chunk->getProperty<float>("rotate",0) , // xrot
            chunk->getProperty<float>("rotate",1) , // yrot
            chunk->getProperty<float>("rotate",2) );// zrot
/*
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR,4)
            << "Transform Matrix: " << std::endl
            << getTransform() << std::endl << vprDEBUG_FLUSH;
*/
      }

      // Setup filter method
      // XXX: For now, just hardcode to a single filter type
      // in future, there should be a filter factory
      if(true == chunk->getProperty<bool>("useFilter"))
      {
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR,3)
            << "Using filter: Linear sigmoid." << std::endl << vprDEBUG_FLUSH;
         LinearSigmoidPositionFilter* sig_filter;
         sig_filter = new LinearSigmoidPositionFilter();

         jccl::ConfigChunkPtr sigmoid_params =
            chunk->getProperty<jccl::ConfigChunkPtr>("sigmoidParams");

         if(sigmoid_params.get() == NULL)
         {
            vprDEBUG(vprDBG_ERROR,0)
               << clrOutBOLD(clrRED,"ERROR:") << "Got NULL sigmoid Params.\n"
               << vprDEBUG_FLUSH;
         }

         sig_filter->setMaxDist(sigmoid_params->getProperty<float>("maxDist"));
         sig_filter->setMinDist(sigmoid_params->getProperty<float>("minDist"));
         sig_filter->setMaxThreshold(sigmoid_params->getProperty<float>("maxThreshold"));
         mFilter = sig_filter;
      }
      else
      {
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR,3) << "NOT USING FILTER."
                                              << std::endl << vprDEBUG_FLUSH;
      }
   }

   // --- SETUP PROXY with INPUT MGR ---- //
   refresh();

   return true;
}

};
