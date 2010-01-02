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

#ifndef _VRJ_OPENGL_DRAW_OBJECT_FUNCTOR_H_
#define _VRJ_OPENGL_DRAW_OBJECT_FUNCTOR_H_

#include <vrj/Draw/OpenGL/Config.h>

#include <vrj/Kernel/UserPtr.h>


namespace vrj
{

namespace opengl
{

/** \class vrj::opengl::DrawObjectFunctor DrawObjectFunctor.h vrj/Draw/OpenGL/DrawObjectFunctor.h
 *
 * Base interface for an object drawing functor.
 * 
 * Any OpenGL draw objects should be derived from this class.
 * This is used for drawing things such as wands and gloves.
 *
 * @note This class was renamed from vrj::GlDrawObjectFunctor in VR Juggler
 *       2.3.11.
 */
class DrawObjectFunctor
{
public:   
   /** Called to initialize any context specific information. */
   virtual void contextInit() = 0;
   
   /**
    * Callback function for drawing.
    * Called when the object should be drawn.
    * @pre OpenGL context is set and ready to go.
    */
   virtual void draw(vrj::UserPtr user) = 0;

   virtual ~DrawObjectFunctor()
   {
      /* Do nothing. */ ;
   }
};

/** \class vrj::opengl::DrawGloveObjectFunctor DrawObjectFunctor.h vrj/Draw/OpenGL/DrawObjectFunctor.h
 *
 * Base class for glove drawing.  Adds a function for setting glove
 * parameters.
 *
 * @note This class was renamed from vrj::GlDrawGloveObjectFunctor in VR
 *       Juggler 2.3.11.
 */
class DrawGloveObjectFunctor : public vrj::opengl::DrawObjectFunctor
{
public:
   virtual void setGlove() = 0;

   virtual ~DrawGloveObjectFunctor()
   {
      /* Do nothing. */ ;
   }
};

}

}


#endif
