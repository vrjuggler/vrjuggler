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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_OPENGL_DRAW_HEAD_FUNCTORS_H_
#define _VRJ_OPENGL_DRAW_HEAD_FUNCTORS_H_

#include <vrj/Draw/OpenGL/Config.h>

#ifdef VPR_OS_Darwin
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

#include <vrj/Draw/OpenGL/DrawObjectFunctor.h>


namespace vrj
{

namespace opengl
{

/** \class vrj::opengl::DrawEllipsoidHeadFunctor DrawHeadFunctors.h vrj/Draw/OpenGL/DrawHeadFunctors.h
 *
 * Draw a basic ellipsoid head.
 *
 * @note This class was renamed from vrj::GlDrawEllipsoidHeadFunctor in VR
 *       Juggler 2.3.11.
 */ 
class DrawEllipsoidHeadFunctor : public vrj::opengl::DrawObjectFunctor
{
public:
   DrawEllipsoidHeadFunctor();

   virtual ~DrawEllipsoidHeadFunctor();

   /** Called to initialize any context-specific information. */
   virtual void contextInit();

   /**
    * Callback function for drawing.  Called when the object should be drawn.
    * @pre GL context is set and ready to go.
    */
   virtual void draw(vrj::UserPtr user);

protected:
   void drawSphere(const float radius, const int slices, const int stacks);

   GLUquadricObj* mQuadObj;
};

} // End of opengl namespace

} // End of vrj namespace


#endif /* _GL_DRAW_HEAD_FUNCTORS_H_ */
