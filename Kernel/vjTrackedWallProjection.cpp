/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */

#include <vjConfig.h>

#include <Kernel/vjTrackedWallProjection.h>


void vjTrackedWallProjection::updateWallParams()
{
   // Compute the correct rotation matrix
   // mWallRotationMatrix_bak ==> surfMbase
   // tracker_mat ==> baseMtracker
   // We want surfMbase <=== surfMbase*trackerMbase
   // NOTE: wallRotMat's base is the base of the tracking system
   
   vjMatrix tracker_mat = *(mTracker->getData());     // baseMtracker
   
   // Method 1:
   // baseMsurf = baseMtracker*baseMsurf
   // surfMbase = inv(baseMsurf)
   // Cost: 2 inversions, 1 mult, 2 temp matrices
#if 0
   vjMatrix wall_rot_bak_inv;
   wall_rot_bak_inv.invert(mWallRotationMatrix_bak);
   vjMatrix base_m_surf;
   base_m_surf.mult(tracker_mat,wall_rot_bak_inv);
   mWallRotationMatrix.invert(base_m_surf);
#endif

#if 1
   // Method 2:
   // surfMbase = surfMbase*trackerMbase
   // Cost: 1 inversion, 1 mult, 1 temp matrix
   vjMatrix tracker_mat_inv;                 // trackerMbase
   tracker_mat_inv.invert(tracker_mat);
   mWallRotationMatrix.mult(mWallRotationMatrix_bak,tracker_mat_inv);
#endif  
}

ostream& vjTrackedWallProjection::outStream(ostream& out)
{
   out << "vjTrackedWallProjection:\n";
   out << "Pos Proxy: " << mTracker.getProxyName().c_str() << endl;
   return vjWallProjection::outStream(out);
}
