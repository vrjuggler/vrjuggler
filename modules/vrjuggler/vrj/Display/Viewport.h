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

#ifndef _VRJ_VIEWPORT_H_
#define _VRJ_VIEWPORT_H_

#include <vrj/vrjConfig.h>

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <gmtl/Vec.h>
#include <jccl/Config/ConfigElementPtr.h>

#include <vrj/Display/DisplayPtr.h>
#include <vrj/Display/ViewportPtr.h>
#include <vrj/Kernel/UserPtr.h>


namespace vrj
{

class Projection;

/** \class Viewport Viewport.h vrj/Display/Viewport.h
 *
 * Base class for window viewports and for all viewport data.
 * Stores projection data, viewport info, and relevant user.
 *
 * @date 3-5-2001
 */
class VJ_CLASS_API Viewport
   : public boost::enable_shared_from_this<Viewport>
   , boost::noncopyable
{
protected:
   Viewport();

public:
   virtual ~Viewport()
   {
      ;
   }

   /** Type of viewport. */
   enum Type
   {
      UNDEFINED,  /**< Undefined type */
      SURFACE,    /**< Surface viewport */
      SIM         /**< Simulator viewport */
   };

   /** Used for referencing which eye(s) to draw. */
   enum View
   {
      NONE=0,       /**< Neither eye */
      LEFT_EYE=1,   /**< Left eye only */
      RIGHT_EYE=2,  /**< Right eye only */
      STEREO=3      /**< Both left and right eyes */
   };

   /**
    * Takes a viewport element and configures the display based one it.
    *
    * @pre element is a valid configuration element.
    * @post viewport is configured.
    *       If there is an error is the specified config, we output error
    *       and "fix" the error.
    *
    * @note All derived viewport classes MUST call this function
    *       after doing local configuration.
    */
   virtual bool config(jccl::ConfigElementPtr element);

   /**
    * Updates the projection data for this viewport.  This uses the data from
    * the head position for this viewport.
    *
    * @param positionScale Scale value for converting from Juggler units
    *                      (meters) to the display units
    */
   virtual void updateProjections(const float positionScale) = 0;

   /** Gets the type of this viewport. */
   Viewport::Type getType() const
   {
      return mType;
   }

   /** Determines if this is a simulator viewport. */
   bool isSimulator() const
   {
      return (mType == SIM);
   }

   /** Determines if this is a surface viewport. */
   bool isSurface() const
   {
      return (mType == SURFACE);
   }

   /** Determines if this viewport is active. */
   bool isActive() const
   {
      return mActive;
   }

   /** Sets the name of this viewport. */
   void setName(const std::string& name)
   {
      mName = name;
   }

   /** Gets the name of this display. */
   const std::string& getName() const
   {
      return mName;
   }

   /**
    * Determines if this viewport is rendering in stereo.
    *
    * @note If we are in simulator, we can not be in stereo.
    */
   bool inStereo() const
   {
      return (mView == STEREO);
   }

   /** Which view are we supposed to render? */
   Viewport::View getView() const
   {
      return mView;
   }

   /** Sets the origin and size of this viewport. */
   void setOriginAndSize(const float xOrigin, const float yOrigin,
                         const float width, const float height)
   {
      mXorigin = xOrigin;
      mYorigin = yOrigin;
      mXsize   = width;
      mYsize   = height;
   }

   /** Gets the origin and size of this viewport. */
   void getOriginAndSize(float& xOrigin, float& yOrigin, float& width,
                         float& height)
      const
   {
      xOrigin = mXorigin;
      yOrigin = mYorigin;
      width   = mXsize;
      height  = mYsize;
   }

   /** Gets the config element that configured this viewport. */
   jccl::ConfigElementPtr getConfigElement() const
   {
      return mViewportElement;
   }

   /** Gets the user associated with this viewport. */
   UserPtr getUser() const
   {
      return mUser;
   }

   /** Sets the display containing this viewport. */
   void setDisplay(DisplayPtr disp)
   {
      mDisplay = disp;
   }

   /** Gets the display containing this viewport. */
   DisplayPtr getDisplay() const
   {
      return mDisplay.lock();
   }

   /** Gets the left projection. */
   Projection* getLeftProj() const
   {
      return mLeftProj;
   }

   /** Gets the right projection. */
   Projection* getRightProj() const
   {
      return mRightProj;
   }

   virtual std::ostream& outStream(std::ostream& out,
                                   const unsigned int indentLevel = 0);

   friend VJ_API(std::ostream&) operator<<(std::ostream& out, Viewport& viewport);

protected:
   std::string       mName;               /**< The name of the viewport being displayed */
   UserPtr           mUser;               /**< The user being rendered by this window */
   Viewport::Type    mType;               /**< The type of display */
   Viewport::View    mView;               /**< Which buffer(s) to display (left, right, stereo) */
   bool              mActive;             /**< Is this viewport active */

   DisplayWeakPtr    mDisplay;            /**< The parent display */

   jccl::ConfigElementPtr mViewportElement; /**< The config data for this display */

   /**
    * @name Location and size of viewport
    * ASSERT: all values are >= 0.0 and <= 1.0
    */
   //@{
   float mXorigin;
   float mYorigin;
   float mXsize;
   float mYsize;
   //@}

   /**
    * @name Projections
    * Defines the projection for this window.
    */
   //@{
   Projection* mLeftProj;      /**< Left eye projection */
   Projection* mRightProj;     /**< Right eye projection */
   //@}
};

}


#endif
