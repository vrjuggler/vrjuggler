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

#ifndef _VRJ_PROJECTION_H_
#define _VRJ_PROJECTION_H_

#include <vrj/vrjConfig.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>

#include <jccl/Config/ConfigElementPtr.h>
#include <vrj/Display/Frustum.h>
#include <vrj/Display/Viewport.h>


namespace vrj
{

   class Viewport;

/** \class Projection Projection.h vrj/Display/Projection.h
 *
 * Pure virtual base class for projection definitions.
 *
 * Responsible for storing and computing projection information based upon an
 * eye positions.  This class is an abstract base class for other classes
 * that actually compute the projections.
 */
class VJ_CLASS_API Projection
{
public:
   /** Eye type. */
   enum Eye
   {
      LEFT = 1,   /**< Left eye */
      RIGHT = 2   /**< Right eye */
   };

public:
   Projection();

   Projection(const Projection& p);

   virtual ~Projection()
   {
      ;
   }

   /**
    * Configures the projection.  The default implementation does nothing.
    */
   virtual void config(jccl::ConfigElementPtr element);

   /** Sets the eye for this projection. */
   void setEye(Projection::Eye _eye)
   {
      mEye = _eye;
   }

   /** Returns the eye for this projection. */
   Projection::Eye getEye() const
   {
      return mEye;
   }

   /** Sets the viewport associated with this projection. */
   void setViewport(Viewport* vp)
   {
      mViewport = vp;
   }

   /** Returns the viewport associated with this projection. */
   Viewport* getViewport() const
   {
      return mViewport;
   }

   /**
    * Calcualtes the view matrix.
    *
    * @pre eyePos is scaled by position scale factor.  scaleFactor is the
    *      scale currently used.
    */
   virtual void calcViewMatrix(const gmtl::Matrix44f& eyePos,
                               const float scaleFactor) = 0;

   /**
    * Helper to the get the frustum apex and corners in model coordinates.
    *
    * @post The given vars contain the values of the frusta corners in model
    *       space.
    *
    * @note This function is meant for debugging purposes.
    */
   void getFrustumApexAndCorners(gmtl::Vec3f& apex, gmtl::Vec3f& ur,
                                 gmtl::Vec3f& lr, gmtl::Vec3f& ul,
                                 gmtl::Vec3f& ll) const;

   /** Returns this projection's view matrix. */
   const gmtl::Matrix44f& getViewMatrix() const
   {
      return mViewMat;
   }

   /** Returns a constant reference to this projection's frustum. */
   const vrj::Frustum& getFrustum() const
   {
      return mFrustum;
   }

   /** Virtual output oporators.
    * Every class derived from us shoudl just define this, and
    * the opertetor<< will "just work".
    */
   virtual std::ostream& outStream(std::ostream& out,
                                   const unsigned int indentLevel = 0);

   friend VJ_API(std::ostream&) operator<<(std::ostream& out, Projection& proj);


protected:
   gmtl::Matrix44f   mViewMat;     /**< The view transformation matrix for this projection */
   Frustum           mFrustum;     /**< The calculated view frustum for this projection */

   Eye         mEye;          /**< The eye that this projection is rendering */
   Viewport*   mViewport;     /**< The containing viewport for the projection. Used in some projections to get size */

   float       mFocusPlaneDist;     /**< Basically the distance to the surface.  Needed for drawing surface in simulator. */

protected:
   /** @name Static near/far values */
   //@{
   static float mNearDist;    /**< The near distance */
   static float mFarDist;     /**< The far distance */
   //@}

public:
   /** Sets the system-wide near and far values. */
   static void setNearFar(const float nearVal, const float farVal);

   /**
    * Retrieves the system-wide near and far values.
    *
    * @param nearVal Storage for the returned near value.
    * @param farVal  Storage for the returned far value.
    *
    * @since 2.1.4
    */
   static void getNearFar(float& nearVal, float& farVal);
};

}

#endif
