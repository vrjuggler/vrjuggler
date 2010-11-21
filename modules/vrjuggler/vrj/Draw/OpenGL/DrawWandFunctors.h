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

#ifndef _VRJ_OPENGL_DRAW_WAND_FUNCTORS_H_
#define _VRJ_OPENGL_DRAW_WAND_FUNCTORS_H_

#include <vpr/vprConfig.h>
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

/** \class vrj::opengl::DrawConeWandFunctor DrawWandFunctors.h vrj/Draw/OpenGL/DrawWandFunctors.h
 *
 * Draw a basic cone wand.
 *
 * @note This class was renamed from vrj::GlDrawConeWandFunctor in VR Juggler
 *       2.3.11.
 */
class DrawConeWandFunctor : public vrj::opengl::DrawObjectFunctor
{
public:
   DrawConeWandFunctor();

   virtual ~DrawConeWandFunctor();

   /** Called to initialize any context specific information. */
   virtual void contextInit()
   {;}

   /**
    * Callback function for drawing.  Called when the object should be drawn.
    * @pre GL context is set and ready to go.
    */
   virtual void draw(vrj::UserPtr user);

protected:
   GLUquadricObj* mQuadObj;
};


/** \class vrj::opengl::DrawRightAngleWandFunctor DrawWandFunctors.h vrj/Draw/OpenGL/DrawWandFunctors.h
 *
 * Draw a wand with a right angle.
 *
 * @note This class was renamed from vrj::GlDrawRightAngleWandFunctor in VR
 *       Juggler 2.3.11.
 */
class DrawRightAngleWandFunctor : public vrj::opengl::DrawObjectFunctor
{
public:
   virtual ~DrawRightAngleWandFunctor()
   {
      /* Do nothing. */ ;
   }

   /** Called to initialize any context specific information. */
   virtual void contextInit()
   {;}

   /**
    * Callback function for drawing.  Called when the object should be drawn.
    * @pre GL context is set and ready to go.
    */
   virtual void draw(vrj::UserPtr user);
};

} // End of opengl namespace

} // End of vrj namespace


#endif
