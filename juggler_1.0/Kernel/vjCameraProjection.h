/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
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


#ifndef _VJ_CAMERA_PROJECTION_H_
#define _VJ_CAMERA_PROJECTION_H_

#include <vjConfig.h>
#include <Kernel/vjProjection.h>
class vjMatrix;

//: Projection class that simply takes a matrix for the camera position
//
class vjCameraProjection : public vjProjection
{
public:
   vjCameraProjection()
   {
      mType = vjProjection::SIM;
   }

   virtual void config(vjConfigChunk* chunk)
   {
      vjProjection::config(chunk);
   }

   virtual void calcViewMatrix(vjMatrix& cameraPos)
   {
      viewMat = cameraPos;
      frustum.set(-0.6f, 0.6f, -0.6f, 0.6f, mNearDist, mFarDist);
   }

   virtual ostream& outStream(ostream& out)
   {
      out << "vjCameraProjection:\n";
      return out;
   }
};

#endif
