/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_PF_APP_H_
#define _VRJ_PF_APP_H_

#include <vrj/Draw/Pf/Config.h>
#include <vector>
#include <boost/concept_check.hpp>
#include <Performer/pf/pfGroup.h>
#include <Performer/pf/pfChannel.h>

#include <vrj/Kernel/App.h>
#include <vrj/Util/Debug.h>


namespace vrj
{

class Kernel;
class DrawManager;

namespace pf
{

/** \class vrj::pf::App App.h vrj/Draw/Pf/App.h
 *
 * Encapulates an actual Performer application.
 *
 * This class defines the class that Performer application classes should be
 * derived from.  The interface defined by this class is what the Performer
 * Draw Manager will use to interact with the user's application.
 *
 * @note This class was renamed from vrj::PfApp in VR Juggler 2.3.11.
 */
class VJ_PF_API App
   : public vrj::App
{
public:
   App(Kernel* kern = NULL);

   virtual ~App();

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
      vprDEBUG_OutputGuard(
         vrjDBG_DRAW_MGR, vprDBG_VERB_LVL,
         std::string("--- vrj::pf::App::drawChan() Entered ---\n"),
         std::string("--- vrj::pf::App::drawChan() Exited ---\n")
      );
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

   /**
    * Gets the Draw Manager used by this application object. Returns the
    * Performer Draw Manager.
    */
   virtual vrj::DrawManager* getDrawManager();
};

}

}


#endif
