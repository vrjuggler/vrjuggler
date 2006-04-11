
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
   float pre_xt, pre_yt, pre_zt;
   float post_xt, post_yt, post_zt;
   pre_xt = c->getProperty<float>("pre_translate",0);
   pre_yt = c->getProperty<float>("pre_translate",1);
   pre_zt = c->getProperty<float>("pre_translate",2);

   post_xt = c->getProperty<float>("post_translate",0);
   post_yt = c->getProperty<float>("post_translate",1);
   post_zt = c->getProperty<float>("post_translate",2);


   // These values are specified in human-friendly degrees but must be passed
   // to GMTL as radians.
   float pre_xr, pre_yr, pre_zr;
   float post_xr, post_yr, post_zr;
   pre_xr = gmtl::Math::deg2Rad(c->getProperty<float>("pre_rotation",0));
   pre_yr = gmtl::Math::deg2Rad(c->getProperty<float>("pre_rotation",1));
   pre_zr = gmtl::Math::deg2Rad(c->getProperty<float>("pre_rotation",2));

   post_xr = gmtl::Math::deg2Rad(c->getProperty<float>("post_rotation",0));
   post_yr = gmtl::Math::deg2Rad(c->getProperty<float>("post_rotation",1));
   post_zr = gmtl::Math::deg2Rad(c->getProperty<float>("post_rotation",2));

   // Calculate the scale value
   // - If dev_units is 0.0f, then use custom_scale
   mScaleValue = c->getProperty<float>("dev_units");

   if(mScaleValue == 0.0f)
   { mScaleValue = c->getProperty<float>("custom_scale"); }

   // This makes a rotation matrix that moves a pt in
   // the device's coord system to the vj coord system.
   // ==> world_M_transmitter
   // PRE XFORM
   gmtl::Matrix44f pre_trans_mat, pre_rot_mat;
   if((0.0f != pre_xt) || (0.0f != pre_yt) || (0.0f != pre_zt))
   {
      gmtl::setTrans(pre_trans_mat, gmtl::Vec3f(pre_xt, pre_yt, pre_zt) );
   }
   if((pre_xr != 0.0f) || (pre_yr != 0.0f) || (pre_zr != 0.0f))
   {
      gmtl::EulerAngleXYZf euler( pre_xr,pre_yr,pre_zr );      
      pre_rot_mat = gmtl::makeRot<gmtl::Matrix44f>( euler );
   }

   // POST XFORM
   gmtl::Matrix44f post_trans_mat, post_rot_mat;
   if((0.0f != post_xt) || (0.0f != post_yt) || (0.0f != post_zt))
   {
      gmtl::setTrans(post_trans_mat, gmtl::Vec3f(post_xt, post_yt, post_zt) );
   }
   if((post_xr != 0.0f) || (post_yr != 0.0f) || (post_zr != 0.0f))
   {
      gmtl::EulerAngleXYZf euler( post_xr,post_yr,post_zr );      
      post_rot_mat = gmtl::makeRot<gmtl::Matrix44f>( euler );
   }
      
   mPreXform = pre_trans_mat;
   gmtl::postMult(mPreXform, pre_rot_mat);         // xformMat = T*R

   mPostXform = post_trans_mat;
   gmtl::postMult(mPostXform, post_rot_mat);         // xformMat = T*R
         
   vprDEBUG(vprDBG_ALL,0) << "m:preXform [T*R]: \n" << mPreXform
                          << "\npre_trans_mat:\n" << pre_trans_mat
                          << "\npre_rot_mat:\n" << pre_rot_mat 
                          << "\n" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,0) << "m:postXform [T*R]: \n" << mPostXform
                          << "\npost_trans_mat:\n" << post_trans_mat
                          << "\npost_rot_mat:\n" << post_rot_mat 
                          << "\n" << vprDEBUG_FLUSH;
                                
   return true;
}

// Update all the position samples by xforming them by the transform matrix
void PositionXformFilter::apply(std::vector< PositionData >& posSample)
{
   gmtl::Matrix44f* cur_mat(NULL);

   for(std::vector<PositionData>::iterator i=posSample.begin(); i != posSample.end(); ++i)
   {                               
      cur_mat = &((*i).mPosData);

      gmtl::postMult(*cur_mat, mPostXform);           // POST xform: cur = cur*postTrans*postRot

      gmtl::Vec3f trans;                              // SCALE:
      gmtl::setTrans(trans, *cur_mat);                // Get the translational vector
      trans *= mScaleValue;                           // Scale the translation and set the value again
      gmtl::setTrans(*cur_mat, trans);

      gmtl::preMult(*cur_mat, mPreXform);        // PRE: S_world = wMs * S_sensor       
   }
}

} // End of gadget namespace

