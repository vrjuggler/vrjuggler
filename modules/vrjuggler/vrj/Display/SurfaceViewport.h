/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
#include <jccl/Config/ConfigChunkPtr.h>

#include <gmtl/Point.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>

namespace vrj
{


/**
 * Defines a display surface an associated projections
 */
class SurfaceViewport : public Viewport
{
public:
   SurfaceViewport() :  mTracked(false)
   {;}

   virtual ~SurfaceViewport() {}

public:
   /**
    * Takes a display chunk and configures the display based one it.
    * @pre chunk is a valid chunk.
    * @post display is configured.
    *        If there is an error is the specified config, we output error
    *        and "fix" the error.
    * @note All derived display classes MUST call this function
    *        after doing local configuration.
    */
   virtual void config(jccl::ConfigChunkPtr chunk);

   virtual void updateProjections(const float positionScale);

   void getCorners(gmtl::Point3f& ll, gmtl::Point3f& lr, gmtl::Point3f& ur,
                   gmtl::Point3f& ul)
   {
      ll = mLLCorner; lr = mLRCorner; ur = mURCorner; ul = mULCorner;
   }

   virtual std::ostream& outStream(std::ostream& out,
                                   const unsigned int indentLevel = 0);

protected:
   void calculateSurfaceRotation();

   void calculateCornersInBaseFrame();

   /** Checks the pts to make sure they form a legal surface. */
   void assertPtsLegal()
   {
      gmtl::Vec3f norm1, norm2;
      gmtl::Vec3f bot_side = mLRCorner-mLLCorner;
      gmtl::Vec3f diag = mULCorner-mLRCorner;
      gmtl::Vec3f right_side = mURCorner-mLRCorner;
      gmtl::cross(norm1, bot_side, diag);
      gmtl::cross(norm2, bot_side, right_side);
      gmtl::normalize( norm1 ); gmtl::normalize(norm2);
      if(norm1 != norm2)
         vprDEBUG(vprDBG_ERROR,0) << "ERROR: Invalid surface corners.\n" << vprDEBUG_FLUSH;
   }


protected:
   gmtl::Point3f   mLLCorner, mLRCorner, mURCorner, mULCorner;  /**< The corners in 3Space (for config) */
   gmtl::Matrix44f mSurfaceRotation;                            /**< baseMsurf - rotation to base coordinate frame of the surface view plane */

   // Deal with tracked surfaces (ie. HMD, movable walls, desks, etc)
   bool           mTracked;            /**< Is this surface tracked? */
   std::string    mTrackerProxyName;   /**< If tracked, what is the name of the tracker */

private:
         // These values are used to compute the coordinates of the view plane
         // in the transformed coord system of mSurfaceRotation
   gmtl::Point3f  mxLLCorner, mxLRCorner, mxURCorner, mxULCorner;    /**< The corners transformed onto an x,y plane */
};

}

#endif
