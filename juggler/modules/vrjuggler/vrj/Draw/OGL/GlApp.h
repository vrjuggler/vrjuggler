/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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


#ifndef _VJ_GL_APP_
#define _VJ_GL_APP_

#include <vjConfig.h>

#include <Kernel/vjApp.h>

#include <Kernel/GL/vjGlDrawManager.h>

//#include <Kernel/vjKernel.h>
#include <Kernel/vjUser.h>
#include <Kernel/vjProjection.h>

namespace vrj
{
   class Kernel;

//-----------------------------------------------------------
//: GlApp: Encapulates an actual OpenGL application.
//
// PURPOSE:
// This class defines the class that OpenGL
//  application classes should be derived from.  The interface
//  given is the interface that the System expects in order to
//  interface with the application.
//
// See also: App
//
// @author Allen Bierbaum
//  Date: 1-12-98
//-------------------------------------------------------------
//!PUBLIC_API:
class VJ_CLASS_API GlApp : public App
{
public:
   GlApp(Kernel* kern) : App(kern)
   {
      //api.setOpenGL();     // Tell everyone that we are OpenGL
   }

   GlApp() {;}

   //: Function to draw the scene
   // Override this function with the user draw routine
   //!PRE: OpenGL state has correct transformation and buffer selected
   //!POST: The current scene has been drawn
   virtual void draw() = 0;

   //: Function that is called immediately after a new context is created
   //  Use this function to create context specific data structures.
   //  i.e. Display lists, Texture objects, etc.
   //! PRE: The ogl context has been set to the new context
   //! POST: Application has completed in initialization the user wishes
   virtual void contextInit()
   {;}

   //: Function that is called immediately before a context is closed
   // Use the function to clean up any context data structures
   virtual void contextClose()
   {;}

   //: Function that is called upon entry into the context for a draw
   //! PRE: The ogl context has been set to the context for drawing
   //! POST: User application has executed any commands that need
   //+   to only be executed once per context, per frame
   //! NOTE: This function can be used for things that need to happen
   //+       every frame, but only once per context
   //+  <br> Ex: Dynamically Create display lists
   virtual void contextPreDraw()
   {;}

   //: Function that is called upon entry into a buffer of a gl context
   //! PRE: The ogl context has been set to the context for drawing
   //! POST: User application has executed any commands that need
   //+   to only be executed once per context, per buffer, per frame
   //! NOTE: This function is designed to be used when you want to do something
   //+       only once per buffer (ie.once for left buffer, once for right buffer)
   //+  <br> Ex: glClear's need to be done in this method
   virtual void bufferPreDraw()
   {;}

   //: Function that is called at the beginning of the drawing of each pipe
   //!PRE: The library is preparing to render all windows on a given pipe
   //!POST: Any pre-pipe user calls have been done
   //!NOTE: Currently the OGL context is not set when this function is called <br>
   //+      This is a TEST function.  USE AT YOUR OWN RISK!!!
   virtual void pipePreDraw()
   {;}

public:  // --- Factory functions --- //
   //: Get the DrawManager to use
   // Returns the ogl draw manager
   virtual DrawManager*    getDrawManager()
   { return GlDrawManager::instance(); }
};


};

#endif
