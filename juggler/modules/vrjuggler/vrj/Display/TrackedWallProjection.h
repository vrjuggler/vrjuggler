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

#ifndef _VRJ_TRACKED_WALL_PROJECTION_H_
#define _VRJ_TRACKED_WALL_PROJECTION_H_

#include <vrj/vrjConfig.h>
#include <vrj/Display/WallProjection.h>
#include <vrj/Util/Debug.h>
#include <gadget/Type/PositionInterface.h>

namespace vrj
{

class Matrix;

//-----------------------------------------------------------------
//: Wall specific class for viewport definitions.
//
//  This class behaves the same as the WallProjection class
//  except that it is tracked.
//
//  This means that all parameters are relative to a tracked (moving)
//  frame of reference.
//
// @author Allen Bierbaum
//  Date: 10-5-97
//----------------------------------------------------------------
class TrackedWallProjection : public WallProjection
{
public:

   //: Construct a Tracked wall projections
   //!ARGS: tracker_name - Name of the tracker tracking the screen
   TrackedWallProjection(Matrix surfaceRot, float toScr,
                    float toRight, float toLeft,
                    float toTop, float toBottom, std::string tracker_name)
      : WallProjection(surfaceRot, toScr, toRight, toLeft, toTop, toBottom)
   {
      // --- Backup some parameters --- //
      mOriginToScreen_bak = toScr;
      mOriginToRight_bak = toRight;
      mOriginToLeft_bak = toLeft;
      mOriginToTop_bak = toTop;
      mOriginToBottom_bak = toBottom;
      mWallRotationMatrix_bak = surfaceRot;

      //XXX: Watch for timing problems here if trakcer is not inited first.
      //     It shoulbe be though from dependency checking
      mTracker.init(tracker_name);              // Intialize the tracker
   }

   virtual ~TrackedWallProjection() {}

   virtual void config(jccl::ConfigChunk* chunk)
   {
      Projection::config(chunk);        // Call base class config first
   }

   //: Recalculate the projection matrix
   //!POST: frustum has been recomputed for given eyePos
   virtual void calcViewMatrix(Matrix& eyePos)
   {
      updateWallParams();

      calcViewFrustum(eyePos);

      // We don't need to postTrans like in non-tracked projection
      // because the wall position is already in mWallRotationMatrix
      mViewMat = mWallRotationMatrix;
   }

   void updateWallParams();

   std::ostream& outStream(std::ostream& out);

private:
   // ---- Original parameters ------/
   Matrix   mWallRotationMatrix_bak;       // Rotation of the screen
   // Screen configuration
   float mOriginToScreen_bak, mOriginToRight_bak, mOriginToLeft_bak, mOriginToTop_bak, mOriginToBottom_bak;

   gadget::PositionInterface    mTracker;
};

};
#endif
