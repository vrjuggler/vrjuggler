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

#ifndef _VRJ_SURFACE_DISPLAY_H_
#define _VRJ_SURFACE_DISPLAY_H_

#include <vrj/vrjConfig.h>

#include <vrj/Util/Debug.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Display/Projection.h>
#include <vrj/Math/Vec3.h>

namespace jccl {
    class ConfigChunk;
};

namespace vrj
{


//: Defines a display surface an associated projections
//
//
class SurfaceViewport : public Viewport
{
public:
   SurfaceViewport() :  mTracked(false), mLeftProj(NULL), mRightProj(NULL)
   {;}

   virtual ~SurfaceViewport() {}

public:
   //: Takes a display chunk and configures the display based one it.
   //! PRE: chunk is a valid chunk
   //! POST: display is configured
   //+       If there is an error is the specified config, we output error
   //+       and "fix" the error.
   //! NOTE: All derived display classes MUST call this function
   //+       after doing local configuration.
   virtual void config(jccl::ConfigChunk *chunk);

   virtual void updateProjections();

   void getCorners(Vec3& ll, Vec3& lr, Vec3& ur, Vec3& ul)
   {
      ll = mLLCorner; lr = mLRCorner; ur = mURCorner; ul = mULCorner;
   }

   Projection* getLeftProj()
   { return mLeftProj; }

   Projection* getRightProj()
   { return mRightProj; }

   virtual std::ostream& outStream(std::ostream& out)
   {
      Viewport::outStream(out);

      out << "LL: " << mLLCorner << ", LR: " << mLRCorner
          << ", UR: " << mURCorner << ", UL:" << mULCorner << std::endl;
      out << "surfRot: \n" << mSurfaceRotation << std::endl;
      out << "leftProj: " << *mLeftProj;
      out << "  rightProj: " << *mRightProj << std::endl;
      return out;
   }

protected:
   void calculateSurfaceRotation();

   void calculateCornersInBaseFrame();

   //: Check the pts to make sure they form a legal surface
   void assertPtsLegal()
   {
      Vec3 norm1, norm2;
      Vec3 bot_side = mLRCorner-mLLCorner;
      Vec3 diag = mULCorner-mLRCorner;
      Vec3 right_side = mURCorner-mLRCorner;
      norm1 = bot_side.cross(diag);
      norm2 = bot_side.cross(right_side);
      norm1.normalize(); norm2.normalize();
      if(norm1 != norm2)
         vprDEBUG(vprDBG_ERROR,0) << "ERROR: Invalid surface corners.\n" << vprDEBUG_FLUSH;
   }


protected:
   Vec3   mLLCorner, mLRCorner, mURCorner, mULCorner;  //: The corners in 3Space (for config)
   Matrix mSurfaceRotation;                            //: surfMbase - rotation to base coordinate frame of the surface view plane

   // Deal with tracked surfaces (ie. HMD, movable walls, desks, etc)
   bool           mTracked;            // Is this surface tracked
   std::string    mTrackerProxyName;   // If tracked, what is the name of the tracker

   /// Defines the projection for this window. Ex. RIGHT, LEFT, FRONT
   Projection*   mLeftProj;              //: Left eye projection
   Projection*   mRightProj;             //: Right eye projection

private:
         // These values are used to compute the coordinates of the view plane
         // in the transformed coord system of mSurfaceRotation
   Vec3   mxLLCorner, mxLRCorner, mxURCorner, mxULCorner;    //: The corners transformed onto an x,y plane
};

};
#endif
