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


#ifndef _VJ_TRACKED_WALL_PROJECTION_
#define _VJ_TRACKED_WALL_PROJECTION_

#include <vjConfig.h>
#include <Kernel/vjWallProjection.h>
#include <Kernel/vjDebug.h>
#include <Input/InputManager/vjPosInterface.h>

class vjMatrix;

//-----------------------------------------------------------------
//: Wall specific class for viewport definitions.
//
//  This class behaves the same as the vjWallProjection class
//  except that it is tracked.
//
//  This means that all parameters are relative to a tracked (moving)
//  frame of reference.
//
// @author Allen Bierbaum
//  Date: 10-5-97
//----------------------------------------------------------------
class vjTrackedWallProjection : public vjWallProjection
{
public:

   //: Construct a Tracked wall projections
   //!ARGS: tracker_name - Name of the tracker tracking the screen
   vjTrackedWallProjection(vjMatrix surfaceRot, float toScr,
                    float toRight, float toLeft,
                    float toTop, float toBottom, std::string tracker_name)
      : vjWallProjection(surfaceRot, toScr, toRight, toLeft, toTop, toBottom)
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
   
   virtual ~vjTrackedWallProjection() {}
   
   virtual void config(vjConfigChunk* chunk)
   {
      vjProjection::config(chunk);        // Call base class config first
   }

   //: Recalculate the projection matrix
   //!POST: frustum has been recomputed for given eyePos
   virtual void calcViewMatrix(vjMatrix& eyePos)
   {
      updateWallParams();
      
      calcViewFrustum(eyePos);
      
      // We don't need to postTrans like in non-tracked projection
      // because the wall position is already in mWallRotationMatrix
      mViewMat = mWallRotationMatrix;   
   }

   void updateWallParams();

   ostream& outStream(ostream& out);

private:
   // ---- Original parameters ------/
   vjMatrix   mWallRotationMatrix_bak;       // Rotation of the screen
   // Screen configuration
   float mOriginToScreen_bak, mOriginToRight_bak, mOriginToLeft_bak, mOriginToTop_bak, mOriginToBottom_bak;

   vjPosInterface    mTracker;
};


#endif
