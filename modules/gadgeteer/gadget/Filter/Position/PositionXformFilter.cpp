
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
#include <gadget/Filter/Position/PositionXformFilter.h>
#include <gadget/Filter/Position/PositionFilterFactory.h>

#include <jccl/Config/ConfigChunk.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>
#include <gmtl/Output.h>

namespace gadget
{

/** Register this filter */
GADGET_REGISTER_POSFILTER_CREATOR( PositionXformFilter );

bool PositionXformFilter::config(jccl::ConfigChunkPtr c)
{
   vprDEBUG_OutputGuard(vprDBG_ALL, 0, 
                        std::string("PositionXformFilter::config: ") + c->getFullName() + std::string(":") + c->getDescToken(),
                        std::string("PositionXformFilter::config: done.\n") );

   std::string chunk_type = c->getDescToken();
   vprASSERT(chunk_type == PositionXformFilter::getChunkType());

   // These are the transforms from the base tracker coord system
   float xt, yt, zt;
   xt = c->getProperty<float>("translate",0);
   yt = c->getProperty<float>("translate",1);
   zt = c->getProperty<float>("translate",2);

   // These values are specified in human-friendly degrees but must be passed
   // to GMTL as radians.
   float xr, yr, zr;
   xr = gmtl::Math::deg2Rad(c->getProperty<float>("rotation",0));
   yr = gmtl::Math::deg2Rad(c->getProperty<float>("rotation",1));
   zr = gmtl::Math::deg2Rad(c->getProperty<float>("rotation",2));

   // Calculate the scale value
   // - If dev_units is 0.0f, then use custom_scale
   mScaleValue = c->getProperty<float>("dev_units");

   if(mScaleValue == 0.0f)
   { mScaleValue = c->getProperty<float>("custom_scale"); }

   // This makes a rotation matrix that moves a pt in
   // the device's coord system to the vj coord system.
   // ==> world_M_transmitter
   gmtl::Matrix44f trans_mat, rot_mat;
   if((0.0f != xt) || (0.0f != yt) || (0.0f != zt))
   {
      gmtl::setTrans(trans_mat, gmtl::Vec3f(xt, yt, zt) );
   }
   if((xr != 0.0f) || (yr != 0.0f) || (zr != 0.0f))
   {
      gmtl::EulerAngleXYZf euler( xr,yr,zr );      
      rot_mat = gmtl::makeRot<gmtl::Matrix44f>( euler );
   }
      
   m_worldMsensor = trans_mat;
   gmtl::postMult(m_worldMsensor, rot_mat);         // xformMat = T*R
      
   vprDEBUG(vprDBG_ALL,0) << "m:worldMsensor [T*R*S]: \n" << m_worldMsensor
                          << "\ntrans_mat:\n" << trans_mat
                          << "\nrot_mat:\n" << rot_mat 
                          << "\nscale value: " << mScaleValue << "\n" << vprDEBUG_FLUSH;
      
   return true;
}

// Update all the position samples by xforming them by the transform matrix
void PositionXformFilter::apply(std::vector< PositionData >& posSample)
{
   gmtl::Matrix44f* cur_mat(NULL);

   for(std::vector<PositionData>::iterator i=posSample.begin(); i != posSample.end(); ++i)
   {                               
      cur_mat = (*i).getPosition();
      gmtl::Vec3f trans;
      gmtl::setTrans(trans, *cur_mat);                // Get the translational vector
      trans *= mScaleValue;                           // Scale the translation and set the value again
      gmtl::setTrans(*cur_mat, trans);

      gmtl::preMult(*cur_mat, m_worldMsensor);        // S_world = wMs * S_sensor      
   }
}

} // End of gadget namespace

