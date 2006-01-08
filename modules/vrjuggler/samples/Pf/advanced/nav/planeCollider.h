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

#ifndef _PLANE_COLLIDER_H_
#define _PLANE_COLLIDER_H_

#include <collider.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Matrix.h>

class planeCollider : public collider
{
public:
   planeCollider() : mHeight( 0 )
   {
   }

   bool testMove(gmtl::Vec3f curPos, gmtl::Vec3f delta, gmtl::Vec3f& correction, bool curPosWithDelta = false);

private:
   float mHeight;    // Height of the plane
};


inline bool planeCollider::testMove(gmtl::Vec3f curPos, gmtl::Vec3f delta, gmtl::Vec3f& correction, bool curPosWithDelta)
{
   correction.set(0,0,0);

   gmtl::Vec3f target_pos = curPos+delta;
   if(target_pos[1] < mHeight)
   {
      correction[1] = (mHeight-target_pos[1]);  // Get it back up there
      setDidCollide(true);
      return true;
   }
   else
   {
      setDidCollide(false);
      return false;
   }

}


#endif
