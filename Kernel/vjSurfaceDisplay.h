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

#ifndef _VJ_SURFACE_DISPLAY_H_
#define _VJ_SURFACE_DISPLAY_H_

#include <vjConfig.h>

#include <Kernel/vjDebug.h>
#include <Kernel/vjDisplay.h>
//#include <Kernel/vjProjection.h>
#include <Kernel/vjWallProjection.h>
#include <Kernel/vjTrackedWallProjection.h>
#include <Math/vjVec3.h>
#include <Math/vjVec4.h>
#include <Math/vjCoord.h>

//: Defines a display surface an associated projections
//
//
class vjSurfaceDisplay : public vjDisplay
{
public:
   vjSurfaceDisplay() :  mTracked(false), mLeftProj(NULL), mRightProj(NULL)
   {;}

   virtual ~vjSurfaceDisplay() {}

public:
   //: Takes a display chunk and configures the display based one it.
   //! PRE: chunk is a valid chunk
   //! POST: display is configured
   //+       If there is an error is the specified config, we output error
   //+       and "fix" the error.
   //! NOTE: All derived display classes MUST call this function
   //+       after doing local configuration.
   virtual void config(vjConfigChunk *chunk)
   {
      vjASSERT(chunk != NULL);
      vjDisplay::config(chunk);     // Call base class config

      mType = SURFACE;

      // Read in the corners
      vjConfigChunk* ll_corner_chunk = chunk->getProperty("corners",0);
      vjConfigChunk* lr_corner_chunk = chunk->getProperty("corners",1);
      vjConfigChunk* ur_corner_chunk = chunk->getProperty("corners",2);
      vjConfigChunk* ul_corner_chunk = chunk->getProperty("corners",3);
      mLLCorner.set(ll_corner_chunk->getProperty("x"),
                    ll_corner_chunk->getProperty("y"),
                    ll_corner_chunk->getProperty("z"));
      mLRCorner.set(lr_corner_chunk->getProperty("x"),
                    lr_corner_chunk->getProperty("y"),
                    lr_corner_chunk->getProperty("z"));
      mURCorner.set(ur_corner_chunk->getProperty("x"),
                    ur_corner_chunk->getProperty("y"),
                    ur_corner_chunk->getProperty("z"));
      mULCorner.set(ul_corner_chunk->getProperty("x"),
                    ul_corner_chunk->getProperty("y"),
                    ul_corner_chunk->getProperty("z"));

      // Calculate the rotation and the pts
      calculateSurfaceRotation();
      calculateCornersInBaseFrame();

      // Get info about being tracked
      mTracked = chunk->getProperty("tracked");
      if(mTracked)
         mTrackerProxyName = (std::string)chunk->getProperty("trackerproxy");

      // Create Projection objects
      // NOTE: The -'s are because we are measuring distance to
      //  the left(bottom) which is opposite the normal axis direction
      //vjMatrix rot_inv;
      //rot_inv.invert(mSurfaceRotation);
      if(!mTracked)
      {
         mLeftProj = new vjWallProjection(mSurfaceRotation,-mxLLCorner[VJ_Z],
                                       mxLRCorner[VJ_X],-mxLLCorner[VJ_X],
                                       mxURCorner[VJ_Y],-mxLRCorner[VJ_Y]);
         mRightProj = new vjWallProjection(mSurfaceRotation,-mxLLCorner[VJ_Z],
                                       mxLRCorner[VJ_X],-mxLLCorner[VJ_X],
                                       mxURCorner[VJ_Y],-mxLRCorner[VJ_Y]);
      }
      else
      {
         mLeftProj = new vjTrackedWallProjection(mSurfaceRotation,-mxLLCorner[VJ_Z],
                                       mxLRCorner[VJ_X],-mxLLCorner[VJ_X],
                                       mxURCorner[VJ_Y],-mxLRCorner[VJ_Y],
                                                 mTrackerProxyName);
         mRightProj = new vjTrackedWallProjection(mSurfaceRotation,-mxLLCorner[VJ_Z],
                                       mxLRCorner[VJ_X],-mxLLCorner[VJ_X],
                                       mxURCorner[VJ_Y],-mxLRCorner[VJ_Y],
                                                  mTrackerProxyName);


      }
      // Configure the projections
      mLeftProj->config(chunk);
      mLeftProj->setEye(vjProjection::LEFT);
      mRightProj->config(chunk);
      mRightProj->setEye(vjProjection::RIGHT);
   }

   virtual void updateProjections()
   {
      vjMatrix left_eye_pos, right_eye_pos;     // NOTE: Eye coord system is -z forward, x-right, y-up

      // -- Calculate Eye Positions -- //
      vjMatrix cur_head_pos = *(mUser->getHeadPos());
      vjCoord  head_coord(cur_head_pos);       // Create a user readable version

      vjDEBUG(vjDBG_ALL,5)
         << "vjDisplay::updateProjections: Getting head position" << std::endl
         << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_ALL,5) << "\tHeadPos:" << head_coord.pos << "\tHeadOr:"
                           << head_coord.orient << std::endl << vjDEBUG_FLUSH;

      // Compute location of left and right eyes
      //float interocularDist = 2.75f/12.0f;
      float interocularDist = mUser->getInterocularDistance();
      float eye_offset = interocularDist/2.0f;      // Distance to move eye

      left_eye_pos.postTrans(cur_head_pos, -eye_offset, 0, 0);
      right_eye_pos.postTrans(cur_head_pos, eye_offset, 0, 0);

      mLeftProj->calcViewMatrix(left_eye_pos);
      mRightProj->calcViewMatrix(right_eye_pos);
   }

   void getCorners(vjVec3& ll, vjVec3& lr, vjVec3& ur, vjVec3& ul)
   {
      ll = mLLCorner; lr = mLRCorner; ur = mURCorner; ul = mULCorner;
   }

   vjProjection* getLeftProj()
   { return mLeftProj; }

   vjProjection* getRightProj()
   { return mRightProj; }

   virtual std::ostream& outStream(std::ostream& out)
   {
      vjDisplay::outStream(out);

      out << "LL: " << mLLCorner << ", LR: " << mLRCorner
          << ", UR: " << mURCorner << ", UL:" << mULCorner << std::endl;
      out << "surfRot: \n" << mSurfaceRotation << std::endl;
      out << "leftProj: " << *mLeftProj;
      out << "  rightProj: " << *mRightProj << std::endl;
      return out;
   }

protected:
   void calculateSurfaceRotation()
   {
      assertPtsLegal();

      // Find the base vectors for the surface axiis (in terms of the base coord system)
      // With z out, x to the right, and y up
      vjVec3 x_base, y_base, z_base;
      x_base = (mLRCorner-mLLCorner);
      y_base = (mURCorner-mLRCorner);
      z_base = x_base.cross(y_base);

      // They must be normalized
      x_base.normalize(); y_base.normalize(); z_base.normalize();

      // Calculate the surfaceRotMat using law of cosines
      mSurfaceRotation.makeDirCos(x_base,y_base,z_base);      // surfMbase
      //mSurfaceRotation.invert(mSurfRotInv);              // baseMsurf
   }

   void calculateCornersInBaseFrame()
   {
      mxLLCorner.xformFull(mSurfaceRotation,mLLCorner);
      mxLRCorner.xformFull(mSurfaceRotation,mLRCorner);
      mxURCorner.xformFull(mSurfaceRotation,mURCorner);
      mxULCorner.xformFull(mSurfaceRotation,mULCorner);

      // Verify that they are all in the same x,y plane
      vjDEBUG(vjDBG_ALL,5) << std::setprecision(10) << mxLLCorner[VJ_Z]  << " "
                           << mxLRCorner[VJ_Z] << " "
                           << mxURCorner[VJ_Z] << " "
                           << mxULCorner[VJ_Z] << "\n" << vjDEBUG_FLUSH;

#ifdef VJ_DEBUG
      const float epsilon = 1e-6;
#endif
      vjASSERT(vjIsEqual(mxLLCorner[VJ_Z], mxLRCorner[VJ_Z], epsilon) &&
               vjIsEqual(mxURCorner[VJ_Z], mxULCorner[VJ_Z], epsilon) &&
               vjIsEqual(mxLLCorner[VJ_Z], mxULCorner[VJ_Z], epsilon));
   }

   //: Check the pts to make sure they form a legal surface
   void assertPtsLegal()
   {
      vjVec3 norm1, norm2;
      vjVec3 bot_side = mLRCorner-mLLCorner;
      vjVec3 diag = mULCorner-mLRCorner;
      vjVec3 right_side = mURCorner-mLRCorner;
      norm1 = bot_side.cross(diag);
      norm2 = bot_side.cross(right_side);
      norm1.normalize(); norm2.normalize();
      if(norm1 != norm2)
         vjDEBUG(vjDBG_ERROR,0) << "ERROR: Invalid surface corners.\n" << vjDEBUG_FLUSH;
   }


protected:
   vjVec3   mLLCorner, mLRCorner, mURCorner, mULCorner;  //: The corners in 3Space (for config)
   vjMatrix mSurfaceRotation;                            //: surfMbase - rotation to base coordinate frame of the surface view plane

   // Deal with tracked surfaces (ie. HMD, movable walls, desks, etc)
   bool           mTracked;            // Is this surface tracked
   std::string    mTrackerProxyName;   // If tracked, what is the name of the tracker

   /// Defines the projection for this window. Ex. RIGHT, LEFT, FRONT
   vjProjection*   mLeftProj;              //: Left eye projection
   vjProjection*   mRightProj;             //: Right eye projection

private:
         // These values are used to compute the coordinates of the view plane
         // in the transformed coord system of mSurfaceRotation
   vjVec3   mxLLCorner, mxLRCorner, mxURCorner, mxULCorner;    //: The corners transformed onto an x,y plane
};

#endif
