/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#ifndef _VRJ_PROJECTION_H_
#define _VRJ_PROJECTION_H_

#include <vrj/vrjConfig.h>
#include <vrj/Display/Frustum.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Viewport.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>

#include <jccl/Config/ConfigChunk.h>

namespace vrj
{

   class Viewport;

/** Pure virtual base class for viewport definitions.
 *
 * Responsible for storing and computing projection information based upon an
 * eye positions.  This class is an abstract base class for other classes
 * that actually compute the projections.
 */
class VJ_CLASS_API Projection
{
public:
   /** Eye and type */
   enum Eye
   {
      LEFT = 1,   /**< Left eye */
      RIGHT = 2   /**< Right eye */
   };

public:
   Projection()
   {
      mEye = LEFT;
      mFocusPlaneDist = 1.0f;
      mViewport = NULL;
   }

   /** Configures the projection.
   * Default implementation does nothing.
   */
   virtual void config(jccl::ConfigChunkPtr chunk)
   {;}

   void setEye(Projection::Eye _eye)
   { mEye = _eye; }

   int getEye()
   { return mEye;}

   void setViewport(Viewport* vp)
   { mViewport = vp; }
   Viewport* getViewport()
   { return mViewport; }

   virtual void calcViewMatrix(gmtl::Matrix44f& eyePos) = 0;

   /** Helper to the frustum apex and corners in model coordinates.
   * @note This function is meant for debugging purposes
   * @post The given vars contain the values of the frustums
   * corners in model space.
   */
   void getFrustumApexAndCorners(gmtl::Vec3f& apex, gmtl::Vec3f& ur, gmtl::Vec3f& lr,
                                 gmtl::Vec3f& ul, gmtl::Vec3f& ll);

   /** Virtual output oporators.
   * Every class derived from us shoudl just define this, and
   * the opertetor<< will "just work"
   */
   virtual std::ostream& outStream(std::ostream& out);

   friend VJ_API(std::ostream&) operator<<(std::ostream& out, Projection& proj);


public:
   gmtl::Matrix44f   mViewMat;     /**< The view transformation matrix for this projection */
   Frustum           mFrustum;     /**< The calculated view frustum for this projection */

protected:
   Eye         mEye;          /**< The eye that this projection is rendering */
   Viewport*   mViewport;     /**< The containing viewport for the projection. Used in some projections to get size */

   float       mFocusPlaneDist;     // Basically the distance to the surface.  Needed for drawing surface in simulator.

protected:
   /** @name Static near/far values */
   //@{
   static float mNearDist;    /**< The near distance */
   static float mFarDist;     /**< The far distance */
   //@}

public:
   /** Set the system wide near and far values */
   static void setNearFar(float near_val, float far_val);
};

};

#endif
