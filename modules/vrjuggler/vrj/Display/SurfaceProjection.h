/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _VRJ_WALL_PROJECTION_H_
#define _VRJ_WALL_PROJECTION_H_

#include <vrj/vrjConfig.h>
#include <vrj/Display/Projection.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Point.h>

namespace vrj
{

class Matrix;

/** \class SurfaceProjection SurfaceProjection.h vrj/Display/SurfaceProjection.h
 *
 * Surface specific class for projection definitions.
 *
 * Responsible for storing and computing projection information of a surface
 * specified.
 */
class SurfaceProjection : public Projection
{
public:
   /**
    * @param llCorner Lower left corner.
    * @param lrCorner Lower right corner.
    * @param urCorner Upper right corner.
    * @param ulCorner Upper left corner.
    */
   SurfaceProjection(const gmtl::Point3f& llCorner,
                     const gmtl::Point3f& lrCorner,
                     const gmtl::Point3f& urCorner,
                     const gmtl::Point3f& ulCorner)
      : mLLCorner(llCorner)
      , mLRCorner(lrCorner)
      , mURCorner(urCorner)
      , mULCorner(ulCorner)
   {
      /* Do nothing. */ ;
   }

   /**
    * Checks the corner points to make sure they form a legal surface. If not,
    * an exception of type vrj::InvalidSurfaceException is thrown.
    *
    * @throw vrj::InvalidSurfaceException is thrown if the corners do not form
    *        a legal surface.
    *
    * @since 2.1.11
    */
   void validateCorners();

   /** Configures the projection using the element given. */
   virtual void config(jccl::ConfigElementPtr element);

   /**
    * Recalculates the projection matrix.
    * @pre WallRotation matrix must be set correctly.
    *      mOrigin*'s must all be set correctly.
    * @pre eyePos is scaled by position factor.
    * @pre scaleFactor is the scale current used
    * @post frustum has been recomputed for given eyePos.
    */
   virtual void calcViewMatrix(const gmtl::Matrix44f& eyePos,
                               const float scaleFactor);

   /**
    * Calculates the frustum needed for the view matrix.
    * @note This function is called as part of calcViewMatrix.
    */
   virtual void calcViewFrustum(const gmtl::Matrix44f& eyePos,
                                const float scaleFactor);

   std::ostream& outStream(std::ostream& out,
                           const unsigned int indentLevel = 0);

protected:
   /**
    * @name Screen calculation functions
    *
    * These calculate mOriginToScreen, etc, from the screen corners.
    * calculateOffsets requires that mLLCorner to mULCorner be set correctly,
    * and it will handle calling calculateSurfaceRotation and
    * calculateCornersInBaseFrame.
    */
   //@{
   void calculateOffsets();
   void calculateSurfaceRotation();
   void calculateCornersInBaseFrame();
   //@}

   /**
    * @name Coordinate systems
    *
    * Coordinate system descriptions:
    *
    * Base              - B  - Base coordinate system of the real physical
    *                          world.<br>
    * Surface           - S  - Cordinate frame that is aligned with the surface.
    *                          This is the coordinates that.  The surface and
    *                          frustum (projection) parameters are stored
    *                          in/relative to.<br>
    * Surface Transform - ST - Coordinate frame that offsets the Surface in
    *                          space.  Only used for tracked surfaces.<br>
    * Eye               - E  - Coordinate system that is aligned with the eye.
    *                          This is the frame that the frustum is drawn in
    *                          for final projection.  This is where the "view"
    *                          starts from.
    */
   //@{
   gmtl::Matrix44f mSurfaceRotation;	/**< Same as m_base_M_surface */

   /*
    * @name Rotation of the surface
    * Transform from the Base to the surface.
    */
   gmtl::Matrix44f   m_base_M_surface;
   gmtl::Matrix44f   m_surface_M_base;  /**< Stored inverse for performance */
   //@}

   /** @name Screen configuration (in Surface coordinate frame) */
   //@{
   gmtl::Point3f mLLCorner;
   gmtl::Point3f mLRCorner;
   gmtl::Point3f mURCorner;
   gmtl::Point3f mULCorner;
   //@}

   /** @name Corners transformed onto an XY plane */
   //@{
   gmtl::Point3f mxLLCorner;
   gmtl::Point3f mxLRCorner;
   gmtl::Point3f mxURCorner;
   gmtl::Point3f mxULCorner;
   //@}

   float mOriginToScreen;
   float mOriginToRight;
   float mOriginToLeft;
   float mOriginToTop;
   float mOriginToBottom;
};

}

#endif
