
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
#include <gadget/Filter/Position/PositionXformFilter.h>
#include <gadget/Filter/Position/PositionFilterFactory.h>

/** Register this filter */
GADGET_REGISTER_POSFILTER_CREATOR( PositionXformFilter );

namespace gadget
{

bool PositionXformFilter::config(jccl::ConfigChunkPtr c)
{
   std::string chunk_type = first_chunk->getDescToken();
   vprASSERT(chunk_type == PositionXformFilter::getChunkType());

   //vprDEBUG(vprDBG_ALL,0) << "vjPosition::config(jccl::ConfigChunkPtr)" << vprDEBUG_FLUSH;
   if ( (c->getNum("translate") == 3) && (c->getNum("rotation") == 3) )
   {
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

      // This makes a rotation matrix that moves a pt in
      // the device's coord system to the vj coord system.
      // ==> world_M_transmitter
      gmtl::EulerAngleXYZf euler( xr,yr,zr );
      gmtl::Matrix44f rotMat = gmtl::makeRot<gmtl::Matrix44f>( euler );

      gmtl::identity(m_worldMsensor);
      gmtl::setTrans(m_worldMsensor, gmtl::Vec3f(xt, yt, zt) );
      gmtl::postMult(m_worldMsensor, rotMat);         // xformMat = T*R
   }
}

// Update all the position samples by xforming them by the transform matrix
void PositionXformFilter::apply(std::vector< PositionData >& posSample)
{
   gmtl::Matrix44f* cur_mat(NULL);

   for(std::vector<PositionData>::iterator i=posSample.begin(); i != posSample.end(); ++i)
   {
      cur_mat = (*i).getPosition();
      gmtl::preMult(*cur_mat, m_worldMsensor);        // S_world = wMs * S_sensor
   }
}

} // End of gadget namespace

