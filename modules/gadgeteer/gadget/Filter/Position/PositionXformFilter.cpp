/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>
#include <gadget/Filter/Position/PositionXformFilter.h>
#include <gadget/Filter/Position/PositionFilterFactory.h>

#include <jccl/Config/ConfigElement.h>

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

std::string PositionXformFilter::getElementType()
{
   return "position_transform_filter";
}

bool PositionXformFilter::config(jccl::ConfigElementPtr e)
{
   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_VERB_LVL, 
                        std::string("PositionXformFilter::config: ") +
                           e->getFullName() + std::string(":") +
                           e->getID() + std::string("\n"),
                        std::string("PositionXformFilter::config: done.\n") );

   vprASSERT(e->getID() == PositionXformFilter::getElementType());

   // These are the transforms from the base tracker coord system
   float pre_xt, pre_yt, pre_zt;
   float post_xt, post_yt, post_zt;
   pre_xt = e->getProperty<float>("pre_translate",0);
   pre_yt = e->getProperty<float>("pre_translate",1);
   pre_zt = e->getProperty<float>("pre_translate",2);

   post_xt = e->getProperty<float>("post_translate",0);
   post_yt = e->getProperty<float>("post_translate",1);
   post_zt = e->getProperty<float>("post_translate",2);


   // These values are specified in human-friendly degrees but must be passed
   // to GMTL as radians.
   float pre_xr, pre_yr, pre_zr;
   float post_xr, post_yr, post_zr;
   pre_xr = gmtl::Math::deg2Rad(e->getProperty<float>("pre_rotation",0));
   pre_yr = gmtl::Math::deg2Rad(e->getProperty<float>("pre_rotation",1));
   pre_zr = gmtl::Math::deg2Rad(e->getProperty<float>("pre_rotation",2));

   post_xr = gmtl::Math::deg2Rad(e->getProperty<float>("post_rotation",0));
   post_yr = gmtl::Math::deg2Rad(e->getProperty<float>("post_rotation",1));
   post_zr = gmtl::Math::deg2Rad(e->getProperty<float>("post_rotation",2));

   // Calculate the scale value
   // - If dev_units is 0.0f, then use custom_scale
   mScaleValue = e->getProperty<float>("device_units");

   if(mScaleValue == 0.0f)
   {
      mScaleValue = e->getProperty<float>("custom_scale");
   }

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
         
   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "m:preXform [T*R]: \n" << mPreXform
         << "\npre_trans_mat:\n" << pre_trans_mat
         << "\npre_rot_mat:\n" << pre_rot_mat 
         << "\n" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "m:postXform [T*R]: \n" << mPostXform
         << "\npost_trans_mat:\n" << post_trans_mat
         << "\npost_rot_mat:\n" << post_rot_mat 
         << "\n" << vprDEBUG_FLUSH;
                                
   return true;
}

// Update all the position samples by xforming them by the transform matrix
void PositionXformFilter::apply(std::vector< PositionData >& posSample)
{
   typedef std::vector<PositionData>::iterator iter_type;
   for (iter_type i = posSample.begin(); i != posSample.end(); ++i)
   {                               
      gmtl::Matrix44f& cur_mat((*i).editValue());

      gmtl::postMult(cur_mat, mPostXform); // POST xform: cur = cur*postTrans*postRot

      gmtl::Vec3f trans;                   // SCALE:
      gmtl::setTrans(trans, cur_mat);      // Get the translational vector
      trans *= mScaleValue;                // Scale the translation and set the value again
      gmtl::setTrans(cur_mat, trans);

      gmtl::preMult(cur_mat, mPreXform);   // PRE: S_world = wMs * S_sensor
   }
}

} // End of gadget namespace

