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
   vjMatrix tracker_mat = *(mTracker->getData());

   // Compute the correct rotation matrix
   mWallRotationMatrix.mult(tracker_mat,mWallRotationMatrix_bak);

   // Compute the correct rotation parameters
}

ostream& vjTrackedWallProjection::outStream(ostream& out)
{
   out << "vjTrackedWallProjection:\n";
   out << "Pos Proxy: " << mTracker.getProxyName().c_str() << endl;
   return vjWallProjection::outStream(out);
}
