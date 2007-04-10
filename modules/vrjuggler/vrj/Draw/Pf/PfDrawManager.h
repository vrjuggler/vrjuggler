/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_PF_DRAW_MANAGER_
#define _VRJ_PF_DRAW_MANAGER_

#include <vrj/Draw/Pf/Config.h>

#include <vector>
#include <functional>
#include <algorithm>
#include <boost/concept_check.hpp>

#include <Performer/pf/pfChannel.h>

#include <vpr/Util/Singleton.h>
#include <vrj/Draw/DrawManager.h>
#include <vrj/Draw/Pf/PfUtil.h>
#include <vrj/Util/Debug.h>

#ifdef VPR_OS_Windows
#  define WINKEY HWND
#else
#  define WINKEY ::Window
#endif

namespace vrj
{

class Display;
class PfApp;
class Projection;
class SimViewport;
class Viewport;
class PfInputHandler;

    // Performer Config function called in draw proc after window is set up
void PFconfigPWin(pfPipeWindow*);
void PfDrawFunc(pfChannel*, void*, const bool, const bool, const bool,
                const bool);
void PfAppFunc(pfChannel*, void*);

/** \class PfDrawManager PfDrawManager.h vrj/Draw/Pf/PfDrawmanager.h
 *
 * Concrete singleton class for API-specific Draw Manager for OpenGL Performer.
 * Responsible for all Performer rendering and windowing.
 */
class VJ_PF_CLASS_API PfDrawManager : public DrawManager
{
protected:

   /** The channels associated with a viewport. */
   struct pfViewport
   {
      /**
       * Identifier for the channels.
       * ie. PRIMARY is the primary channel, etc.
       */
      enum {PRIMARY = 0, SECONDARY = 1};

      pfViewport()
      {
         chans[0] = chans[1] = NULL;
         viewport = NULL;
      }

      pfChannel*  chans[2];      /**< The channels. */
      Viewport* viewport;        /**< The viewport that we are rendering. */
   };

   struct pfDisplay
   {
      pfDisplay()
         : pWin(NULL)
      {
      }

      DisplayPtr              disp;
      pfPipeWindow*           pWin;
      std::vector<pfViewport> viewports;
   };

   struct pfDisplay_disp : public std::unary_function<pfDisplay, DisplayPtr>
   {
      DisplayPtr operator()(const pfDisplay disp) const
      {
         return disp.disp;
      }
   };

   /*
   struct findPfDispChan : unary_function<pfDisp, bool>
   {
      findPfDispChan(pfChannel* chan) : mChan(chan) {;}
      bool operator()(pfDisp disp)
      {
         return ((disp.chans[0] == mChan) || (disp.chans[1] == mChan));
      }
      pfChannel* mChan;                // Channel to find
   };
   */


public:
   /**
    * Blocks until the end of the frame.
    * @pre None.
    * @post The frame has been drawn.
    */
   virtual void sync();

   /**
    * Enables a frame to be drawn.
    * @pre None.
    * @post Frame has been triggered to begin drawing.
    */
   virtual void draw();

   /**
    * Returns the Simulator SceneGraph so that PfBasicSimulator can
    * add/update the Head and Wand.
    */
   pfScene* getRootWithSim()
   {
      return mRootWithSim;
   }

   /**
    * Sets the app the draw whould interact with.
    * @pre None
    * @post self'.app = _app
    */
   virtual void setApp(App* _app);

   /**
    * Initializes the drawing API (if not already running).  We should call
    * pfInit().
    *
    * Configure process model.<br>
    * Configure multi-pipe model.<br>
    * Then call pfConfig to start the MP stuff.<br>
    * Sets up channels and pWins.
    *
    * @note Fork happens here.
    */
   virtual void initAPI();

   /** Initializes the parameters of the master channel. */
   void initChanGroupAttribs(pfChannel* masterChan);

   /** Callback invoked when display is added to display manager. */
   virtual void addDisplay(DisplayPtr disp);

   /** Callback invoked when display is removed to display manager. */
   virtual void removeDisplay(DisplayPtr disp);

   /** Shuts down the drawing API. */
   virtual void closeAPI();

   /**
    * Updates all the projections for the displays.
    * @post All windows have the projections correctly set.
    */
   virtual void updatePfProjections();

   /** Dumps this object's internal state. */
   void debugDump(int debugLevel);

   void debugDumpPfDisp(pfDisplay* pf_disp, int debugLevel);

   friend void PFconfigPWin(pfPipeWindow*);
   friend void PfDrawFunc(pfChannel*, void*, const bool, const bool,
                          const bool);
   friend void PfAppFunc(pfChannel*, void*);

public: // Config element handlers
   /**
    * Can the handler handle the given element?
    * @returns true if we can handle it; false otherwise.
    */
   virtual bool configCanHandle(jccl::ConfigElementPtr element)
   {
      boost::ignore_unused_variable_warning(element);
      return false;
   }

protected:     // --- Config handling functions --- //
   /**
    * Adds the element to the configuration.
    * @pre configCanHandle(element) == true.
    * @returns false.
    */
   virtual bool configAdd(jccl::ConfigElementPtr element)
   {
      boost::ignore_unused_variable_warning(element);
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
         << "vrj::PfDrawManager::configAdd: configAdd is not supported.\n"
         << vprDEBUG_FLUSH;
      return false;
   }

   /**
    * Removes the element from the current configuration.
    * @pre  configCanHandle(element) == true.
    * @returns false.
    */
   virtual bool configRemove(jccl::ConfigElementPtr element)
   {
      boost::ignore_unused_variable_warning(element);
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
         << "vrj::PfDrawManager::configRemove: configRemove is not supported.\n"
         << vprDEBUG_FLUSH;
      return false;
   }

   /**
    * Sets up display system related attributes.
    * @pre element is a config element of the "dispaySystem" type.
    * @note MUST be called before initDrawing.
    * @note This must be called by the draw manager because the element must
    *       be gotten from the draw manager.
    */
   bool configDisplaySystem(jccl::ConfigElementPtr element);

protected:
   /** Calls all the application channel callbacks. */
   void callAppChanFuncs();

   /** Helper to set channel view params from a Projection. */
   void updatePfProjection(pfChannel* chan, Projection* proj); //, bool simulator=false);

   /** Helper function to create the base scene graph stuff. */
   void initPerformerGraph();

   /**
    * Helper that (re)loads the application's scene graph into the active
    * scene(s).
    */
   void initAppGraph();

   /**
    * Helper to get the pfDisp given a channel.
    *
    * @return NULL to indicate that the display was not found.
    */
   //**//pfDisp* getPfDisp(pfChannel* chan);

   /** Initializes all the pipes that may need to be used. */
   void initPipes();

   /**
    * Returns a Performer pipe.
    * @pre pipeNum < mNumPipes, Fork must have happend.
    */
   pfPipe* getPfPipe(const unsigned int pipeNum);

   /**
    * @name Display helpers
    * Helpers for releasing a display and the associated cruft.
    */
   //@{
   void releaseDisplay(pfDisplay& disp);
   void releaseViewport(pfDisplay& disp, pfViewport& vp);
   pfPipeWindow* allocatePipeWin(const unsigned int pipeNum);
   void releasePipeWin(pfPipeWindow* pipeWin, const unsigned int pipeNum);
   //@}

   /** Returns the needed mono frame buffer config. */
   std::vector<int> getMonoFBConfig(vrj::DisplayPtr disp);
   /** Returns the needed stereo frame buffer config. */
   std::vector<int> getStereoFBConfig(vrj::DisplayPtr disp);

   /**
    * Adds frame buffer configuration options to the given vector based on
    * the given vrj::Display object's OpenGL frame buffer configuration.
    */
   void configFrameBuffer(vrj::DisplayPtr disp, std::vector<int>& attrs);

protected:
   // NOTE:  ---- REMEMBER THAT PF HAS SHARED MEM Model ---
   // Rember that Performer uses forks, so its processes need to
   // have shared memory allocated variable in order to work
   // correctly.

   // --- Config Data --- //
   unsigned int mNumPipes;    /**< The number of Performer pipes */

   /** @name Performer State */
   //@{
   PfApp*                  mApp;              /**< The user application. */
   std::vector<pfDisplay>  mDisplays;         /**< All Performer displays. */
   std::vector<PfInputHandler*> mPfInputHandlers; /**< Input Handlers for all Performer windows. */
   
   std::vector<pfChannel*> mSurfChannels;
   std::vector<pfChannel*> mSimChannels;        /**< List of sim displays. */
   pfChannel*              mSurfMasterChan;  /**< Master channel. */
   pfChannel*              mSimMasterChan;   /**< Master channel for simulators. */
   //@}

   std::vector<pfPipe*>    mPipes;       /**< Performer pipes we have opened. */
   std::vector<std::string> mPipeStrs;   /**< The X11 names of the pipes. */
   bool                    mPfHasForked; /**< Performer has forked its processes already. */

   /**
    * List of (available) pipe windows grouped by pipe num.
    * This is used as a repository of previously allocated but unused pipe
    * windows.  We do this because Performer does not allow the deletion of
    * pfPipeWindows.
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
   pfScene*          mRoot;            /**< Root of performer tree to render. */
   pfGroup*          mSceneRoot;       /**< Root of scene to render (changes at run-time). */
   pfGroup*          mSceneGroup;      /**< The group node with only sceneRoot under it. */
   pfScene*          mRootWithSim;     /**< The root with the simulator group & the sceneRoot. */
   //@}

protected:

   PfDrawManager()
      : mApp(NULL)
      , mSurfMasterChan(NULL)
      , mSimMasterChan(NULL)
      , mPfHasForked(false)
      , mRoot(NULL)
      , mSceneRoot(NULL)
      , mSceneGroup(NULL)
      , mRootWithSim(NULL)
   {;}

   virtual ~PfDrawManager();

   vprSingletonHeader(PfDrawManager);
};

}


#endif
