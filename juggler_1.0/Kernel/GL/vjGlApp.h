#ifndef _VJ_GL_APP_
#define _VJ_GL_APP_

#include <config.h>
#include <Kernel/vjApp.h>

#include <Kernel/vjKernel.h>
#include <vector.h>

//-----------------------------------------------------------
//: vjGlApp: Encapulates an actual OpenGL application.
//
// PURPOSE:
//	This class defines the class that OpenGL
//  application classes should be derived from.  The interface
//  given is the interface that the System expects in order to
//  interface with the application.
//
// @author Allen Bierbaum
//  Date: 1-12-98
//-------------------------------------------------------------
class vjGlApp : public vjApp
{
public:
   vjGlApp(vjKernel* kern) : vjApp(kern)
   {
      api.setOpenGL();     // Tell everyone that we are OpenGL
      mCurContextId = 0;   // Initialize the context value
   }

   //: Function to draw the scene
   //!PRE: OpenGL state has correct transformation and buffer selected
   //!POST: The current scene has been drawn
   virtual void draw() = 0;

   //: Function that is called at the beginning of the drawing of each pipe (ogl context)
   //!PRE: The library is preparing to render all windows on a given pipe
   //!POST: Any pre-pipe user calls have been done
   virtual void pipePreDraw()
   {;}

   //: Function that is called immedately after a new context is created
   //! PRE: The ogl context has been set to the new context
   //! POST: Application has completed in initialization the user wishes
   virtual void contextInit()
   {;}

   //: Function to set a context id
   //! NOTE: Used internally only !!!
   //+       Users should not call !!!
   void setContextId(int contextId)
   { mCurContextId = contextId; }

protected:
   int                        mCurContextId;          // Internal Unique id of the current openGL context
};


#endif
