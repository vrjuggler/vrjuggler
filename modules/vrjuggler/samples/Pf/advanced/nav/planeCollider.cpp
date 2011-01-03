/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#include <gmtl/VecOps.h>
#include "planeCollider.h"


bool planeCollider::testMove(const gmtl::Vec3f& curPos,
                             const gmtl::Vec3f& delta,
                             gmtl::Vec3f& correction,
                             bool curPosWithDelta)
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

