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

#ifndef _VRJ_LINEAR_SIGMOID_POS_FILTER_H_
#define _VRJ_LINEAR_SIGMOID_POS_FILTER_H_

#include <vrj/vrjConfig.h>

#include <gad/Type/PosFilter.h>
#include <vrj/Math/Quat.h>

namespace vrj
{

//: A pos filter class that uses a linear sigmoid
class LinearSigmoidPosFilter : public PosFilter
{
public:
   LinearSigmoidPosFilter()
   {
      mMinDist = 0.0f;
      mMaxDist = 0.05f;
      mMaxThreshold = 10000000.0f;        // Just set to some huge number
   }

   // Get the position to use
   // ARGS: newPos - The new postion this frame of the physical device
   Matrix getPos(const Matrix newPos);

   // Given a distance, return the scale factor
   // based upon the minDist and maxDist values
   float getScaleFactor(const float distance);

   void setMinDist(float val) { mMinDist = val;}
   float minDist() { return mMinDist;}

   void setMaxDist(float val) { mMaxDist = val;}
   float maxDist() { return mMaxDist;}

   void setMaxThreshold(float val) { mMaxThreshold = val; }
   float maxTheshold() { return mMaxThreshold; }

private:
   Matrix mLastReturnedPos;

   // These parameters influence the sigmoid
   float    mMinDist;            // Mininum distance parameter
   float    mMaxDist;            // Maximum distance parameter

   float    mMaxThreshold;       // Maximum distance that the tracker is allowed to move.
                                 // Anything farther is just ignored and is not interpolated to
};


//
//  Uses a linear sigmoid function with minimum and
// maximum threshholds.
//
//     maxThreshold    --- 0.0
//            |         |
//           max        |
//            /----------- 1.0
//           /          |
//          /           |
//         /            |
// -------/            --- 0.0
// 0     min
//
// Algorithm:
// ----------
// Determine the scale factor to use
// If the scale factor is 0.0 or 1.0, then just return the matrix directly
// Else, perform a quaternion slerp on the rotation and linear calculation
// on position and return that matrix.
Matrix LinearSigmoidPosFilter::getPos(const Matrix newPos)
{
   // If value is the same, then return immediately
   if(newPos == mLastReturnedPos)
   { return mLastReturnedPos; }

   const float eps(0.001);         // An epsilon value because of numerical precision as we approach zero difference
   float scale_factor(0.0f);
   float dist;
   Vec3 last_returned_trans;
   Vec3 new_trans;
   Vec3 trans_diff;

   // Get difference in translation
   last_returned_trans = mLastReturnedPos.getTrans();
   new_trans = newPos.getTrans();
   trans_diff = new_trans-last_returned_trans;
   dist = trans_diff.length();

   vprDEBUG(vprDBG_ALL,2) << "sigmoid: dist: " << dist << std::endl
                        << vprDEBUG_FLUSH;

   // Check max threshold
   if(dist > mMaxThreshold)
   {
      return mLastReturnedPos;      // If outside of threshold, just return old data
   }

   // Get scale factor
   scale_factor = getScaleFactor(dist);

   if(scale_factor <= eps)
   { return mLastReturnedPos; }
   else if(scale_factor >= (1.0f-eps))
   {
      mLastReturnedPos = newPos;
      return newPos;
   }
   else
   {
      vprDEBUG(vprDBG_ALL,2) << "sigmoid: scale_factor: " << scale_factor
                           << std::endl << vprDEBUG_FLUSH;

      vprASSERT((scale_factor > eps) && (scale_factor < (1.0f-eps)));

      Vec3 ret_trans;
      Matrix ret_val;

      ret_trans = last_returned_trans + (trans_diff * scale_factor);

      vprDEBUG(vprDBG_ALL,2) << "\tret_trans = last_returned_trans + (trans_diff * scale_factor) -->"
                           << ret_trans << " = " << last_returned_trans << " + (" << trans_diff
                           << " * " << scale_factor << " )\n" << vprDEBUG_FLUSH;

      // Compute scaled rotation
      Quat      source_rot, goal_rot, slerp_rot;
      source_rot.makeRot( mLastReturnedPos );          // Create source
      goal_rot.makeRot( newPos );                      // Create goal

      // ASSERT: We don't have two identical matrices
      slerp_rot.slerp( scale_factor,source_rot,goal_rot );    // Transform part way there
      ret_val.makeQuaternion( slerp_rot );            // Create the transform matrix to use
      ret_val.setTrans(ret_trans);

      mLastReturnedPos = ret_val;
      return mLastReturnedPos;
   }
}

float LinearSigmoidPosFilter::getScaleFactor(const float distance)
{
   if(distance < mMinDist)
   {
      return 0.0f;
   }
   else if(distance > mMaxDist)
   {
      return 1.0f;
   }
   else
   {
      vprASSERT(mMaxDist >= mMinDist);
      float range = mMaxDist - mMinDist;
      float scale_factor = (distance-mMinDist)/range;
      return scale_factor;
   }
}

};

#endif
