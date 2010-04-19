/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _VRJ_SURFACE_PROJECTION_H_
#define _VRJ_SURFACE_PROJECTION_H_

#include <vrj/vrjConfig.h>

#include <gmtl/Point.h>
#include <gmtl/Matrix.h>

#include <vrj/Display/Projection.h>
#include <vrj/Display/SurfaceProjectionPtr.h>


namespace vrj
{

/** \class SurfaceProjection SurfaceProjection.h vrj/Display/SurfaceProjection.h
 *
 * Surface specific class for projection definitions.
 *
 * Responsible for storing and computing projection information of a surface
 * specified.
 */
class SurfaceProjection : public Projection
{
protected:
   /**
    * Constructs a new surface projection using the given corner points.
    *
    * @param llCorner Lower left corner.
    * @param lrCorner Lower right corner.
    * @param urCorner Upper right corner.
    * @param ulCorner Upper left corner.
    */
   SurfaceProjection(const gmtl::Point3f& llCorner,
                     const gmtl::Point3f& lrCorner,
                     const gmtl::Point3f& urCorner,
                     const gmtl::Point3f& ulCorner);

public:
   /**
    * Constructs a new surface projection using the given corner points.
    *
    * @param llCorner Lower left corner.
    * @param lrCorner Lower right corner.
    * @param urCorner Upper right corner.
    * @param ulCorner Upper left corner.
    *
    * @since 2.3.4
    */
   static SurfaceProjectionPtr create(const gmtl::Point3f& llCorner,
                                      const gmtl::Point3f& lrCorner,
                                      const gmtl::Point3f& urCorner,
                                      const gmtl::Point3f& ulCorner);

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

   /**
    * Changes the corners originally specified whn created.
    * An exception of type vrj::InvalidSurfaceException is thrown.
    */
   void updateCorners(const gmtl::Point3f& llCorner,
                      const gmtl::Point3f& lrCorner,
                      const gmtl::Point3f& urCorner,
                      const gmtl::Point3f& ulCorner);

   /** Configures the projection using the given element. */
   virtual void config(jccl::ConfigElementPtr element);

   /**
    * Recalculates the projection matrix. This uses a method that needs to
    * know the distance in the screen plane from the origin (determined by the
    * normal to the plane throught the origin) to the edges of the screen.
    * This method can be used for any rectangular planar screen. By adjusting
    * the wall rotation matrix, this method can be used for the general case
    * of a rectangular screen in 3-space.
    *
    * @pre WallRotation matrix must be set correctly. All \c mOrigin*
    *      values must be set correctly. \p eyePos must be set correctly.
    *      \p scaleFactor is the current scaling in use.
    * @post \c mFrustum has been recomputed for the given eye position.
    *
    * @param eyePos      The eye position.
    * @param scaleFactor The scale factor currently in use.
    */
   virtual void calcViewMatrix(const gmtl::Matrix44f& eyePos,
                               const float scaleFactor);

   /**
    * Calculates the view frustum needed for the view matrix. This uses a
    * method that needs to know the distance in the screen plane from the
    * origin (determined by the normal to the plane through the origin) to the
    * edges of the screen. This method can be used for any rectangular planar
    * screen. By adjusting the wall rotation matrix, this method can be used
    * for the general case of a rectangular screen in 3-space.
    *
    * @pre \c m_base_M_surface and \c m_surface_M_base matrices must be set
    *      correctly. All the \c mOrigin* values must be set correctly.
    *
    * @param eyePos      The eye position.
    * @param scaleFactor The scale factor currently in use.
    *
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
    * These calculate \c mOriginToScreen, etc, from the screen corners.
    * calculateOffsets() requires that \c mLLCorner to \c mULCorner be set
    * correctly, and it will handle calling calculateSurfaceRotation() and
    * calculateCornersInBaseFrame().
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
    * Surface           - S  - Cordinate frame that is aligned with the
    *                          surface. This is the coordinates that. The
    *                          surface and frustum (projection) parameters are
    *                          stored in/relative to.<br>
    * Surface Transform - ST - Coordinate frame that offsets the Surface in
    *                          space. Only used for tracked surfaces.<br>
    * Eye               - E  - Coordinate system that is aligned with the eye.
    *                          This is the frame that the frustum is drawn in
    *                          for final projection. This is where the "view"
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
