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

//#include <gadget/Type/PositionFilter.h>
//#include <gadget/Type/LinearSigmoidPositionFilter.h>
#include <gadget/Util/Debug.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>

#include <gadget/Type/PositionProxy.h>
#include <gadget/Type/Position/PositionUnitConversion.h>

namespace gadget
{

float PositionProxy::sScaleFactor = gadget::PositionUnitConversion::ConvertToFeet;

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
      //setTransform(0,0,0, 0,0,0 );
   }
   else
   {
      mUnitNum = chunk->getProperty<int>("unit");
      mDeviceName = chunk->getProperty<std::string>("device");

      if (true == chunk->getProperty<bool>("etrans") )
      {
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR,3)
            << "Position Transform enabled..." << std::endl << vprDEBUG_FLUSH;
         /*
         setTransform
            ( chunk->getProperty<float>("translate",0) , // xtrans
            chunk->getProperty<float>("translate",1) , // ytrans
            chunk->getProperty<float>("translate",2) , // ztrans
            chunk->getProperty<float>("rotate",0) , // xrot
            chunk->getProperty<float>("rotate",1) , // yrot
            chunk->getProperty<float>("rotate",2) );// zrot
         */
/*
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR,4)
            << "Transform Matrix: " << std::endl
            << getTransform() << std::endl << vprDEBUG_FLUSH;
*/
      }

      // Setup filter method
      // XXX: For now, just hardcode to a single filter type
      // in future, there should be a filter factory
      /*
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
      */
   }

   // --- SETUP PROXY with INPUT MGR ---- //
   refresh();

   return true;
}

void PositionProxy::updateData()
{
   if((!mStupified) && (mTypedDevice != NULL))
   {
       mPositionData = (mTypedDevice->getPositionData (mUnitNum));

       //vprDEBUG(vprDBG_ALL,0) << "Proxy::updateData: mPosData:" << *(mPositionData.getPosition()) << "\n" << vprDEBUG_FLUSH;

      /*
      if(mETrans)
         transformData();
         */

      // Filter the data if there is an active filters
      /*
      if(mFilter != NULL)
      {
         *(mPositionData.getPosition()) = mFilter->getPos(*(mPositionData.getPosition()));
      }
      */

      // Do scaling based on Proxy scale factor
      gmtl::Vec3f trans;                              // SCALE:
      gmtl::setTrans(trans, *(mPositionData.getPosition()));           // Get the translational vector
      trans *= sScaleFactor;                          // Scale the translation and set the value again
      gmtl::setTrans(*(mPositionData.getPosition()), trans);
   }
}

} // End of gadget namespace
