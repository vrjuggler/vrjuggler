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
#include <float.h>
#include <vpr/Util/Debug.h>
#include <jccl/Config/ConfigChunk.h>
#include <gadget/Type/Position.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>


namespace gadget
{

// Set up the transformation information
bool Position::config(jccl::ConfigChunkPtr c)
{
  //vprDEBUG(vprDBG_ALL,0) << "vjPosition::config(jccl::ConfigChunkPtr)" << vprDEBUG_FLUSH;
  if ((c->getNum("translate") == 3) && (c->getNum("rotate") == 3))
  {
    // These are the transforms from the base tracker coord system

    float xt, yt, zt;
    xt = c->getProperty<float>("translate",0);
    yt = c->getProperty<float>("translate",1);
    zt = c->getProperty<float>("translate",2);

    // These values are specified in human-friendly degrees but must be passed
    // to GMTL as radians.
    float xr, yr, zr;
    xr = gmtl::Math::deg2Rad(c->getProperty<float>("rotate",0));
    yr = gmtl::Math::deg2Rad(c->getProperty<float>("rotate",1));
    zr = gmtl::Math::deg2Rad(c->getProperty<float>("rotate",2));

      // This makes a rotation matrix that moves a pt in
      // the device's coord system to the vj coord system.
      // ==> world_M_transmitter
    gmtl::EulerAngleXYZf euler( xr,yr,zr );
    rotMat = gmtl::makeRot<gmtl::Matrix44f>( euler );

    gmtl::identity(xformMat);
    gmtl::setTrans(xformMat, gmtl::Vec3f(xt, yt, zt) );
    gmtl::postMult(xformMat, rotMat);         // xformMat = T*R
  }

  return true;
}


Position::Position()
{
  //vprDEBUG(vprDBG_ALL,0) << "vjPosition::Position()" << vprDEBUG_FLUSH;
  gmtl::identity(xformMat);
}

Position::~Position()
{
    ;
}

};
