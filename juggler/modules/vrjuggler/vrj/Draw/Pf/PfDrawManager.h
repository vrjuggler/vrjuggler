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

#ifndef _VRJ_PF_DRAW_MANAGER_
#define _VRJ_PF_DRAW_MANAGER_

#include <vrj/vrjConfig.h>

#include <vector>
#include <functional>
#include <algorithm>

#include <Performer/pf/pfChannel.h>

#include <vrj/Draw/DrawManager.h>

#include <vrj/Draw/Pf/PfUtil.h>
#include <vpr/Util/Singleton.h>

#include <boost/concept_check.hpp>


namespace vrj
{

class Display;
class PfApp;
class Projection;
class SimViewport;
class Viewport;

    // Performer Config function called in draw proc after window is set up
void PFconfigPWin(pfPipeWindow* pWin);
void PfDrawFunc(pfChannel *chan, void* chandata,bool left_eye, bool right_eye, bool stereo, bool simulator);
void PfAppFunc(pfChannel *chan, void* chandata);

/** Concrete singleton class for API specific Draw Manager.
*
* Responsible for all Performer rendering and windowing
*/
class VJ_CLASS_API PfDrawManager : public DrawManager
{
protected:

   /** The channels associated with a viewport */
   struct pfViewport
   {
      // Identifier for the channels
      // ie. PRIMARY is the primary channel, etc
      enum {PRIMARY = 0, SECONDARY = 1};

      pfViewport()
      {
         chans[0] = chans[1] = NULL;
         viewport = NULL;
      }

      pfChannel*  chans[2];      /**< The channels */
      Viewport* viewport;        /**< The viewport that we are rendering */
   };

   struct pfDisplay
   {
      pfDisplay()
      {
         disp = NULL;
         pWin = NULL;
      }

      Display*              disp;
      pfPipeWindow*           pWin;
      std::vector<pfViewport> viewports;
   };

   struct pfDisplay_disp : public std::unary_function<pfDisplay,Display*>
   {
     Display* operator()(const pfDisplay disp) const { return disp.disp; }
   };

   /*
   struct findPfDispChan : unary_function<pfDisp, bool>
   {
      findPfDispChan(pfChannel* chan) : mChan(chan) {;}
      bool operator()(pfDisp disp) {
         return ((disp.chans[0] == mChan) || (disp.chans[1] == mChan));
      }
      pfChannel* mChan;                // Channel to find
   };
   */

public:
    /** Blocks until the end of the frame
    * @pre none
    * @post The frame has been drawn
    */
   virtual void sync();

   /** Enable a frame to be drawn
   * @pre none
   * @post Frame has been triggered to begin drawing
   */
   virtual void draw();

   /**
    * Returns the Simulator SceneGraph so that PfBasicSimulator can
    * add/update the Head and Wand
    */
   pfScene* getRootWithSim()
   { return mRootWithSim; }

   /** Set the app the draw whould interact with.
   * @pre none
   * @post self'.app = _app
   */
   virtual void setApp(App* _app);

   /** Initialize the drawing API (if not already running)
   *  should call pfInit()
   *
   * Configure process model
   * Configure multi-pipe model
   * Then call pfConfig to start the MP stuff
   * Sets up channels and pWins.
   *
   * @note Fork happens here
   */
   virtual void initAPI();

   /** Initialize the parameters of the master channel */
   void initChanGroupAttribs(pfChannel* masterChan);

   /** Callback when display is added to display manager */
   virtual void addDisplay(Display* disp);

   /** Callback when display is removed to display manager */
   virtual void removeDisplay(Display* disp);

   /** Shutdown the drawing API */
   virtual void closeAPI();

   /** Update all the projections for the displays
   * @post All windows have the projections correctly set.
   */
   virtual void updatePfProjections();

   /** dumps the object's internal state */
   void debugDump(int debugLevel);

   void debugDumpPfDisp(pfDisplay* pf_disp, int debugLevel);

   friend void PFconfigPWin(pfPipeWindow* pWin);
   friend void PfDrawFunc(pfChannel *chan, void* chandata,bool left_eye, bool right_eye, bool stereo);
   friend void PfAppFunc(pfChannel *chan, void* chandata);

public: // Chunk handlers
   /** Can the handler handle the given chunk?
   * @returns true - Can handle it
   *          false - Can't handle it
   */
   virtual bool configCanHandle(jccl::ConfigChunkPtr chunk)
   {
      boost::ignore_unused_variable_warning(chunk);
      return false;
   }

protected:     // --- Config handling functions --- //
   /** Add the chunk to the configuration
   * @pre configCanHandle(chunk) == true
   * @returns success
   */
   virtual bool configAdd(jccl::ConfigChunkPtr chunk)
   {
      boost::ignore_unused_variable_warning(chunk);
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "vjPfDrawManager::configAdd: configAdd is not supported.\n" << vprDEBUG_FLUSH;
      return false;
   }

   /** Remove the chunk from the current configuration
   * @pre  configCanHandle(chunk) == true
   * @returns success
   */
   virtual bool configRemove(jccl::ConfigChunkPtr chunk)
   {
      boost::ignore_unused_variable_warning(chunk);
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "vjPfDrawManager::configRemove: configRemove is not supported.\n" << vprDEBUG_FLUSH;
      return false;
   }

   /** Setup display system related attributes
   * @pre chunk is a chunks of the "dispaySystem" type
   * @note  MUST be called before initDrawing
   * @note This must be called by the draw manager
   *        because the chunk must be gotten from the draw manager
   */
   bool configDisplaySystem(jccl::ConfigChunkPtr chunk);

protected:
   /** Call all the application channel callbacks */
   void callAppChanFuncs();

   /** Helper to set channel view params from a Projection */
   void updatePfProjection(pfChannel* chan, Projection* proj); //, bool simulator=false);

   /** Helper function to create the base scene graph stuff */
   void initPerformerGraph();

   /** Helper that (re)loads the application's scene graph into the active scene(s) */
   void initAppGraph();

   /**
    * Helper to get the pfDisp given a channel.
    *
    * @return NULL to indicate that the display was not found.
    */
   //**//pfDisp* getPfDisp(pfChannel* chan);

   /** Init all the pipes that may need to be used */
   void initPipes();

   /** Get a performer pipe
   * @pre pipe_num < mNumPipes
   *       Fork must have happend
   */
   pfPipe* getPfPipe(unsigned pipe_num);

   /** @name Display helpers
   * Helpers for releasing a display and the associated cruft
   */
   //@{
   void releaseDisplay(pfDisplay& disp);
   void releaseViewport(pfDisplay& disp, pfViewport& vp);
   pfPipeWindow* allocatePipeWin(unsigned pipeNum);
   void releasePipeWin(pfPipeWindow* pipeWin, unsigned pipeNum);
   //@}

   /** Return the needed mono frame buffer config. */
   std::vector<int> getMonoFBConfig(vrj::Display* disp);
   /** Return the needed stereo frame buffer config. */
   std::vector<int> getStereoFBConfig(vrj::Display* disp);

   /**
    * Adds frame buffer configuration options to the given vector based on
    * the given vrj::Display object's OpenGL frame buffer configuration.
    */
   void configFrameBuffer(vrj::Display* disp, std::vector<int>& attrs);

protected:
   // NOTE:  ---- REMEMBER THAT PF HAS SHARED MEM Model ---
   // Rember that Performer uses forks, so it's processes need to
   // have shared memory allocated variable in order to work
   // correctly.

   // --- Config Data --- //
   unsigned int mNumPipes;    /**< The number of Performer pipes */

   // --- Performer State --- //
   PfApp*                  mApp;              /**< The User applications */
   std::vector<pfDisplay>  mDisplays;         /**< All Performer displays */

   std::vector<pfChannel*> mSurfChannels;
   std::vector<pfChannel*> mSimChannels;        /**< List of sim displays */
   pfChannel*              mSurfMasterChan;  /**< Master channel */
   pfChannel*              mSimMasterChan;   /**< Master channel for simulators */

   std::vector<pfPipe*>    mPipes;            /**< Performer pipes we have opened */
   std::vector<std::string> mPipeStrs;        /**< The X-Strs of the pipes */
   bool                    mPfHasForked;     /**< Performer has forked it processes already */

   /** List of (available) pipe windows grouped by pipe num
   * This is used as a repository of previously allocated but unused pipe windows
   * We do this because Performer does not allow the deletion of pfPipeWindows
   */
   std::vector< std::vector<pfPipeWindow*> > mPipeWindows;

   /** @name Scene graph nodes
   * <pre>
   *       mRoot
   *        |
   *     mSceneGroup -- mSceneRoot -- app scene
   *       /
   * mRootWithSim   mHeadDCS
   *       \       /
   *       mSimTree
   *              |
   *              mWandDCS
   * </pre>
   */
   //@{
   pfScene*          mRoot;            /**< Root of performer tree to render */
   pfGroup*          mSceneRoot;       /**< Root of scene to render (changes at run-time) */
   pfGroup*          mSceneGroup;      /**< The group node with only sceneRoot under it */
   pfScene*          mRootWithSim;     /**< The root with the simulator group & the sceneRoot */
   //@}

protected:

   PfDrawManager() :
      mApp(NULL),
      mSurfMasterChan(NULL),
      mSimMasterChan(NULL),
      mPfHasForked(false),
      mRoot(NULL),
      mSceneRoot(NULL),
      mSceneGroup(NULL),
      mRootWithSim(NULL)
   {;}

   virtual ~PfDrawManager() {}

   vprSingletonHeader(PfDrawManager);
};


};

#endif
