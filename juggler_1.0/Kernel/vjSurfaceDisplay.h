/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_SURFACE_DISPLAY_H_
#define _VJ_SURFACE_DISPLAY_H_

#include <Kernel/vjDebug.h>
#include <Kernel/vjDisplay.h>
#include <Kernel/vjProjection.h>
#include <Kernel/vjWallProjection.h>
#include <Math/vjVec3.h>

//: Defines a display surface an associated projections
//
//
class vjSurfaceDisplay : public vjDisplay
{
public:
   vjSurfaceDisplay() : mLeftProj(NULL), mRightProj(NULL)
   {;}

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


      // Create Projection objects
      // NOTE: The -'s are because we are measuring distance to
      //  the left(bottom) which is opposite the normal axis direction
      vjMatrix rot_inv;
      rot_inv.invert(mSurfaceRotation);
      mLeftProj = new vjWallProjection(mSurfaceRotation,-mxLLCorner[VJ_Z],
                                       mxLRCorner[VJ_X],-mxLLCorner[VJ_X],
                                       mxURCorner[VJ_Y],-mxLRCorner[VJ_Y]);
      mRightProj = new vjWallProjection(mSurfaceRotation,-mxLLCorner[VJ_Z],
                                       mxLRCorner[VJ_X],-mxLLCorner[VJ_X],
                                       mxURCorner[VJ_Y],-mxLRCorner[VJ_Y]);

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

      vjDEBUG(vjDBG_ALL,5) << "vjDisplay::updateProjections: Getting head position" << endl << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_ALL,5) << "\tHeadPos:" << head_coord.pos << "\tHeadOr:" << head_coord.orient << endl << vjDEBUG_FLUSH;

      // Compute location of left and right eyes
      float interocularDist = 2.75/12.0f;
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
      vjDEBUG(vjDBG_ALL,5) << mxLLCorner[VJ_Z]  << " " << mxLRCorner[VJ_Z]
                         << " " <<  mxURCorner[VJ_Z]  << " " <<  mxULCorner[VJ_Z] << "\n" << vjDEBUG_FLUSH;
      vjASSERT((mxLLCorner[VJ_Z] == mxLRCorner[VJ_Z]) &&
               (mxURCorner[VJ_Z] == mxULCorner[VJ_Z]) &&
               (mxLLCorner[VJ_Z] == mxULCorner[VJ_Z]));
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
   vjVec3   mLLCorner, mLRCorner, mURCorner, mULCorner;  //: The corners in 3Space
   vjMatrix mSurfaceRotation;                            //: baseMsurf

   /// Defines the projection for this window. Ex. RIGHT, LEFT, FRONT
   vjProjection*   mLeftProj;              //: Left eye projection
   vjProjection*   mRightProj;             //: Right eye projection

private:
   vjVec3   mxLLCorner, mxLRCorner, mxURCorner, mxULCorner;    //: The corners transformed onto an x,y plane
};

#endif
