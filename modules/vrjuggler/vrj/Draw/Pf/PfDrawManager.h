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


#ifndef _VJ_PF_DRAW_MANAGER_
#define _VJ_PF_DRAW_MANAGER_

#include <vjConfig.h>

#include <functional>
#include <algorithm>

#include <Performer/pf/pfChannel.h>

#include <Kernel/vjDrawManager.h>
#include <Kernel/vjDisplay.h>
#include <Kernel/vjSimViewport.h>

//#include <Kernel/Pf/vjPfApp.h>
#include <Kernel/Pf/vjPfUtil.h>
#include <Utils/vjSingleton.h>


class vjPfApp;
class vjProjection;
class vjConfigChunkDB;

    // Performer Config function called in draw proc after window is set up
void vjPFconfigPWin(pfPipeWindow* pWin);
void vjPfDrawFunc(pfChannel *chan, void* chandata,bool left_eye, bool right_eye, bool stereo, bool simulator);
void vjPfAppFunc(pfChannel *chan, void* chandata);

//------------------------------------------------------------
//: Concrete singleton class for API specific Draw Manager.
//
// Responsible for all Performer rendering and windowing
//
// @author Allen Bierbaum
//  Date: 9-7-97
//------------------------------------------------------------
class vjPfDrawManager : public vjDrawManager
{
protected:

   // The channels associated with a viewport
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

      pfChannel*  chans[2];         // The channels
      vjViewport* viewport;         // The viewport that we are rendering
   };

   struct pfDisplay
   {
      pfDisplay()
      {
         disp = NULL;
         pWin = NULL;
      }

      vjDisplay*              disp;
      pfPipeWindow*           pWin;
      std::vector<pfViewport> viewports;
   };

   struct pfDisplay_disp : public std::unary_function<pfDisplay,vjDisplay*>
   {
     vjDisplay* operator()(const pfDisplay disp) const { return disp.disp; }
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
    //: Function to config API specific stuff.
    // Takes a chunkDB and extracts API specific stuff
   //virtual void configInitial(vjConfigChunkDB*  chunkDB);

    //: Blocks until the end of the frame
    //! PRE: none
    //! POST: The frame has been drawn
   virtual void sync();

   //: Enable a frame to be drawn
   //! PRE: none
   //! POST: Frame has been triggered to begin drawing
   virtual void draw();


   //: Set the app the draw whould interact with.
   //! PRE: none
   //! POST: self'.app = _app
   virtual void setApp(vjApp* _app);


   //: Initialize the drawing API (if not already running)
   //  should call pfInit()
   //
   // Configure process model
   // Configure multi-pipe model
   // Then call pfConfig to start the MP stuff
   // Sets up channels and pWins.
   //
   //!NOTE: Fork happens here
   virtual void initAPI();

   // Initialize the parameters of the master channel
   void initChanGroupAttribs(pfChannel* masterChan);

   //: Callback when display is added to display manager
   virtual void addDisplay(vjDisplay* disp);

   //: Callback when display is removed to display manager
   virtual void removeDisplay(vjDisplay* disp);

      //: Shutdown the drawing API
   virtual void closeAPI();

      //: Update all the projections for the displays
      //!POST: All windows have the projections correctly set.
   virtual void updateProjections();

   //: dumps the object's internal state
   void debugDump(int debugLevel);

   void debugDumpPfDisp(pfDisplay* pf_disp, int debugLevel);

   friend void vjPFconfigPWin(pfPipeWindow* pWin);
   friend void vjPfDrawFunc(pfChannel *chan, void* chandata,bool left_eye, bool right_eye, bool stereo, bool simulator);
   friend void vjPfAppFunc(pfChannel *chan, void* chandata);

public: // Chunk handlers
   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   virtual bool configCanHandle(vjConfigChunk* chunk);

protected:     // --- Config handling functions --- //
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! RETURNS: success
   virtual bool configAdd(vjConfigChunk* chunk);

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //!RETURNS: success
   virtual bool configRemove(vjConfigChunk* chunk)
   {
      vjDEBUG(vjDBG_ALL,vjDBG_CRITICAL_LVL) << "vjPfDrawManager::configRemove: configRemove is not supported.\n" << vjDEBUG_FLUSH;
      return false;
   }

   //: Setup display system related attributes
   //! PRE: chunk is a chunks of the "dispaySystem" type
   //! NOTE: MUST be called before initDrawing
   //! NOTE: This must be called by the draw manager
   //        because the chunk must be gotten from the draw manager
   bool configDisplaySystem(vjConfigChunk* chunk);

   //: Configure pfAPI stuff
   //! PRE: chunk.type == "apiPerformer"
   //! NOTE: MUST be called before initDrawing
   bool configPerformerAPI(vjConfigChunk* chunk);

protected:
   //: Call all the application channel callbacks
   void callAppChanFuncs();

   //: Helper to set channel view params from a vjProjection
   void updatePfProjection(pfChannel* chan, vjProjection* proj, bool simulator=false);

   //: Helper function to create the base scene graph stuff
   void initPerformerGraph();

   //: Helper to initialize the Performer simulato
   void initSimulatorGraph();

   //: Helper that (re)loads the application's scene graph into the active scene(s)
   void initAppGraph();


   void initLoaders();
   void updateSimulator(vjSimViewport* simVp);

   //: Helper to get the pfDisp given a channel
   //! RETURNS: NULL - Not found
   //**//pfDisp* getPfDisp(pfChannel* chan);

   // Init all the pipes that may need to be used
   void initPipes();

   // Get a performer pipe
   //! PRE: pipe_num < mNumPipes
   //       Fork must have happend
   pfPipe* getPfPipe(unsigned pipe_num);

   // Helpers for releasing a display and the associated cruft
   void releaseDisplay(pfDisplay& disp);
   void releaseViewport(pfDisplay& disp, pfViewport& vp);
   pfPipeWindow* allocatePipeWin(unsigned pipeNum);
   void releasePipeWin(pfPipeWindow* pipeWin, unsigned pipeNum);

   //: Return the needed mono frame buffer config
   std::vector<int> getMonoFBConfig();
   //: Return the needed stereo frame buffer config
   std::vector<int> getStereoFBConfig();

protected:
   // NOTE:  ---- REMEMBER THAT PF HAS SHARED MEM Model ---
   // Rember that Performer uses forks, so it's processes need to
   // have shared memory allocated variable in order to work
   // correctly.

   // --- Config Data --- //
   unsigned int mNumPipes;    // The number of Performer pipes

   // --- Performer State --- //
   vjPfApp*                app;              // There User applications
   std::vector<pfDisplay>  mDisplays;        // All Performer displays

   std::vector<pfChannel*> mSurfChannels;
   std::vector<pfChannel*> mSimChannels;        // List of sim displays
   pfChannel*              mSurfMasterChan;  // Master channel
   pfChannel*              mSimMasterChan;   // Master channel for simulators

   std::vector<pfPipe*>    mPipes;            // Performer pipes we have opened
   std::vector<char*>      mPipeStrs;        // The X-Strs of the pipes
   bool                    mPfHasForked;     // Performer has forked it processes already

   // List of (available) pipe windows grouped by pipe num
   // This is used as a repository of previously allocated but unused pipe windows
   // We do this because Performer does not allow the deletion of pfPipeWindows
   std::vector< std::vector<pfPipeWindow*> > mPipeWindows;

   //   mRoot
   //       |
   //     mSceneGroup -- mSceneRoot -- app scene
   //       /
   // mRootWithSim   mHeadDCS
   //       \       /
   //       mSimTree
   //              |
   //              mWandDCS
   pfScene*          mRoot;            // Root of performer tree to render
   pfGroup*          mSceneRoot;       // Root of scene to render (changes at run-time)
   pfGroup*          mSceneGroup;      // The group node with only sceneRoot under it
   pfScene*          mRootWithSim;  // The root with the simulator group & the sceneRoot


   // ---- Simulator stuff --- //
   pfGroup*          mSimTree;      // The simulator scene graph
   pfDCS*            mHeadDCS;      // The DCS above the head
   pfDCS*            mWandDCS;      // The DCS above the wand
   std::string       mHeadModel;    // The head model file path
   std::string       mWandModel;    // The wand model file path


protected:

   vjPfDrawManager() {
      mSurfMasterChan = NULL;
      mSimMasterChan = NULL;
      mRoot         = NULL;
      mSceneRoot    = NULL;
      mSceneGroup = NULL;
      mSimTree     = NULL;
      mRootWithSim = NULL;
      mHeadDCS     = NULL;
      mWandDCS     = NULL;
      mPfHasForked = false;
   }

   virtual ~vjPfDrawManager() {}

   vjSingletonHeader(vjPfDrawManager);
/*
   // --- Singleton Stuff --- //
public:
   static vjPfDrawManager* instance()
   {
      if (_instance == NULL)
      {
         _instance = new vjPfDrawManager();
      }
      return _instance;
   }

private:
   static vjPfDrawManager* _instance;
   */
};


#endif
