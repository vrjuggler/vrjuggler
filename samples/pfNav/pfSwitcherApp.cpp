#include <pfSwitcherApp.h>
#include <pfFileIO.h>
#include <Performer/pfutil.h>

// ------------------------------- //
// ------ SWITCHER INTERFACE ----- //
// ------------------------------- //

// Register an application with the system.
// This tells the system to render this application
void pfSwitcherApp::registerApp(pfAppHandle appHandle)
{
   // Add application to list
   mApps.push_back(appHandle);
   vjDEBUG(vjDBG_ALL,0) << clrSetNORM(clrMAGENTA) << "Registered App: " << appHandle.mAppName << clrRESET << endl << vjDEBUG_FLUSH;

   // If we have already initialized, then call whatever we can
   // XXX: write this code
}

// Build a skeleton outline of the scenegraph
// Only consructs the parts that are not app specific
void pfSwitcherApp::constructSceneGraphSkeleton()
{
   mRootNode      = new pfGroup;
   mConstructDCS  = new pfDCS;

   // Load the construct model
   pfFileIO::addFilePath("/usr/share/Performer/data:");
   mConstructModel = pfFileIO::loadFile("board.sv");
   //mConstructModel = new pfGroup();

   if(mConstructModel == NULL)
   {
      vjDEBUG(vjDBG_ALL,0) << clrOutNORM(clrRED,"Could not load construct model\n") << vjDEBUG_FLUSH;
      return;
   }

   mRootNode->addChild(mConstructDCS);
   mConstructDCS->addChild(mConstructModel);
   mConstructDCS->setTrans(0.0f,0.0f,-1.0f);

   // Add the sun to the root
   mSun = new pfLightSource;                 // Create the SUN
   mSun->setPos( 0.3f, 0.0f, 0.3f, 0.0f );
   mSun->setColor( PFLT_DIFFUSE,0.9f,0.9f,0.9f );
   mSun->setColor( PFLT_AMBIENT,0.2f,0.2f,0.2f );
   mSun->setColor( PFLT_SPECULAR, 1.0f, 1.0f, 1.0f );
   mSun->on();
   mRootNode->addChild(mSun);
}

// Called for each application
// Called after: initScene, apiInit
// Called as part of: getScene()
// Calls app's getScene()
void pfSwitcherApp::constructAppScene(pfAppHandle& handle)
{
   vjASSERT(handle.mApp != NULL);
   vjDEBUG(vjDBG_ALL,0) << clrSetNORM(clrMAGENTA)
                        << "Constructing appScene for: " << handle.mAppName
                        << clrRESET<< endl << vjDEBUG_FLUSH;
   handle.mAppRoot = handle.mApp->getScene();
   handle.mAppSwitch = new pfSwitch;
   handle.mAppSwitch->setVal(PFSWITCH_ON);
   handle.mAppXformDCS = new pfDCS;
   handle.mAppSwitch->addChild(handle.mAppRoot);
   handle.mAppXformDCS->addChild(handle.mAppSwitch);

   pfBox bound_box;
   pfuTravCalcBBox(handle.mAppRoot,&bound_box);
   pfVec3 diag = (bound_box.max - bound_box.min);
   handle.mUnitScaleFactor = (1.0f/diag.length());
   handle.mUnitTrans = -((bound_box.max+bound_box.min)/2.0f);

   //pfSphere app_bound;
   //handle.mAppRoot->getBound(&app_bound);
   //handle.mUnitScaleFactor = (1.0f/app_bound.radius);

   handle.mAppXformDCS->setScale(handle.mUnitScaleFactor);
}

void pfSwitcherApp::addAppGraph(pfAppHandle& handle)
{
   mRootNode->addChild(handle.mAppXformDCS);
}

void pfSwitcherApp::removeAppGraph(pfAppHandle& handle)
{
   mRootNode->removeChild(handle.mAppXformDCS);
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
   // Init my scene stuff
   constructSceneGraphSkeleton();

   // Init any apps scene stuff
   for(unsigned i=0;i<mApps.size();i++)
      mApps[i].mApp->initScene();

   // For all registered, construct their skeleton scenes
   // And add them to mine
   // NOTE: This invokes getScene internally
   for(unsigned j=0;j<mApps.size();j++)
   {
      constructAppScene(mApps[j]);
      addAppGraph(mApps[j]);
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
      mApps[i].mApp->preDrawChan(chan,chandata);
}

// Function called by the DEFAULT drawChan function after clearing the channel
// and drawing the next frame (pfFrame())
//! POST: For each registered app, this function is called
void pfSwitcherApp::postDrawChan(pfChannel* chan, void* chandata)
{
   for(unsigned i=0;i<mApps.size();i++)
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
   for(unsigned i=0;i<mApps.size();i++)
      mApps[i].mApp->preFrame();
}

//: Function called <b>during</b> the application's drawing time
//! POST: For each registered app, this is called
void pfSwitcherApp::intraFrame()
{
   for(unsigned i=0;i<mApps.size();i++)
      mApps[i].mApp->intraFrame();;
}

//: Function called before updating trackers but after the frame is complete
//! POST: For each registered app, this is called
void pfSwitcherApp::postFrame()
{
   for(unsigned i=0;i<mApps.size();i++)
      mApps[i].mApp->postFrame();
}
