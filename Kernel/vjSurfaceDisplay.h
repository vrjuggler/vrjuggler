#ifndef _VJ_SURFACE_DISPLAY_H_
#define _VJ_SURFACE_DISPLAY_H_

#include <Kernel/vjDisplay.h>
#include <Kernel/vjProjection.h>
#include <Kernel/vjWallProjection.h>

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

      char* proj  = chunk->getProperty("projectiontype");

      // Create Projection objects
      mLeftProj = new vjWallProjection;
      mRightProj = new vjWallProjection;

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

      vjDEBUG(4) << "vjDisplay::updateProjections: Getting head position" << endl << vjDEBUG_FLUSH;
      vjDEBUG(4) << "\tHeadPos:" << head_coord.pos << "\tHeadOr:" << head_coord.orient << endl << vjDEBUG_FLUSH;

      // Compute location of left and right eyes
      float interocularDist = 2.75/12.0f;
      float eye_offset = interocularDist/2.0f;      // Distance to move eye

      left_eye_pos.postTrans(cur_head_pos, -eye_offset, 0, 0);
      right_eye_pos.postTrans(cur_head_pos, eye_offset, 0, 0);

      mLeftProj->calcViewMatrix(left_eye_pos);
      mRightProj->calcViewMatrix(right_eye_pos);
   }

   vjProjection* getLeftProj()
   { return mLeftProj; }

   vjProjection* getRightProj()
   { return mRightProj; }

protected:
   /// Defines the projection for this window. Ex. RIGHT, LEFT, FRONT
   vjProjection*   mLeftProj;              //: Left eye projection
   vjProjection*   mRightProj;             //: Right eye projection

};
#endif

