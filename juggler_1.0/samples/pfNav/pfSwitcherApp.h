/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#ifndef _PF_SWITCHER_APP_H_
#define _PF_SWITCHER_APP_H_

#include <Kernel/vjKernel.h>
#include <Kernel/Pf/vjPfApp.h>
#include <vector>
#include <string>
#include <Performer/pf/pfDCS.h>
#include <Performer/pf/pfSwitch.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pf/pfGroup.h>
#include <Performer/pf/pfLightSource.h>
#include <Input/InputManager/vjDigitalInterface.h>
#include <StopWatch.h>

// Wrapper around a pfApp
class pfAppHandle
{
public:

   pfAppHandle(vjPfApp* app, std::string appName)
   {
      mApp = app;
      mAppName = appName;
      mAppRoot = NULL;
      mAppXformDCS = NULL;
      mAppSwitch = NULL;
      mUnitScaleFactor = 0.0f;
   }

   // Constructs the scene graph
   // POST: All nodes are allocated
   //       DCS set to scaling to unit cube
   //       switchOff() is called
   void constructAppSceneGraph();

   void switchOn();
   void switchOff();

   // Set the scale in the range of [0..mUnitScaleFactor]
   // Where percentage is the amount along the range
   void setScaleZeroToUnit(float percent);

   // Set the scale in the range of [mUnitScaleFactor..1.0(ie. Full Model)]
   // Where percentage is the amount along the range
   void setScaleUnitToFull(float percent);

   // Set scale to the unit scale value
   void setUnitScale();

   // Set scale to full scale (ie. 1.0f)
   void setFullScale();

public:
   vjPfApp*    mApp;
   std::string mAppName;

   // These are for the scene graph structure that the switcher needs
   // -- mAppSwitch -- mAppXformDCS -- mAppRoot --- << APP STUFF >>
   // NOTE: These are allocated by the SWITCHER
   // NOTE: put switcher in each app so multiple can
   //       be displayed at once
   pfGroup*    mAppRoot;
   pfDCS*      mAppXformDCS;
   pfSwitch*   mAppSwitch;             // Switch to select active application
   float       mUnitScaleFactor;    // Scale factor needed to scale to unit cube
   pfVec3      mUnitTrans;          // Amount to translate to get to unit cube
};

// Application that can switch between multiple performer applications
//
// Keeps a list of the applications to switch between
//
// They have to register with this application
//
class pfSwitcherApp : public vjPfApp
{
public:
   // Enums used for the state machine in the program
   enum SwitcherState
   {
      RUN_SWITCHER,        // We are just running the switcher right now
      RUN_APP,             // We are running the active app exclusively
      SWITCH_IN,           // We are Switching into an app
      SWITCH_OUT,          // We are Switching out of an app
      CHANGE_APP,          // Changing the application
      CHANGE_APP_OUT,      // We are changing the active application
      CHANGE_APP_IN
   };

public:
   // ------ SWITCHER INTERFACE ----- //
   // Register an application with the system.
   // This tells the system to render this application
   void registerApp(pfAppHandle appHandle);
   void constructSceneGraphSkeleton();

protected:  // --- HELPERS --- //
   void switchConstructOn();
   void switchConstructOff();

   void addAppGraph(pfAppHandle& handle);
   void removeAppGraph(pfAppHandle& handle);

   // Is the application whose index is given active?
   // ie. Is it supposed to be updated by the callbacks
   bool isAppActive(unsigned index);

   // Set the active application
   //!POST: If have old active, set non-focu
   //       Set focus for new app
   void setActiveApp(unsigned index);

   // Initialize switcher interaction objects
   // Called from init()
   void initInteraction();

   // Perform any interaction updating needed
   // Modifies (transitions) system states
   void updateInteraction();

   // Set a new state
   // Encapsulates any calls that need to be made because of state switch
   // NOTE: If we are already in newState, then just ignore it
   void setState(SwitcherState newState);

protected: // --- TRANSITION HELPERS --- //
   /*
    * The switch transition is for switching from the witcher to running
    * another application in full
    * The change transition is used when changing which application
    * is active and is displayed to the user for selection.
    */

   // Update transition for switching apps in
   // POST: Scene graph is modified for transition
   //       If transition is complete ==> RUN_APP state
   void updateSwitchTransIn();
   void initSwitchTransIn();

   // Update transition for switching apps out
   // POST: Scene graph is modified for transition
   //       If transition is complete ==> RUN state
   void updateSwitchTransOut();
   void initSwitchTransOut();

   // Update transition for changing apps in
   // POST: Scene graph is modified for trans
   //       If transition is complete ==> RUN state
   void updateChangeAppTransIn();
   void initChangeAppTransIn();

   // Update transition for chaning apps out
   // POST: Scene graph is modified for trans
   //       If transition is complete ==> CHANGE_APP_IN
   void updateChangeAppTransOut();
   void initChangeAppTransOut();

private:
   SwitcherState     mCurState;

   bool                       mHaveInitialized;    // Have the app initialization functions been called yet (ie. they won't be called again)
   std::vector<pfAppHandle>   mApps;               // List of the applications
   int                        mActiveApp;          // Index of the active application
   int                        mAppToMakeActive;    // The application that will be made active when in state: APP_CHANGE_STATE

   // Interaction objects
   vjDigitalInterface   mNextAppButton;
   vjDigitalInterface   mPrevAppButton;
   vjDigitalInterface   mSelectButton;

   // Transition members objects
   StopWatch   mClock;           // Clock to keep track of time passage
   float       mChangeTransLength;     // Length of the transitions (in seconds)
   float       mSwitchTransLength;     // Length of transition to switch to an application

                                 // These values run from 0..1 as a percentage of trans completed
   float       mTransIn;         // How much have we transitioned in (NOTE: Shared by both trans types)
   float       mTransOut;        // How much have we transitioned out (NOTE: Shared by both trans types)

   // ---- SCENE GRAPH ---- //
   //           /--- mSun
   //          /--- mConstructSwitch -- mConstructDCS - mConstructModel
   //  mRootNode
   //          \---- mAppRoot(s) --- switch -- sclaeDCS -- model
   //
   pfGroup*       mRootNode;              // Root node of the scene graph
   pfSwitch*      mConstructSwitch;       // Switch for turning on and off the construct
   pfDCS*         mConstructDCS;          // DCS to transform the construct model
   pfNode*        mConstructModel;
   pfDCS*         mAppScalerDCS;          // DCS to scale active application
   pfLightSource* mSun;                   // Sun to light the environment

public:
   // ----------- PERFORMER APP INTERFACE ---------- //

   //: Initialize the scene graph
   // Called after pfInit & pfConfig but before apiInit
   // POST: For all registered applications, call this function
   virtual void initScene();

   //: Called between pfInit and pfConfig
   // POST: For all registered, call this function
   virtual void preForkInit();

   //: Function called in application process for each active channel each frame
   // For each registered, call this function
   virtual void appChanFunc(pfChannel* chan);

   //: Return the current scene graph
   // POST: Return our scene graph
   virtual pfGroup* getScene();

   //: Init a pWin
   // POST: For any registered, called this function
   virtual void configPWin(pfPipeWindow* pWin);

   //: Return the needed parameters for the performer framebuffer
   //! POST: Returns all of the options added together
   virtual std::vector<int> getFrameBufferAttrs();

   // Function called by the DEFAULT drawChan function before clearing the channel
   // and drawing the next frame (pfFrame())
   //! POST: This function is called for each registered app
   virtual void preDrawChan(pfChannel* chan, void* chandata);

   // Function called by the DEFAULT drawChan function after clearing the channel
   // and drawing the next frame (pfFrame())
   //! POST: For each registered app, this function is called
   virtual void postDrawChan(pfChannel* chan, void* chandata);

public:
         // --------- BASE APPLICATION INTERFACE ---------- //

   //: Application init function
   // Execute any initialization needed before the API is started
   //! POST: For each registered app, this is called
   virtual void init();

   //: Application API init function
   // Execute any initialization needed <b>after</b> API is started
   //  but before the drawManager starts the drawing loops.
   //! POST: For each registered app, this is called
   virtual void apiInit();

   //: Execute any final cleanup needed for the application
   //! POST: For each registered app, this is called
   virtual void exit();

   //: Function called before juggler frame starts.
   // Called after tracker update but before start of a new frame
   //! POST: For each registered app, this is called
   virtual void preFrame();

   //: Function called <b>during</b> the application's drawing time
   //! POST: For each registered app, this is called
   virtual void intraFrame();

   //: Function called before updating trackers but after the frame is complete
   //! POST: For each registered app, this is called
   virtual void postFrame();

   //: Reset the application
   // This is used when the system (or applications) would like the application
   // to reset to the initial state that it started in.
   virtual void reset() {;}

public:  // --- CONFIG HANDLERS ---> (inherited from vjConfigChunkHandler) --- //
   // Default to not handling anything
   virtual bool configCanHandle(vjConfigChunk* chunk)
   { return false; }

   //: Are any application dependencies satisfied
   // If the application requires anything special of the system for successful
   // initialization, check it here.
   // If retval == false, then the application will not be started yet
   //    retval == true, application will be allowed to enter the system
   virtual bool depSatisfied()
   { return true; }

protected:
   //! NOTE: Inherited from vjConfigChunkHandler
   virtual bool configAdd(vjConfigChunk* chunk)
   { vjASSERT(false);  return false; }
   //! NOTE: INherited from vjConfigChunkHandler
   virtual bool configRemove(vjConfigChunk* chunk)
   { vjASSERT(false); return false; }

public:     // -------- SINGLETON ---------- //
   //: Get instance of singleton object
   static pfSwitcherApp* instance()
   {
      if (_instance == NULL)
         _instance = new pfSwitcherApp();
      return _instance;
   }

protected:
   pfSwitcherApp()
   {
       mRootNode = NULL;
       mConstructModel = NULL;
       mHaveInitialized = false;
       mAppScalerDCS = NULL;
       mCurState = RUN_SWITCHER;
       mActiveApp = -1;
       mAppToMakeActive = -1;
       mChangeTransLength = 0.5f;
       mSwitchTransLength = 3.0f;
       mTransIn = mTransOut = 0.0f;
   }

private:
   static pfSwitcherApp* _instance;   //: The instance

};


#endif
