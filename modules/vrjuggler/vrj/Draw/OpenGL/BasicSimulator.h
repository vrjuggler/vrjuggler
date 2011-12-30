/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_OPENGL_BASIC_SIMULATOR_H_
#define _VRJ_OPENGL_BASIC_SIMULATOR_H_

#include <vrj/Draw/OpenGL/Config.h>

#include <vrj/Draw/OpenGL/SimInterface.h>
#include <vrj/Draw/OpenGL/DrawObjectFunctor.h>

#include <gadget/Type/PositionInterface.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>


namespace vrj
{

namespace opengl
{

/** \class vrj::opengl::BasicSimulator BasicSimulator.h vrj/Draw/OpenGL/BasicSimulator.h
 *
 * Interface for objects that wish to perform simulator function with an
 * OpenGL application.
 *
 * @note This class was renamed from vrj::GlBasicSimulator in VR Juggler
 *       2.3.11.
 */
class BasicSimulator : public SimInterface
{
public:
   BasicSimulator();

   virtual ~BasicSimulator();

   /**
    * Configures the basic OpenGL simulator.
    *
    * @pre element is a valid configuration element.
    * @post It should be configured.
    */
   virtual bool config(jccl::ConfigElementPtr element);

   static std::string getElementType()
   {
      return std::string("default_simulator");
   }

   /**
    * Tells the simulator draw functors to perform context-specific
    * negotiation.
    *
    * @since 2.1.18
    */
   virtual void contextInit();

   /**
    * Draws this sim device using the given information about the Window it
    * will be drawing into.
    */
   virtual void draw(const float scaleFactor);

   virtual void initialize(SimViewportPtr simVp)
   {
      mSimViewport = simVp;
   }

   /**
    * Sets the keyboard/mouse device the simulator can use to get input from
    * the user.
    */
   virtual void setKeyboardMouse(gadget::KeyboardMouseInterface kmInterface);

public:
   const gmtl::Matrix44f& getCameraPos() const
   {
      return mCameraPos;
   }

   const gmtl::Matrix44f& getHeadPos() const
   {
      return mHeadPos;
   }

   const gmtl::Matrix44f& getWandPos() const
   {
      return mWandPos;
   }

   virtual void updateProjectionData(const float positionScale,
                                     ProjectionPtr leftProj,
                                     ProjectionPtr rightProj);

   /**  Update internal simulator data */
   void updateInternalData(const float positionScale);

public:  // Sim Drawing parameters
   bool shouldDrawProjections() const
   {
      return mDrawProjections;
   }

   const gmtl::Vec3f& getSurfaceColor() const
   {
      return mSurfaceColor;
   }

protected: // Drawing functions used by library
   /**
    * Draws any objects that the manager needs to display
    * i.e. Gloves, etc.
    */
   void drawObjects();

   /** Draws projections in OpenGL. */
   void drawProjections(const bool drawFrustum, const gmtl::Vec3f& surfColor,
                        const float scaleFactor);

   /**
    * Tells the simulator draw functors to perform context-specific
    * negotiation.
    */
   void initSimulator();

   /**
    * Draws a simulator using OpenGL commands.
    *
    * @note This is called internally by the library.
    */
   void drawSimulator(const float scaleFactor);

   /** Sets the functor used to render the wand. */
   void setDrawWandFunctor(vrj::opengl::DrawObjectFunctor* functor)
   {
      mDrawWandFunctor = functor;
   }

   /** Sets the functor used to render the head. */
   void setDrawHeadFunctor(vrj::opengl::DrawObjectFunctor* functor)
   {
      mDrawHeadFunctor = functor;
   }

   /** Sets the functor used to render the axes. */
   void setDrawAxesFunctor(vrj::opengl::DrawAxesFunctor* functor)
   {
      mDrawAxesFunctor = functor;
   }

protected:     // --- Geom helpers --- //
   void initQuadObj();
   void drawLine(const gmtl::Vec3f& start, const gmtl::Vec3f& end);
   void drawBox(const float size, const GLenum type);
   void drawWireCube(const float size);
   void drawSolidCube(const float size);
   //void drawGlove(gadget::GloveProxy* gloveProxy);

   /** The functor to draw the wand. */
   vrj::opengl::DrawObjectFunctor* mDrawWandFunctor;

   /** The functor to draw the head. */
   vrj::opengl::DrawObjectFunctor* mDrawHeadFunctor;

   /** The functor to draw the axes. */
   vrj::opengl::DrawAxesFunctor* mDrawAxesFunctor;

   SimViewportPtr mSimViewport;

   // Drawing attributes
   bool           mDrawProjections;   /**< Should we draw projections */
   gmtl::Vec3f    mSurfaceColor;      /**< Color to draw surfaces */

   gadget::PositionInterface mCamera; /**< Proxy interfaces to devices needed */
   gadget::PositionInterface mWand;

   gmtl::Matrix44f    mCameraPos;     /**< The data about the position of all this stuff */
   gmtl::Matrix44f    mHeadPos;
   gmtl::Matrix44f    mWandPos;
};

}

}


#endif