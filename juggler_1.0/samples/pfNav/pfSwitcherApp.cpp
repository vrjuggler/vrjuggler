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

#include <pfSwitcherApp.h>
#include <pfFileIO.h>
#include <Performer/pfutil.h>

#define SWITCHER_APP_BASE_SIZE 5.0f

pfSwitcherApp* pfSwitcherApp::_instance = NULL;

// Called for each application
// Called after: initScene, apiInit
// Called as part of: getScene()
// Calls app's getScene()
void pfAppHandle::constructAppSceneGraph()
{
   vjDEBUG(vjDBG_ALL,0) << clrSetNORM(clrMAGENTA)
                        << "Constructing appScene for: " << mAppName
                        << clrRESET<< endl << vjDEBUG_FLUSH;
   mAppRoot = mApp->getScene();
   mAppXformDCS = new pfDCS;
   mAppXformDCS->addChild(mAppRoot);
   // ------ CONFIGURE APP portion of graph ---- //
   mAppSwitch = new pfSwitch;
   switchOff();
   mAppSwitch->addChild(mAppXformDCS);

   // Get Scaling factor
   pfBox bound_box;
   pfuTravCalcBBox(mAppRoot,&bound_box);
   pfVec3 diag = (bound_box.max - bound_box.min);
   mUnitScaleFactor = (SWITCHER_APP_BASE_SIZE/diag.length());
   mUnitTrans = -((bound_box.max+bound_box.min)/2.0f);

   //
   mAppXformDCS->setScale(mUnitScaleFactor);
}

void pfAppHandle::switchOn()
{
   if(mAppSwitch != NULL)
   {
      vjDEBUG(vjDBG_ALL,0) << clrOutNORM(clrMAGENTA,"Switching on:") << mAppName << clrRESET << endl << vjDEBUG_FLUSH;
      mAppSwitch->setVal(PFSWITCH_ON);
   }
}

void pfAppHandle::switchOff()
{
   if(mAppSwitch != NULL)
   {
      vjDEBUG(vjDBG_ALL,0) << clrOutNORM(clrMAGENTA,"Switching off:") << mAppName << clrRESET << endl << vjDEBUG_FLUSH;
      mAppSwitch->setVal(PFSWITCH_OFF);
   }
}

// Set the scale in the range of [0..mUnitScaleFactor]
// Where percentage is the amount along the range
void pfAppHandle::setScaleZeroToUnit(float percent)
{
   mAppXformDCS->setScale(percent*mUnitScaleFactor);
}

// Set the scale in the range of [mUnitScaleFactor..1.0(ie. Full Model)]
// Where percentage is the amount along the range
void pfAppHandle::setScaleUnitToFull(float percent)
{
   float range = 1.0f-mUnitScaleFactor;
   float new_scale = mUnitScaleFactor + (percent*range);
   mAppXformDCS->setScale(new_scale);
}

// Set scale to the unit scale value
void pfAppHandle::setUnitScale()
{
   mAppXformDCS->setScale(mUnitScaleFactor);
}

// Set scale to full scale (ie. 1.0f)
void pfAppHandle::setFullScale()
{
   mAppXformDCS->setScale(1.0f);
}

// ------------------------------- //
// ------ SWITCHER INTERFACE ----- //
// ------------------------------- //

// Register an application with the system.
// This tells the system to render this application
void pfSwitcherApp::registerApp(pfAppHandle appHandle)
{
   // Add application to list
   mApps.push_back(appHandle);
   appHandle.mApp->setFocus(false);          // Set to non-focused
   vjDEBUG(vjDBG_ALL,0) << clrSetNORM(clrMAGENTA) << "Registered App: " << appHandle.mAppName << clrRESET << endl << vjDEBUG_FLUSH;

   // If first application in list, then set it active
   if(mActiveApp == -1)
   { setActiveApp(0); }

   // If we have already initialized, then call whatever we can
   // XXX: write this code
}

// Build a skeleton outline of the scenegraph
// Only consructs the parts that are not app specific
void pfSwitcherApp::constructSceneGraphSkeleton()
{
   mRootNode         = new pfGroup;
   mConstructDCS     = new pfDCS;
   mConstructSwitch  = new pfSwitch;

   // ----- CONFIGURE CONSTRUCT MODEL ----- //
   // Load the construct model
   pfFileIO::addFilePath("/usr/share/Performer/data:");
   mConstructModel = pfFileIO::loadFile("board.sv");
   //mConstructModel = new pfGroup();

   if(mConstructModel == NULL)
   {
      vjDEBUG(vjDBG_ALL,0) << clrOutNORM(clrRED,"Could not load construct model\n") << vjDEBUG_FLUSH;
      return;
   }

   mRootNode->addChild(mConstructSwitch);
   mConstructSwitch->addChild(mConstructDCS);
   mConstructDCS->addChild(mConstructModel);
   mConstructDCS->setTrans(0.0f,0.0f,-1.0f);

   // -- ADD SUN ---
   mSun = new pfLightSource;                 // Create the SUN
   mSun->setPos( 0.3f, 0.0f, 0.3f, 0.0f );
   mSun->setColor( PFLT_DIFFUSE,0.9f,0.9f,0.9f );
   mSun->setColor( PFLT_AMBIENT,0.2f,0.2f,0.2f );
   mSun->setColor( PFLT_SPECULAR, 1.0f, 1.0f, 1.0f );
   mSun->on();
   mRootNode->addChild(mSun);
}

void pfSwitcherApp::addAppGraph(pfAppHandle& handle)
{
   mRootNode->addChild(handle.mAppSwitch);
}

void pfSwitcherApp::removeAppGraph(pfAppHandle& handle)
{
   mRootNode->removeChild(handle.mAppSwitch);
}

// --------------------- //
// ------ HELPERS ------ //
// --------------------- //
void pfSwitcherApp::switchConstructOn()
{
   mConstructSwitch->setVal(PFSWITCH_ON);
}

void pfSwitcherApp::switchConstructOff()
{
   mConstructSwitch->setVal(PFSWITCH_OFF);
}

// Is the application whose index is given active?
// ie. Is it supposed to be updated by the callbacks
bool pfSwitcherApp::isAppActive(unsigned index)
{
   return (index == mActiveApp);
   //return true;
}

// Set the active application
//!POST: If have old active, set non-focu
//       Set focus for new app
void pfSwitcherApp::setActiveApp(unsigned index)
{
   vjASSERT(index < mApps.size());

   vjDEBUG(vjDBG_ALL,0) << clrSetNORM(clrCYAN)
                        << "pfSwitcher::setActiveApp Setting active app to: "
                        << mApps[index].mAppName << " idx:"
                        << index << endl << clrRESET << vjDEBUG_FLUSH;

   // -- HAVE OLD ONE -- //
   // Set old non-focused
   if(mActiveApp != -1)
   {
      vjDEBUG(vjDBG_ALL,0) << clrSetNORM(clrCYAN)
                        << "pfSwitcher::setActiveApp have old ap: "
                        << mApps[mActiveApp].mAppName << " idx:"
                        << mActiveApp << " Closing it.\n" << clrRESET << vjDEBUG_FLUSH;
      mApps[mActiveApp].switchOff();
   }

   // -- SET NEW ONE -- //
   mActiveApp = index;                       // Set new active
   mApps[mActiveApp].switchOn();
}

void pfSwitcherApp::initInteraction()
{
   mNextAppButton.init(std::string("VJButton3"));
   mPrevAppButton.init(std::string("VJButton5"));
   mSelectButton.init(std::string("VJButton4"));
}


// Perform any interaction updating needed
void pfSwitcherApp::updateInteraction()
{
   bool next_btn(false);
   bool prev_btn(false);
   bool select_btn(false);

   if(mNextAppButton->getData() == vjDigital::TOGGLE_ON)
   {
      next_btn = true;
      vjDEBUG(vjDBG_ALL,0) << clrOutBOLD(clrMAGENTA,"NEXT APP btn pressed\n") << vjDEBUG_FLUSH;
   }

   if(mPrevAppButton->getData() == vjDigital::TOGGLE_ON)
   {
      prev_btn = true;
      vjDEBUG(vjDBG_ALL,0) << clrOutBOLD(clrMAGENTA,"PREV APP btn pressed\n") << vjDEBUG_FLUSH;
   }

   if(mSelectButton->getData() == vjDigital::TOGGLE_ON)
   {
      select_btn = true;
      vjDEBUG(vjDBG_ALL,0) << clrOutBOLD(clrMAGENTA,"SELECT btn pressed\n") << vjDEBUG_FLUSH;
   }


      // ------ STATE SWITCHING -------- //
   if(mCurState == RUN_SWITCHER)
   {
      if((next_btn) || (prev_btn))
      {
         // RUN_SWITCHER [NextApp]/CHANGE_APP_OUT
         // RUN_SWITCHER [PrevApp]/CHANGE_APP_OUT
         int new_active(mActiveApp);
         if(next_btn)                        // nextApp
            new_active += 1;
         else if(prev_btn)                   // PrevApp
            new_active -= 1;

         // Correct for out of range values
         if((unsigned)new_active >= mApps.size())
         { new_active = 0; }
         else if(new_active < 0)
         { new_active = (mApps.size()-1);}

         mAppToMakeActive = new_active;
         vjDEBUG(vjDBG_ALL,0) << clrOutBOLD(clrMAGENTA,"SELECTED New app: RUN_SWITCHER -> CHANGE_APP_OUT: ")
                              << "appToMakeCurrent:" << mApps[mAppToMakeActive].mAppName << " id:" << mAppToMakeActive << vjDEBUG_FLUSH;
         setState(CHANGE_APP_OUT);
      }
      else if(select_btn)
      {
         // RUN_SWITCHER [select_btn]/SWITCH_IN
         setState(SWITCH_IN);
      }
   }
   else if(mCurState == CHANGE_APP_OUT)
   {
      // No keypresses allowed

      // Update the change transition
      updateChangeAppTransOut();       // This may switch state to: CHANGE_APP
   }
   else if(mCurState == CHANGE_APP)
   {
      // No keypresses allowed

      // Complete the change and start the CHANGE_APP_IN
      vjASSERT(mAppToMakeActive != -1);
      setActiveApp(mAppToMakeActive);
      mAppToMakeActive = -1;

      vjDEBUG(vjDBG_ALL,0) << clrOutBOLD(clrMAGENTA,"CHANGE_APP State: CHANGE_APP -> CHANGE_APP_IN: ")
                              << "new cur app:" << mApps[mActiveApp].mAppName << " id:" << mActiveApp << vjDEBUG_FLUSH;
      setState(CHANGE_APP_IN);
   }
   else if(mCurState == CHANGE_APP_IN)
   {
      // No keypresses allowed

      // Update the change transition
      updateChangeAppTransIn();        // This may switch state to: RUN_SWITCHER
   }
   else if(mCurState == SWITCH_IN)
   {
      // No keypress allowed
      // Update the switch transition
      updateSwitchTransIn();           // This may switch state to: RUN_APP
   }
   else if(mCurState == RUN_APP)
   {
      // RUN_APP [select_btn]/SWITCH_OUT
      if(select_btn)    // While in application, selected to come out
      {
         setState(SWITCH_OUT);
      }
   }
   else if(mCurState == SWITCH_OUT)
   {
      // No keypress allowed
      // Update the switch transition
      updateSwitchTransOut();          // This may switch state to: RUN_SWITCHER
   }

}

// Set a new state
// Encapsulates any calls that need to be made because of state switch
void pfSwitcherApp::setState(SwitcherState newState)
{
   // Ignore if we are already in the state
   if(mCurState == newState)
   {
      return;
   }

   if(newState == RUN_SWITCHER)
   {
      mCurState = newState;
   }
   else if(newState == CHANGE_APP_OUT)
   {
      mCurState = newState;
      initChangeAppTransOut();
   }
   else if(newState == CHANGE_APP)
   {
      mCurState = newState;
   }
   else if(newState == CHANGE_APP_IN)
   {
      mCurState = newState;
      initChangeAppTransIn();
   }
   else if(newState == SWITCH_IN)
   {
      mCurState = newState;
      initSwitchTransIn();
   }
   else if(newState == RUN_APP)
   {
      switchConstructOff();                     // Don't need to display construct any more
      mApps[mActiveApp].mApp->setFocus(true);   // Give the application focus
      mCurState = newState;
   }
   else if(newState == SWITCH_OUT)
   {
      switchConstructOn();                      // We need to display it again
      mApps[mActiveApp].mApp->setFocus(false);  // It is losing focus
      mCurState = newState;
      initSwitchTransOut();
   }
}


// ------------------------------------ //
// -------- TRANSITION HELPERS -------- //
// ------------------------------------ //

// Update transition for switching apps in
// POST: Scene graph is modified for transition
//       If transition is complete ==> RUN_APP state
void pfSwitcherApp::updateSwitchTransIn()
{
   // Update the percentage complete
   mTransIn += (mClock.timeInstant/ mSwitchTransLength);
   vjDEBUG(vjDBG_ALL,0) << "AppChangeIn: " << mTransIn << endl << vjDEBUG_FLUSH;

   // If not complete, Update model based on that information
   // Else if complete, Stop transition and change state ==> RUN_APP
   if(mTransIn < 1.0f)  // NOT DONE
   {
      mApps[mActiveApp].setScaleUnitToFull(mTransIn);
   }
   else                 // DONE
   {
      mApps[mActiveApp].setFullScale();
      setState(RUN_APP);
   };
}

void pfSwitcherApp::initSwitchTransIn()
{
   mTransIn = 0.0f;                                   // Initialize transtion value
   mApps[mActiveApp].setScaleUnitToFull(mTransIn);    // Set the initial scale;
}

// Update transition for switching apps out
// POST: Scene graph is modified for transition
//       If transition is complete ==> RUN_SWITCHER state
void pfSwitcherApp::updateSwitchTransOut()
{
   // Update the percentage of transition completed
   mTransOut += (mClock.timeInstant/mSwitchTransLength);

   // If not complete, Update model based on that information
   // Else if complete, Stop transition and change state ==> RUN_SWITCHER
   if(mTransOut < 1.0f) // NOT DONE
   {
      mApps[mActiveApp].setScaleUnitToFull(1.0f-mTransOut);
   }
   else                 // DONE
   {
      mApps[mActiveApp].setUnitScale();   // Verify that we are at unit scale
      mApps[mActiveApp].mApp->reset();    // Reset the application that we just used
      setState(RUN_SWITCHER);
   };
}
void pfSwitcherApp::initSwitchTransOut()
{
   mTransOut = 0.0f;                                        // Initialize tranition value
   mApps[mActiveApp].setScaleUnitToFull(1.0f-mTransOut);    // Set the initial scale;
}

// Update transition for changing apps in
// POST: Scene graph is modified for trans
//       If transition is complete ==> RUN_SWITCHER state
void pfSwitcherApp::updateChangeAppTransIn()
{
   // Update the percentage complete
   mTransIn += (mClock.timeInstant/ mChangeTransLength);
   vjDEBUG(vjDBG_ALL,0) << "AppTransIn: " << mTransIn << endl << vjDEBUG_FLUSH;

   // If not complete, Update model based on that information
   // Else if complete, Stop transition and change state ==> RUN_SWITCHER
   if(mTransIn < 1.0f)  // NOT DONE
   {
      mApps[mActiveApp].setScaleZeroToUnit(mTransIn);
   }
   else                 // DONE
   {
      mApps[mActiveApp].setUnitScale();
      setState(RUN_SWITCHER);
   }

}
void pfSwitcherApp::initChangeAppTransIn()
{
   mTransIn = 0.0f;                                   // Initialize transtion value
   mApps[mActiveApp].setScaleZeroToUnit(mTransIn);    // Set the initial scale
}

// Update transition for chaning apps out
// POST: Scene graph is modified for trans
//       If transition is complete ==> CHANGE_APP
void pfSwitcherApp::updateChangeAppTransOut()
{
   // Update the percentage of transition completed
   mTransOut += (mClock.timeInstant/mChangeTransLength);

   // If not complete, Update model based on that information
   // Else if complete, Stop transition and change state ==> CHANGE_APP
   if(mTransOut < 1.0f) // NOT DONE
   {
      mApps[mActiveApp].setScaleZeroToUnit(1.0f-mTransOut);
   }
   else                 // DONE
   {
      // Leave scale alone
      setState(CHANGE_APP);
   }
}
void pfSwitcherApp::initChangeAppTransOut()
{
   mTransOut = 0.0f;                                        // Initialize tranition value
   mApps[mActiveApp].setScaleZeroToUnit(1.0f-mTransOut);    // Set the initial scale
}

// ---------------------------------------------- //
// ----------- PERFORMER APP INTERFACE ---------- //
// ---------------------------------------------- //

//: Initialize the scene graph
// Called after pfInit & pfConfig but before apiInit
// POST: For all registered applications, call this funtcion
// - Construct my scnee graph
// - Call init scene on apps
// - Construct app graphs
// - Add app to my scene grapy
void pfSwitcherApp::initScene()
{
   mHaveInitialized = true;      // Tell future app adds that we are already inited

   // --- SWITCHER SKELETON -- //
   constructSceneGraphSkeleton();

   // --- APPS InitScene() --- //
   for(unsigned i=0;i<mApps.size();i++)
      mApps[i].mApp->initScene();

   // -- CONSTRUCT APPS -- //
   // For all registered, construct their scenes And add them to mine
   // NOTE: This invokes getScene internally
   for(unsigned j=0;j<mApps.size();j++)
   {
      mApps[j].constructAppSceneGraph();
      addAppGraph(mApps[j]);
   }

   // Make sure that the active application is viewable
   if(mActiveApp != -1)
   {
      vjASSERT(mApps[mActiveApp].mAppSwitch != NULL);
      mApps[mActiveApp].switchOn();
   }
}

//: Called between pfInit and pfConfig
// POST: For all registered, call this function
void pfSwitcherApp::preForkInit()
{
   for(unsigned i=0;i<mApps.size();i++)
      mApps[i].mApp->preForkInit();
}

//: Function called in application process for each active channel each frame
// For each registered, call this function
void pfSwitcherApp::appChanFunc(pfChannel* chan)
{
   for(unsigned i=0;i<mApps.size();i++)
      if(isAppActive(i))
         mApps[i].mApp->appChanFunc(chan);
}

//: Return the current scene graph
// POST: Return our scene graph
pfGroup* pfSwitcherApp::getScene()
{
   return mRootNode;
}

//: Init a pWin
// POST: For any registered, called this function
void pfSwitcherApp::configPWin(pfPipeWindow* pWin)
{
   for(unsigned i=0;i<mApps.size();i++)
      mApps[i].mApp->configPWin(pWin);
}

//: Return the needed parameters for the performer framebuffer
//! POST: Returns all of the options added together
std::vector<int> pfSwitcherApp::getFrameBufferAttrs()
{
   std::vector<int> ret_val;
   std::vector<int> temp_val;

   for(unsigned i=0;i<mApps.size();i++)
   {
      temp_val = mApps[i].mApp->getFrameBufferAttrs();
      ret_val.insert(ret_val.end(),temp_val.begin(),temp_val.end());
   }
   return ret_val;
}

// Function called by the DEFAULT drawChan function before clearing the channel
// and drawing the next frame (pfFrame())
//! POST: This function is called for each registered app
void pfSwitcherApp::preDrawChan(pfChannel* chan, void* chandata)
{
   for(unsigned i=0;i<mApps.size();i++)
      if(isAppActive(i))
         mApps[i].mApp->preDrawChan(chan,chandata);
}

// Function called by the DEFAULT drawChan function after clearing the channel
// and drawing the next frame (pfFrame())
//! POST: For each registered app, this function is called
void pfSwitcherApp::postDrawChan(pfChannel* chan, void* chandata)
{
   for(unsigned i=0;i<mApps.size();i++)
      if(isAppActive(i))
         mApps[i].mApp->postDrawChan(chan,chandata);
}

// ----------------------------------------------- //
// --------- BASE APPLICATION INTERFACE ---------- //
// ----------------------------------------------- //

//: Application init function
// Execute any initialization needed before the API is started
//! POST: For each registered app, this is called
void pfSwitcherApp::init()
{
   mClock.start();            // Initialize clock

   // Initialize switcher
   initInteraction();         // Initialize the interation objects

   // Call app init functions
   for(unsigned i=0;i<mApps.size();i++)
      mApps[i].mApp->init();
}

//: Application API init function
// Execute any initialization needed <b>after</b> API is started
//  but before the drawManager starts the drawing loops.
//! POST: For each registered app, this is called
void pfSwitcherApp::apiInit()
{
   for(unsigned i=0;i<mApps.size();i++)
      mApps[i].mApp->apiInit();
}

//: Execute any final cleanup needed for the application
//! POST: For each registered app, this is called
void pfSwitcherApp::exit()
{
   for(unsigned i=0;i<mApps.size();i++)
      mApps[i].mApp->exit();
}

//: Function called before juggler frame starts.
// Called after tracker update but before start of a new frame
//! POST: For each registered app, this is called
void pfSwitcherApp::preFrame()
{
   // Keep clock up to date
   mClock.stop();
   mClock.start();

   // -- UPDATE Switcher app -- //
   updateInteraction();

   for(unsigned i=0;i<mApps.size();i++)
      if(isAppActive(i))
         mApps[i].mApp->preFrame();
}

//: Function called <b>during</b> the application's drawing time
//! POST: For each registered app, this is called
void pfSwitcherApp::intraFrame()
{
   for(unsigned i=0;i<mApps.size();i++)
      if(isAppActive(i))
         mApps[i].mApp->intraFrame();;
}

//: Function called before updating trackers but after the frame is complete
//! POST: For each registered app, this is called
void pfSwitcherApp::postFrame()
{
   for(unsigned i=0;i<mApps.size();i++)
      if(isAppActive(i))
         mApps[i].mApp->postFrame();
}
