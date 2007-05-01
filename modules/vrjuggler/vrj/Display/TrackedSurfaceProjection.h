/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_TRACKED_WALL_PROJECTION_H_
#define _VRJ_TRACKED_WALL_PROJECTION_H_

#include <vrj/vrjConfig.h>
#include <vrj/Display/SurfaceProjection.h>
#include <vrj/Util/Debug.h>
#include <gadget/Type/PositionInterface.h>


namespace vrj
{

class Matrix;

/** \class TrackedSurfaceProjection TrackedSurfaceProjection.h vrj/Display/TrackedSurfaceProjection.h
 *
 * Wall-specific class for viewport definitions.
 *
 * This class behaves the same as the SurfaceProjection class except that it is
 * tracked.  This means that all parameters are relative to a tracked (moving)
 * frame of reference.
 */
class TrackedSurfaceProjection : public SurfaceProjection
{
public:
   /**
    * Constructs a Tracked wall projection.
    *
    * @param llCorner    Lower left corner.
    * @param lrCorner    Lower right corner.
    * @param urCorner    Upper right corner.
    * @param ulCorner    Upper left corner.
    * @param trackerName Name of the tracker tracking the screen.
    */
   TrackedSurfaceProjection(const gmtl::Point3f& llCorner,
                            const gmtl::Point3f& lrCorner,
                            const gmtl::Point3f& urCorner,
                            const gmtl::Point3f& ulCorner,
                            const std::string& trackerName);

   virtual ~TrackedSurfaceProjection()
   {
      /* Do nothing. */ ;
   }

   virtual void config(jccl::ConfigElementPtr element)
   {
      Projection::config(element);        // Call base class config first
   }

   /**
    * Recalculate the projection matrix.
    *
    * @pre \p eyePos is scaled by position factor.  \p scaleFactor is the
    *      scale currently used.
    * @post Frustum has been recomputed for given \p eyePos.
    */
   virtual void calcViewMatrix(const gmtl::Matrix44f& eyePos,
                               const float scaleFactor);

   /** Update the parameters of the tracked surface. */
   void updateSurfaceParams(const float scaleFactor);

   std::ostream& outStream(std::ostream& out,
                           const unsigned int indentLevel = 0);

private:
   /**
    * Tracker connected to the surface.
    * The surface coordinate system is relative to this tracker
    * base_M_surfacetrans.
    */
   gadget::PositionInterface    mTracker;

   /**
    * Original surface coordinates.
    * TrackedSurfaceProjections keep these so they can rotate them and feed
    * them into the SurfaceProjection calculations.
    */
   //@{
   gmtl::Point3f mOriginalLLCorner;
   gmtl::Point3f mOriginalLRCorner;
   gmtl::Point3f mOriginalURCorner;
   gmtl::Point3f mOriginalULCorner;
   //@}
};

}


#endif
