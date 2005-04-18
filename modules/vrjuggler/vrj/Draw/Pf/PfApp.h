/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <vrj/vrjConfig.h>
#include <vector>
#include <boost/concept_check.hpp>
#include <Performer/pf/pfGroup.h>
#include <Performer/pf/pfChannel.h>

#include <vrj/Kernel/App.h>
#include <vrj/Draw/Pf/PfUtil.h>
#include <vrj/Util/Debug.h>


namespace vrj
{
   class Kernel;
   class DrawManager;

/** \class PfApp PfApp.h vrj/Draw/Pf/PfApp.h
 *
 * Encapulates an actual Performer application.
 *
 * This class defines the class that Performer application classes should be
 * derived from.  The interface defined by this class is what the Performer
 * Draw Manager will use to interact with the user's application.
 */
class VJ_CLASS_API PfApp : public App
{
public:
   PfApp(Kernel* kern=NULL);

   virtual ~PfApp();

   /**
    * Initializes the scene graph.
    * Called after pfInit and pfConfig but before apiInit.
    */
   virtual void initScene() = 0;

   /**
    * Called between pfInit and pfConfig.
    * This function allows the user application to do any processing that needs
    * to happen before performer forks its processes off but after
    * pfInit().
    * @note Using this function leads to applications that are not "switchable".
    */
   virtual void preForkInit() {;}

   /**
    * Function called in application process for each active channel each frame.
    * Called immediately before draw (pfFrame()).
    * XXX: Should maybe only call this for one "master" channel each frame.
    */
   virtual void appChanFunc(pfChannel* chan)
   {
      boost::ignore_unused_variable_warning(chan);
   }

   /**
    * Returns the current scene graph.
    * This function must be defined so that the Performer Draw Manager
    * can get a scene graph to draw.
    * @note Do NOT put model loading and manipulation in this function
    *       instead, put that type of thing in initScene().
    * @note Make sure that the node returned is NOT a pfScene.
    *       If it is, then lighting will NOT work.
    */
   virtual pfGroup* getScene() = 0;

   /**
    * Initializes a pWin.
    * Called by the Performer Draw Manager as soon as the pwin
    * is opened.
    */
   virtual void configPWin(pfPipeWindow* pWin)
   {
      boost::ignore_unused_variable_warning(pWin);
   }

   /**
    * Returns the needed parameters for the performer framebuffer.
    * @note Stereo, doublebuffer, depth buffer, and rgba are all requested by
    *       default.
    * ex: To request multisampling return a vector containing: [PFB_SAMPLE_BUFFER,1,PFFB_SAMPLES,1]
    */
   virtual std::vector<int> getFrameBufferAttrs()
   {
      return std::vector<int>();
   }

   /**
    * Function called in the channel draw function to do the actual drawing.
    *
    * For most programs the default behavior of this function is correct.
    * It simply does the following: <br>
    *    chan->clear(); <br>
    *    pfDraw();      <br>
    *
    * Advanced users may want to overide this behavior for advanced rendering
    * effects such as over-lays, or multi-pass rendering.
    * See the Performer man pages about overiding the draw traversal function.
    * This function is the draw traversal function but with the projections
    * set correctly for the given display and eye.
    *
    * @pre chan is ready to draw.
    * @post Channel should be drawn.
    */
   virtual void drawChan(pfChannel* chan, void* chandata)
   {
      vprDEBUG_OutputGuard(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL,
                           std::string("--- vrj::PfApp::drawChan() Entered ---\n"),
                           std::string("--- vrj::PfApp::drawChan() Exited ---\n"));
      this->preDrawChan(chan,chandata);
      chan->clear();       // Clear the channel
      pfDraw();            // Draw the channel
      this->postDrawChan(chan,chandata);
   }

   /**
    * Function called by the DEFAULT drawChan function before clearing the
    * channel and drawing the next frame (pfFrame()).
    */
   virtual void preDrawChan(pfChannel* chan, void* chandata)
   {
      boost::ignore_unused_variable_warning(chan);
      boost::ignore_unused_variable_warning(chandata);
   }

   /**
    * Function called by the DEFAULT drawChan function after clearing the
    * channel and drawing the next frame (pfFrame()).
    */
   virtual void postDrawChan(pfChannel* chan, void* chandata)
   {
      boost::ignore_unused_variable_warning(chan);
      boost::ignore_unused_variable_warning(chandata);
   }

public: // -- Factory Function -- //
   /**
    * Gets the Draw Manager to use. Returns the Performer Draw Manager.
    */
   virtual DrawManager* getDrawManager();
};

}

#endif
