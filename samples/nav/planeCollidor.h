/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#include <collidor.h>
#include <Math/vjVec3.h>
#include <Math/vjMatrix.h>

#ifndef _PLANE_COLLIDOR_H_
#define _PLANE_COLLIDOR_H_

class planeCollidor : public collidor
{
public:
   planeCollidor()
   {
      mHeight = 0;
   }

   bool testMove(vjVec3 curPos, vjVec3 delta, vjVec3& correction, bool curPosWithDelta = false);

private:
   float mHeight;    // Height of the plane
};


bool planeCollidor::testMove(vjVec3 curPos, vjVec3 delta, vjVec3& correction, bool curPosWithDelta)
{
   correction.set(0,0,0);

   vjVec3 target_pos = curPos+delta;
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
