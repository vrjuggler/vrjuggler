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

#ifndef _VRJ_OPENGL_DRAW_AXES_FUNCTORS_H_
#define _VRJ_OPENGL_DRAW_AXES_FUNCTORS_H_

#include <vrj/Draw/OpenGL/Config.h>
#include <vrj/Draw/OpenGL/DrawManager.h>
#include <vrj/Draw/OpenGL/DrawObjectFunctor.h>


namespace vrj
{

namespace opengl
{

/** \class vrj::opengl::DrawAxesFunctor DrawAxesFunctors.h vrj/Draw/OpenGL/DrawAxesFunctors.h
 *
 * Draw basic coordinate system axes using colored lines
 *
 */ 
class DrawAxesFunctor : public vrj::opengl::DrawObjectFunctor
{
public:
   DrawAxesFunctor();

   virtual ~DrawAxesFunctor()
   {
      /* Do nothing. */ ;
   }

   /** Called to initialize any context-specific information. */
   virtual void contextInit();

   /**
    * Callback function for drawing.  Called when the object should be drawn.
    * @pre GL context is set and ready to go.
    */
   virtual void draw(vrj::UserPtr user);

   void setScaleFactor(float scaleFactor)
   {
      mScaleFactor = abs(scaleFactor);
   }

   float getScaleFactor()
   {
      return mScaleFactor;
   }

private:
   gmtl::Vec3f mOrigin, mX_Axis, mY_Axis, mZ_Axis;
   float mScaleFactor;
};

class DrawAxesCoreFunctor : public DrawAxesFunctor
{
public:
   DrawAxesCoreFunctor()
      : mProgram(0)
      , muMVMatrixHandle(0)
      , muPMatrixHandle(0)
      , maVertexCoordHandle(0)
      , maVertexColorHandle(0)
      , muScaleFactorHandle(0)
      , mVertexArrayBufferID(0)
      , mVertexCoordBufferID(0)
      , mVertexColorBufferID(0)
      , mIndexBufferID(0)
   {
      /* Do nothing. */ ;
   }

   virtual ~DrawAxesCoreFunctor()
   {
      /* Do nothing. */ ;
   }

   /** Called to initialize any context specific information. */
   virtual void contextInit();

   /**
    * Callback function for drawing.  Called when the object should be drawn.
    * @pre GL context is set and ready to go.
    */
   virtual void draw(vrj::UserPtr user);

private:
   unsigned int mProgram;
   int muMVMatrixHandle;
   int muPMatrixHandle;
   int maVertexCoordHandle;
   int maVertexColorHandle;
   int muScaleFactorHandle;
   unsigned int mVertexArrayBufferID;
   unsigned int mVertexCoordBufferID;
   unsigned int mVertexColorBufferID;
   unsigned int mIndexBufferID;

};

} // End of opengl namespace

} // End of vrj namespace


#endif /* _GL_DRAW_AXES_FUNCTORS_H_ */
