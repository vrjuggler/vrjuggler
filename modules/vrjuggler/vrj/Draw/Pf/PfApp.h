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

#ifndef _VRJ_PF_APP_H_
#define _VRJ_PF_APP_H_

#include <vrj/vjConfig.h>
#include <Performer/pf/pfGroup.h>
#include <Performer/pf/pfChannel.h>

#include <vrj/Kernel/App.h>
#include <vrj/Draw/DrawManager.h>
//#include <vrj/Kernel/vjKernel.h>
#include <vrj/Draw/Pf/PfDrawManager.h>
#include <vrj/Draw/Pf/PfUtil.h>

namespace vrj
{
   class Kernel;
   
//-------------------------------------------------------------
//: Encapulates an actual Performer application.
//
// This class defines the class that Performer
//  application classes should be derived from.  The interface
//  given is the interface that the System expects in order to
//  interface with the application.
//
// @author Allen Bierbaum
//  Date: 9-8-97
//-------------------------------------------------------------
//!PUBLIC_API:
class PfApp : public App
{
public:
   PfApp(Kernel* kern) : App(kern)
   {
      //api.setPerformer();  // Tell everyone that we are Performer
   }

   PfApp() {;}

   //: Initialize the scene graph
   // Called after pfInit & pfConfig but before apiInit
   virtual void initScene() = 0;

   //: Called between pfInit and pfConfig
   // This function allows the user application to do any processing that needs
   // to happen before performer forks its processes off but after
   // pfInit()
   //! NOTE: Using this function leads to applications that are
   //+ not "switchable"
   virtual void preForkInit() {;}

   //: Function called in application process for each active channel each frame
   // Called immediately before draw (pfFrame())
   // XXX: Should maybe only call this for one "master" channel each frame
   virtual void appChanFunc(pfChannel* chan) {;}

   //: Return the current scene graph
   // This function must be defined so that the performer draw manager
   // can get a scene graph to draw.
   // NOTE: Do NOT put model loading and manipulation in this function
   //       instead, but that type of think in initScene()
   // NOTE: Make sure that the node returned is NOT a pfScene.
   //+      If it is, then lighting will NOT work.
   virtual pfGroup* getScene() = 0;

   //: Init a pWin
   // Called by the pf draw manager as soon as the pwin
   // is opened.
   virtual void configPWin(pfPipeWindow* pWin)
   {;}

   //: Return the needed parameters for the performer framebuffer
   //! NOTE: Stereo, doublebuffer, depth buffer, and rgba are all requested by default
   virtual std::vector<int> getFrameBufferAttrs()
   { return std::vector<int>();}

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
      vjDEBUG_BEGIN(vjDBG_DRAW_MGR, 4) << "--- drawChan: Entered ---.\n" << vjDEBUG_FLUSH;
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

public: // -- Factory Function -- //
   //: Get the DrawManager to use
   // Returns the ogl draw manager
   virtual DrawManager* getDrawManager()
   { return PfDrawManager::instance(); }

};

};

#endif
