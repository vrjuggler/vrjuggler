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


#ifndef _VJ_PF_APP_H_
#define _VJ_PF_APP_H_

#include <vjConfig.h>
#include <Performer/pf/pfScene.h>
#include <Kernel/vjApp.h>
//#include <Kernel/vjKernel.h>
class vjKernel;
#include <Kernel/Pf/vjPfDrawManager.h>
#include <Kernel/Pf/vjPfUtil.h>

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
//!PUBLIC_API:
class vjPfApp : public vjApp
{
public:
   vjPfApp(vjKernel* kern) : vjApp(kern)
   {
      //api.setPerformer();  // Tell everyone that we are Performer
   }

   //: Initialize the scene graph
   virtual void initScene() = 0;

   //: Return the current scene graph
   // This function must be defined so that the performer draw manager
   // can get a scene graph to draw.
   // NOTE: Make sure that the node returned is NOT a pfScene.
   //+      If it is, then lighting will NOT work.
   virtual pfGroup* getScene() = 0;

   //: Init a pWin
   virtual void configPWin(pfPipeWindow* pWin)
   {
     ;
   }

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
      this->preDrawChan(chan,chandata);
      chan->clear();       // Clear the channel
      pfDraw();            // Draw the channel
      this->postDrawChan(chan,chandata);
   }

   // Function called by the DEFAULT drawChan function before clearing the channel
   // and drawing the next frame (pfFrame())
   virtual void preDrawChan(pfChannel* chan, void* chandata){;}

   // Function called by the DEFAULT drawChan function after clearing the channel
   // and drawing the next frame (pfFrame())
   virtual void postDrawChan(pfChannel* chan, void* chandata){;}


public:  // --- Factory functions --- //
   //: Get the DrawManager to use
   // Returns the ogl draw manager
   virtual vjDrawManager*    getDrawManager()
   { return vjPfDrawManager::instance(); }
};

#endif
