/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_GL_APP_
#define _VJ_GL_APP_

#include <vjConfig.h>

#include <Kernel/vjApp.h>

#include <Kernel/GL/vjGlDrawManager.h>

#include <Kernel/vjKernel.h>
#include <Kernel/vjUser.h>
#include <Kernel/vjProjection.h>


//-----------------------------------------------------------
//: vjGlApp: Encapulates an actual OpenGL application.
//
// PURPOSE:
//	This class defines the class that OpenGL
//  application classes should be derived from.  The interface
//  given is the interface that the System expects in order to
//  interface with the application.
//
// See also: vjApp
//
// @author Allen Bierbaum
//  Date: 1-12-98
//-------------------------------------------------------------
//!PUBLIC_API:
class vjGlApp : public vjApp
{
public:
   vjGlApp(vjKernel* kern) : vjApp(kern)
   {
      //api.setOpenGL();     // Tell everyone that we are OpenGL
   }

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
   //+       every frame, but only once per context.
   //+  <br> Ex: Dynamically Create display lists
   virtual void contextPreDraw()
   {;}

   //: Function that is called at the beginning of the drawing of each pipe
   //!PRE: The library is preparing to render all windows on a given pipe
   //!POST: Any pre-pipe user calls have been done
   //!NOTE: Currently the OGL context is not set when this function is called <br>
   //+      This is a TEST function.  DO NOT USE!!!
   virtual void pipePreDraw()
   {;}

public:  // --- Factory functions --- //
   //: Get the DrawManager to use
   // Returns the ogl draw manager
   virtual vjDrawManager*    getDrawManager()
   { return vjGlDrawManager::instance(); }
};


#endif
