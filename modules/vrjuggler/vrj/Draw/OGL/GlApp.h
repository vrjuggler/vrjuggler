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

#ifndef _VRJ_GL_APP_H_
#define _VRJ_GL_APP_H_

#include <vrj/vrjConfig.h>

#include <vrj/Kernel/App.h>

#include <vrj/Draw/OGL/GlDrawManager.h>

//#include <vrj/Kernel/Kernel.h>
#include <vrj/Kernel/User.h>
#include <vrj/Display/Projection.h>

namespace vrj
{
   class Kernel;

/** GlApp: Encapulates an actual OpenGL application.
 *
 * PURPOSE:
 * This class defines the class that OpenGL
 * application classes should be derived from.  The interface
 * given is the interface that the System expects in order to
 * interface with the application.<br> <br>
 *
 * <h3> The control loop will look similar to this: </h3>
 *  NOTE: One time through the loop is a Juggler Frame <br>
 *
 * \code
 *  contextInit();                 // called for each context
 *  while (drawing)
 *  {
 *     preFrame();
 *     bufferPreDraw();            // called for each draw buffer
 *     contextPreDraw();           // called for each context
 *     draw();                     // called for each surfacewindow
 *     contextPostDraw();          // called for each context
 *     intraFrame();               // called in parallel to the draw functions.
 *     sync();
 *     postFrame();
 *
 *     UpdateTrackers();
 *  }
 *
 *  contextClose();                // called for each context
 * \endcode
 *
 * @see App
 */
class VJ_CLASS_API GlApp : public App
{
public:
   GlApp(Kernel* kern) : App(kern)
   {
      //api.setOpenGL();     // Tell everyone that we are OpenGL
   }

   GlApp() {;}

   virtual ~GlApp()
   {
      /* Do nothing. */ ;
   }

   /** Function to draw the scene.
    * Override this function with the user draw routine.
    * @pre OpenGL state has correct transformation and buffer selected.
    * @post The current scene has been drawn.
    */
   virtual void draw() = 0;

   /** Function that is called immediately after a new context is created.
    *  Use this function to create context specific data structures.
    *  i.e. Display lists, Texture objects, etc.
    * @pre The OpenGL context has been set to the new context.
    * @post Application has completed in initialization the user wishes.
    */
   virtual void contextInit()
   {;}

   /** Function that is called immediately before a context is closed.
    * Use the function to clean up any context data structures.
    */
   virtual void contextClose()
   {;}

   /** Function that is called upon entry into the context for a draw.
    * @pre The OpenGL context has been set to the context for drawing.
    * @post User application has executed any commands that need
    *   to only be executed once per context, per frame.
    * @note This function can be used for things that need to happen
    *       every frame, but only once per context.
    *  <br>
    * Ex: Dynamically Create display lists.
    */
   virtual void contextPreDraw()
   {;}

   /** Function that is called upon exit of the context for a draw.
    * @pre The OpenGL context has been set to the context for drawing.
    */
   virtual void contextPostDraw()
   {;}

   /** Function that is called once for each frame buffer of a gl context.
    * This function is executed after contextInit() (if needed) but before
    * contextPreDraw().  It is called once per framebuffer (see note).
    *
    * @pre The OpenGL context has been set to the context for drawing
    * @post User application has executed any commands that need
    *   to only be executed once per context, per buffer, per frame
    * @note This function is designed to be used when you want to do something
    *       only once per frame buffer (i.e. once for left buffer, once for
    *       right buffer)
    * <br>
    * Ex: glClear's need to be done in this method
    */
   virtual void bufferPreDraw()
   {;}

   /** Function that is called at the beginning of the drawing of each pipe.
    * @pre The library is preparing to render all windows on a given pipe.
    * @post Any pre-pipe user calls have been done.
    * @note Currently the OpenGL context is not set when this function is
    *       called.<br>
    *       This is a TEST function.  USE AT YOUR OWN RISK!!!
    */
   virtual void pipePreDraw()
   {;}

public:
   /** @name Factory functions
    * @{
    */

   /** Get the DrawManager to use.
   * Returns the OpenGL Draw Manager.
   */
   virtual DrawManager*    getDrawManager()
   { return GlDrawManager::instance(); }

    /** @} */
};

} // End of vrj namespace


#endif
