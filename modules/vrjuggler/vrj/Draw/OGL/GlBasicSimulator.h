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

#ifndef _VRJ_GL_BASIC_SIMULATOR_H_
#define _VRJ_GL_BASIC_SIMULATOR_H_

#include <vrj/vrjConfig.h>

#include <vrj/Draw/OGL/GlSimInterface.h>
#include <vrj/Draw/OGL/GlDrawObjectFunctor.h>
#include <vrj/Draw/OGL/GlDrawWandFunctors.h>

#ifdef VPR_OS_Darwin
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

#include <gadget/Type/PositionInterface.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>


namespace vrj
{
/**
 * Interface for objects that wish to perform simulator function with an
 * OpenGL application.
 */
class GlBasicSimulator : public GlSimInterface
{
public:
   GlBasicSimulator();

   virtual ~GlBasicSimulator() {}

   /*
   * Configure the basic simulator config
   * @pre chunk is a valid chunk.
   * @post It should be configured
   */
   virtual bool config(jccl::ConfigChunkPtr chunk);

   static std::string getChunkType()
   {
      return std::string("default_simulator");
   }

   /**
    * Draws this sim device using the given information about the Window it
    * will be drawing into.
    */
   virtual void draw(const float scaleFactor);

   virtual void initialize(SimViewport* simVp)
   {
      mSimViewport = simVp;
   }

   /**
    * Sets the event window the simulator can use to get input from the user.
    */
   virtual void setEventWindow(gadget::EventWindowInterface ewInterface);

public:
   gmtl::Matrix44f getCameraPos()
   { return mCameraPos; }

   gmtl::Matrix44f getHeadPos()
   { return mHeadPos; }

   gmtl::Matrix44f getWandPos()
   { return mWandPos; }

   virtual void updateProjectionData(const float positionScale,
                                     Projection* leftProj, Projection* rightProj);

   /**  Update internal simulator data */
   void updateInternalData(float positionScale);

public:  // Sim Drawing parameters
   bool shouldDrawProjections()
   { return mDrawProjections; }

   gmtl::Vec3f getSurfaceColor()
   { return mSurfaceColor; }


protected: // Drawing functions used by library
   /**
    * Draws any objects that the manager needs to display
    * i.e. Gloves, etc.
    */
   void drawObjects();

   /** Draws projections in OpenGL. */
   void drawProjections(bool drawFrustum, gmtl::Vec3f surfColor, const float scaleFactor);

   /**
    * Draws a simulator using OpenGL commands.
    *
    * @note This is called internally by the library.
    */
   void drawSimulator(const float scaleFactor);

   /** Set the functor used to draw the wand */
   void setDrawWandFunctor(GlDrawObjectFunctor* functor)
   { mDrawWandFunctor = functor; }

protected:     // --- Geom helpers --- //
   void initQuadObj();
   void drawLine(gmtl::Vec3f& start, gmtl::Vec3f& end);
   void drawSphere(float radius, int slices, int stacks);
   void drawCone(float base, float height, int slices, int stacks);
   void drawBox(float size, GLenum type);
   void drawWireCube(float size);
   void drawSolidCube(float size);
   //void drawGlove(gadget::GloveProxy* gloveProxy);


protected:
   GlDrawObjectFunctor* mDrawWandFunctor;    /**< The functor to draw the wand */
   GLUquadricObj* mQuadObj;                  /**< Quadric for drawing stuff */

protected:
   SimViewport*   mSimViewport;

   // Drawing attributes
   bool           mDrawProjections;       /**< Should we draw projections */
   gmtl::Vec3f    mSurfaceColor;   /**< Color to draw surfaces */

   gadget::PositionInterface mCamera; /**< Proxy interfaces to devices needed */
   gadget::PositionInterface mWand;

   gmtl::Matrix44f    mCameraPos;    /**< The data about the position of all this stuff */
   gmtl::Matrix44f    mHeadPos;
   gmtl::Matrix44f    mWandPos;
};

}

#endif

