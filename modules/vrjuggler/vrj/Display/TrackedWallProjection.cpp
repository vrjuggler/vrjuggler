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

#include <vrj/vrjConfig.h>

#include <vrj/Display/TrackedWallProjection.h>


namespace vrj
{

void TrackedWallProjection::updateWallParams()
{
   // Compute the correct rotation matrix
   // mWallRotationMatrix_bak ==> surfMbase
   // tracker_mat ==> baseMtracker
   // We want surfMbase <=== surfMbase*trackerMbase
   // NOTE: wallRotMat's base is the base of the tracking system

   Matrix tracker_mat = *(mTracker->getData());     // baseMtracker

   // Method 1:
   // baseMsurf = baseMtracker*baseMsurf
   // surfMbase = inv(baseMsurf)
   // Cost: 2 inversions, 1 mult, 2 temp matrices
#if 0
   Matrix wall_rot_bak_inv;
   wall_rot_bak_inv.invert(mWallRotationMatrix_bak);
   Matrix base_m_surf;
   base_m_surf.mult(tracker_mat,wall_rot_bak_inv);
   mWallRotationMatrix.invert(base_m_surf);
#endif

#if 1
   // Method 2:
   // surfMbase = surfMbase*trackerMbase
   // Cost: 1 inversion, 1 mult, 1 temp matrix
   Matrix tracker_mat_inv;                 // trackerMbase
   tracker_mat_inv.invert(tracker_mat);
   mWallRotationMatrix.mult(mWallRotationMatrix_bak,tracker_mat_inv);
#endif
}

std::ostream& TrackedWallProjection::outStream(std::ostream& out)
{
   out << "vjTrackedWallProjection:\n";
   out << "Pos Proxy: " << mTracker.getProxyName().c_str() << std::endl;
   return WallProjection::outStream(out);
}


};
