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

//-----------------------------------------------------------------------
//: OpenGL application class that has templatized user context data
//
//  This class allows the user to specify a data type that contains
// data that needs to have a dontext specific copy.  For example a struct
// full of display list id's.  The user passes their user-defined data
// structure as the template parameter.  This will then create a contextData()
// function that can be called by the application withing the drawing function
// in order to get the context specific data
//-----------------------------------------------------------------------
template<class ContextDataType = int>
class vjGlUserApp : public vjGlApp
{
public:
   vjGlUserApp(vjKernel* kern) : vjGlApp(kern)
   {
      ;
   }

   //: Returns reference to user data for the current context
   //! PRE: We are in a draw process
   //! NOTE: Should only be called from the draw function.
   //+       Results are un-defined for other functions.
   ContextDataType& contextData()
   {
         // Make sure that we will reference a valid element
      while(mContextDataVector.size() <= mCurContextId)
      {
         mContextDataVector.push_back(ContextDataType());
         cerr << "Adding ContextDataVector element: size now: " << mContextDataVector.size() << endl;
      }

      return mContextDataVector[mCurContextId];
   }
   
private:
   vector<ContextDataType>    mContextDataVector;     //: Vector of user data
};
 
#endif
