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

#ifndef _VRJ_SURFACE_DISPLAY_H_
#define _VRJ_SURFACE_DISPLAY_H_

#include <vrj/vrjConfig.h>

#include <gmtl/Point.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>

#include <jccl/Config/ConfigElementPtr.h>

#include <vrj/Display/Viewport.h>
#include <vrj/Display/SurfaceViewportPtr.h>


namespace vrj
{

/** \class SurfaceViewport SurfaceViewport.h vrj/Display/SurfaceViewport.h
 *
 * Defines a display surface an associated projections.
 */
class VJ_CLASS_API SurfaceViewport : public Viewport
{
protected:
   SurfaceViewport();

public:
   enum CornerUpdateMode
   {
      NO_UPDATE = 0,
      RESIZE_ONLY,
      RESIZE_AND_MOVE,
      RESIZE_WIDTH_ONLY
   };

   /**
    * Creates a SurfaceViewport instance and returns it wrapped in a
    * ViewportPtr object.
    *
    * @since 2.3.3
    */
   static ViewportPtr create();

   virtual ~SurfaceViewport();

   /**
    * Takes a display element and configures the viewport based on it.
    *
    * @pre element is a valid configuration element.
    * @post display is configured.
    *        If there is an error is the specified config, we output error
    *        and "fix" the error.
    *
    * @note All derived display classes MUST call this function
    *        after doing local configuration.
    */
   virtual bool config(jccl::ConfigElementPtr element);

   virtual void updateProjections(const float positionScale);

   void getCorners(gmtl::Point3f& ll, gmtl::Point3f& lr, gmtl::Point3f& ur,
                   gmtl::Point3f& ul)
      const
   {
      ll = mLLCorner;
      lr = mLRCorner;
      ur = mURCorner;
      ul = mULCorner;
   }
  
   /** Compute values that can convert screen info to corner updates. */
   void computePixelTransforms();
   
   /** Update corners using current Display data. */
   void updateCorners();

   virtual std::ostream& outStream(std::ostream& out,
                                   const unsigned int indentLevel = 0);

protected:
   /** @name The corners in 3Space (for config) */
   //@{
   gmtl::Point3f mLLCorner;
   gmtl::Point3f mLRCorner;
   gmtl::Point3f mURCorner;
   gmtl::Point3f mULCorner;
   //@}

   /**
    * @name Tracked surface info
    * Deal with tracked surfaces (HMD, movable walls, desks, etc).
    */
   //@{
   bool           mTracked;            /**< Is this surface tracked? */
   std::string    mTrackerProxyName;   /**< If tracked, what is the name of the tracker */
   //@}

   /**
    * @name Pixel-to-Physical conversion info
    * Deal with resize of surface viewports.
    */
   //@{
   gmtl::Vec3f mHDirection;
   gmtl::Vec3f mVDirection;
   float mPixelsPerUnitX;
   float mPixelsPerUnitY;
   gmtl::Point3f mIntercept;
   float mVpOriginX;
   float mVpOriginY;
   float mVpWidth;
   float mVpHeight;
   CornerUpdateMode mCornerUpdateMode;
   //@{

   void computePixelOriginAndSize();
};

}

#endif
