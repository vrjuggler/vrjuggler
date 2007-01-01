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

#ifndef GL_DRAW_OBJECT_FUNCTOR_H
#define GL_DRAW_OBJECT_FUNCTOR_H

#include <vrj/Draw/OGL/Config.h>


namespace vrj
{

class User;

/** \class GlDrawObjectFunctor GlDrawObjectFunctor.h vrj/Draw/OGL/GlDrawObjectFunctor.h
 *
 * Base interface for an object drawing functor.
 * 
 * Any OpenGL draw objects should be derived from this class.
 * This is used for drawing things such as wands and gloves.
 */
class GlDrawObjectFunctor
{
public:   
   /** Called to initialize any context specific information. */
   virtual void contextInit() = 0;
   
   /**
    * Callback function for drawing.
    * Called when the object should be drawn.
    * @pre OpenGL context is set and ready to go.
    */
   virtual void draw(vrj::User* user) = 0;

   virtual ~GlDrawObjectFunctor()
   {
      /* Do nothing. */ ;
   }
};

/** \class GlDrawGloveObjectFunctor GlDrawObjectFunctor.h vrj/Draw/OGL/GlDrawObjectFunctor.h
 *
 * Base class for glove drawing.  Adds a function for setting glove parameters.
 */
class GlDrawGloveObjectFunctor : public GlDrawObjectFunctor
{
public:
   virtual void setGlove() = 0;

   virtual ~GlDrawGloveObjectFunctor()
   {
      /* Do nothing. */ ;
   }
};

}

#endif
