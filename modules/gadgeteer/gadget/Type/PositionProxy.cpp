/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
#include <gadget/Type/PositionProxy.h>
#include <jccl/Config/ConfigChunk.h>

#include <gadget/Type/PositionFilter.h>
#include <gadget/Type/LinearSigmoidPositionFilter.h>

namespace gadget
{

//: Set the transform for this PositionProxy
// Sets the transformation matrix to
//    mMatrixTransform = M<sub>trans</sub>.post(M<sub>rot</sub>)
//! NOTE: This means that to set transform, you specific the translation
//+       followed by rotation that takes the device from where it physically
//+       is in space to where you want it to be.
void PositionProxy::setTransform(float xoff, float yoff, float zoff,    // Translate
                                 float xrot, float yrot, float zrot)   // Rotate
{
   mETrans = true;
   mMatrixTransform.makeIdent();
   vrj::Matrix trans_mat; trans_mat.makeIdent();
   vrj::Matrix rot_mat; rot_mat.makeIdent();

   if((xoff != 0.0f) || (yoff != 0.0f) || (zoff != 0.0f))
      trans_mat.makeTrans(xoff, yoff, zoff);
   if((xrot != 0.0f) || (yrot != 0.0f) || (zrot != 0.0f))
      rot_mat.makeXYZEuler(xrot, yrot, zrot);

   mMatrixTransform.mult(trans_mat, rot_mat);
}



bool PositionProxy::config(jccl::ConfigChunkPtr chunk)
{
   vprDEBUG_BEGIN(vrjDBG_INPUT_MGR,3) << "------------------ POS PROXY config() -----------------\n" << vprDEBUG_FLUSH;
   vprASSERT(((std::string)chunk->getType()) == "PositionProxy");

   mUnitNum = chunk->getProperty("unit");
   mDeviceName = (std::string)chunk->getProperty("device");

   if (true == (bool)chunk->getProperty("etrans") )
   {
      vprDEBUG_NEXT(vrjDBG_INPUT_MGR,3) << "Position Transform enabled..."
                                      << std::endl << vprDEBUG_FLUSH;
      setTransform
      ( chunk->getProperty("translate",0) , // xtrans
        chunk->getProperty("translate",1) , // ytrans
        chunk->getProperty("translate",2) , // ztrans
        chunk->getProperty("rotate",0) , // xrot
        chunk->getProperty("rotate",1) , // yrot
        chunk->getProperty("rotate",2) );// zrot
      vprDEBUG_NEXT(vrjDBG_INPUT_MGR,4) << "Transform Matrix: " << std::endl
                                      << getTransform() << std::endl
                                      << vprDEBUG_FLUSH;
   }

   // Setup filter method
   // XXX: For now, just hardcode to a single filter type
   // in future, there should be a filter factory
   if(true == (bool)chunk->getProperty("useFilter"))
   {
      vprDEBUG_NEXT(vrjDBG_INPUT_MGR,3) << "Using filter: Linear sigmoid."
                                      << std::endl << vprDEBUG_FLUSH;
      LinearSigmoidPositionFilter* sig_filter;
      sig_filter = new LinearSigmoidPositionFilter();

      jccl::ConfigChunkPtr sigmoid_params = (jccl::ConfigChunkPtr)chunk->getProperty("sigmoidParams");
      if(sigmoid_params.get() == NULL)
      { vprDEBUG(vprDBG_ERROR,0) << clrOutBOLD(clrRED,"ERROR:") << "Got NULL sigmoid Params.\n" << vprDEBUG_FLUSH; }

      sig_filter->setMaxDist(sigmoid_params->getProperty("maxDist"));
      sig_filter->setMinDist(sigmoid_params->getProperty("minDist"));
      sig_filter->setMaxThreshold(sigmoid_params->getProperty("maxThreshold"));
      mFilter = sig_filter;
   }
   else
   {
      vprDEBUG_NEXT(vrjDBG_INPUT_MGR,3) << "NOT USING FILTER." << std::endl
                                      << vprDEBUG_FLUSH;
   }

   // --- SETUP PROXY with INPUT MGR ---- //
   refresh();

   return true;
}

};
