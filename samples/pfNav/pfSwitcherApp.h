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

// Wrapper around a pfApp
class pfAppHandle
{
public:

   pfAppHandle(vjPfApp* app, std::string appName)
   {
      mApp = app;
      mAppName = appName;
      mAppXformDCS = NULL;
      mAppRoot = NULL;
      mAppSwitch = NULL;
   }

public:
   vjPfApp*    mApp;
   std::string mAppName;

   // These are for the scene graph structure that the switcher needs
   // -- mAppXformDCS -- mAppRoot --- << APP STUFF >>
   // NOTE: These are allocated by the SWITCHER
   pfGroup*    mAppRoot;
   pfSwitch*   mAppSwitch;
   pfDCS*      mAppXformDCS;
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
   pfSwitcherApp(vjKernel* kern) : vjPfApp(kern)
   {
       mRootNode = NULL;
       mConstructModel = NULL;
       mHaveInitialized = false;
   }

public:
   // ------ SWITCHER INTERFACE ----- //
   // Register an application with the system.
   // This tells the system to render this application
   void registerApp(pfAppHandle appHandle);

   void constructSceneGraphSkeleton();

   void constructAppScene(pfAppHandle& handle);

   void addAppGraph(pfAppHandle& handle);
   void removeAppGraph(pfAppHandle& handle);

private:
   // ---- SCENE GRAPH ---- //
   //           /--- mSun
   //          /--- mConstructDCS - mConstructModel
   //  mRootNode
   //          \--- mAppXformDCS -- mAppSwitch -- mAppRoot(s)
   //
   pfGroup*       mRootNode;              // Root node of the scene graph
   pfDCS*         mConstructDCS;          // DCS to transform the construct model
   pfNode*        mConstructModel;
   pfLightSource* mSun;                      // Sun to light the environment

   bool                       mHaveInitialized;    // Have the app initialization functions been called yet (ie. they won't be called again)
   std::vector<pfAppHandle>   mApps;      // List of the applications

public:
   // ----------- PERFORMER APP INTERFACE ---------- //

   //: Initialize the scene graph
   // Called after pfInit & pfConfig but before apiInit
   // POST: For all registered applications, call this funtcion
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
         _instance = new pfSwitcherApp(vjKernel::instance());
      return _instance;
   }

private:
   static pfSwitcherApp* _instance;   //: The instance

};


#endif
