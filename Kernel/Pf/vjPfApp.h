#ifndef _VJ_PFAPP_
#define _VJ_PFAPP_

#include <config.h>
#include <Performer/pf/pfScene.h>
#include <Kernel/vjApp.h>
//#include <Kernel/vjKernel.h>
class vjKernel;
#include <Kernel/Pf/vjPfDrawManager.h>

//-------------------------------------------------------------
//: Encapulates an actual Performer application.
//
//	This class defines the class that Performer
//  application classes should be derived from.  The interface
//  given is the interface that the System expects in order to
//  interface with the application.
//
// @author Allen Bierbaum
//  Date: 9-8-97
//-------------------------------------------------------------
class vjPfApp : public vjApp
{
public:
   vjPfApp(vjKernel* kern) : vjApp(kern)
   {
      api.setPerformer();  // Tell everyone that we are Performer
   }

   //: Initialize the scene graph
   virtual void initScene() = 0;

   //: Return the current scene graph
   // This function must be defined so that the performer draw manager
   // can get a scene graph to draw.
   virtual pfScene* getScene() = 0;

   //: Init a pWin
   virtual void initPWin(pfPipeWindow* pWin)
   {;}

   //: Function called in the channel draw function to do the actual drawing
   //
   // For most programs the default behavior of this function is correct.
   // It simply does the following: <br>
   //    chan->clear(); <br>
   //    pfDraw();      <br>
   //
   // Advanced users may want to overide this behavior for advanced rendering effects
   // such as over-lays, or multi-pass rendering.
   // See the Performer man pages about overiding the draw traversal function.
   // This function is the draw traversal function but with the projections
   // set correctly for the given display and eye
   //
   //! PRE: chan is ready to draw
   //! POST: Channel should be drawn
   virtual void drawChan(pfChannel* chan, void* chandata)
   {
      chan->clear();       // Clear the channel
      pfDraw();            // Draw the channel
   }

public:  // --- Factory functions --- //
   //: Get the DrawManager to use
   // Returns the ogl draw manager
   virtual vjDrawManager*    getDrawManager()
   { return vjPfDrawManager::instance(); }
};

#endif
