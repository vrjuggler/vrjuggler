#ifndef _SIMPLE_PF_NAV_APP_H_
#define _SIMPLE_PF_NAV_APP_H_

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <gl/device.h>
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfEarthSky.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pf/pfTraverser.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pfdu.h>
#include <Performer/pfutil.h>

    // --- VR Juggler Stuff --- //
#include <Kernel/vjKernel.h>
#include <Kernel/Pf/vjPfApp.h>    // the performer application base type
#include <Kernel/vjDebug.h>
#include <Kernel/vjProjection.h>  // for setNearFar (for setting clipping planes)
#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>

#include "fileIO.h"

/*
#include "SoundFactory.h"
#include "pjSoundNode.h" //performer-juggler sound node.
#include "pjSoundReplaceTrav.h"

SoundEngine* gSoundEngine = NULL;
*/

// nav includes
#include <pfNavDCS.h>
#include <velocityNav.h>
#include <collider.h>
#include <planeCollider.h>
#include <pfPogoCollider.h>
#include <pfRayCollider.h>
#include <pfBoxCollider.h>
#include <vjPfAppStats.h>

#include "pfFileIO.h" // handy fileloading/caching functions

// Declare my application class
class simplePfNavApp : public vjPfApp
{
public:
   simplePfNavApp( vjKernel* kern ) : mStatusMessageEmitCount(0), vjPfApp( kern )
   {
      mSun = NULL;
      mRootNode = NULL;
      mWorldModel = NULL;
      mWorldDCS = NULL;
      mCollidableModelGroup = NULL;
      enableStats();
   }

   virtual void init()
   {
      vjDEBUG(vjDBG_ALL, 1) << "app::init\n" << vjDEBUG_FLUSH;
      vjProjection::setNearFar( 0.4f, 200000 );

      mStats.setToggleButton("VJButton5");
   }

   virtual void apiInit()
   {
      vjDEBUG(vjDBG_ALL,1) << "simplePfNavApp::apiInit\n" << vjDEBUG_FLUSH;
   }

   virtual void preForkInit()
   {
      // Initialize type system
      vjDEBUG(vjDBG_ALL,1) << "simplePfNavApp::preForkInit: Initializing new types.\n" << vjDEBUG_FLUSH;

      // Initialize loaders
      if(!mModelFileName.empty())
         pfdInitConverter(mModelFileName.c_str());

      pfNavDCS::init();
      if(mUseStats)
         mStats.preForkInit();
   }

   /// Initialize the scene graph
   virtual void initScene();

   // load the model into the scene graph
   // If another model is already in the scene graph, we destroy it and load the newly configured one.
   // The model loaded is based on the configuration information that we currently have
   // This may be called multiple times
   virtual void initializeModel();

   /// Return the current scene graph
   virtual pfGroup* getScene()
   {
      vjDEBUG(vjDBG_ALL, 0) << "simplePfNavApp::getScene\n" << vjDEBUG_FLUSH;
      return mRootNode;
   }

   //: Function called in application process for each active channel each frame
   // Called immediately before draw (pfFrame())
   virtual void appChanFunc(pfChannel* chan)
   {
      if(mUseStats)
         mStats.appChanFunc(chan);
   }

   // Function called by the DEFAULT drawChan function before clearing the channel
   // and drawing the next frame (pfFrame())
   virtual void preDrawChan(pfChannel* chan, void* chandata)
   {
      //pfDisable( PFEN_TEXTURE );
      //pfOverride(PFSTATE_ENTEXTURE, PF_OFF);
      if(mUseStats)
         mStats.preDrawChan(chan,chandata);
   }

   /// Function called after pfSync and before pfDraw
   virtual void preFrame()
   {
      if (0 == (mStatusMessageEmitCount++ % 60))
      {
         vjVec3 cur_pos;
         cur_pos = mVelNavDrive->getCurPos().getTrans();
         cout << "Cur pos:" << cur_pos << endl;
      }

      // Update stats stuff
      if(mUseStats)
         mStats.preFrame();
   }

   /// Function called after pfDraw
   virtual void intraFrame()
   {;}

public:  // Configure the application
   // These must be set before the kernel starts calling the application
   void setFileName(const std::string filename) { mModelFileName = filename; }
   void setFilePath(const std::string path)     { mFilePath = path;}
   void setWorldDcsScale(const float scale)     {mWorldDcsScale = scale;}
   void setWorldDcsTrans(const vjVec3 trans)       {mWorldDcsTrans = trans;}
   void setInitialNavPos(const vjVec3 initialPos)  { mInitialNavPos = initialPos; }

   void enableStats() { mUseStats = true;}
   void disableStats() {mUseStats = false;}

public:
   // CONFIG PARAMS
   std::string    mModelFileName;
   std::string    mFilePath;
   float          mWorldDcsScale;
   vjVec3         mWorldDcsTrans;
   vjVec3         mInitialNavPos;

   int mStatusMessageEmitCount;

   bool           mUseStats;
   vjPfAppStats   mStats;

   // navigation objects.
   velocityNav*   mVelNavDrive;
   pfNavDCS*      mNavigationDCS;

   // SCENE GRAPH NODES
   pfLightSource* mSun;                      // Sun to light the environment
   pfGroup*       mRootNode;                 // The root of the scene graph
   pfNode*        mWorldModel;               // The model of the world
   pfDCS*         mWorldDCS;                 // DCS to transform the world
   pfGroup*       mCollidableModelGroup;     // Part of the world that is collidable
};

// ------- SCENE GRAPH ----
//
//                           /-- sun1
// rootNode -- mNavigationDCS -- mWorldDCS -- collidable_modelGroup -- world_model
//
void simplePfNavApp::initializeModel()
{
   if(NULL == mRootNode)      // If we haven't had initScene called yet
   {
      return;
   }

   // ----------- DESTROY OLD -------- //
   // REMOVE old Model
   if(mWorldModel != NULL)    // If we already have one world model
   {
      mCollidableModelGroup->removeChild(mWorldModel);
   }

   // Destory any old data
   // XXX: Delete old stuff

   // ------------- LOAD NEW --------- //
   // add some common file paths.
   pfFileIO::setFilePath( ".:./data:/usr/share/Performer/data:/usr/share/Performer/data/town:");
   pfFileIO::addFilePath( mFilePath );

   // LOAD file
   mWorldModel = pfFileIO::autoloadFile( mModelFileName, pfFileIO::FEET );

   // --- CONSTRUCT SCENE GRAPH --- //
   pfVec3 pf_worldDcsTrans;
   pf_worldDcsTrans = vjGetPfVec( mWorldDcsTrans );

   mWorldDCS->setScale( mWorldDcsScale );
   mWorldDCS->setTrans( pf_worldDcsTrans[0], pf_worldDcsTrans[1], pf_worldDcsTrans[2]);

   mCollidableModelGroup->addChild(mWorldModel);
}

void simplePfNavApp::initScene()
{
   // Load the scene
   vjDEBUG(vjDBG_ALL, 0) << "pfNavJugglerApplication::initScene\n" << vjDEBUG_FLUSH;

   // Allocate all the nodes needed
   mRootNode             = new pfGroup;       // Root of our graph
   mNavigationDCS        = new pfNavDCS;      // DCS to navigate with
   mCollidableModelGroup = new pfGroup;
   mWorldDCS             = new pfDCS;

   // Create the SUN
   mSun = new pfLightSource;
   mSun->setPos( 0.3f, 0.0f, 0.3f, 0.0f );
   mSun->setColor( PFLT_DIFFUSE,0.3f,0.0f,0.95f );
   mSun->setColor( PFLT_AMBIENT,0.4f,0.4f,0.4f );
   mSun->setColor( PFLT_SPECULAR, 1.0f, 1.0f, 1.0f );
   mSun->on();

   // --- Load the model (if it is configured) --- //
   initializeModel();

   // --- CONSTRUCT STATIC Structure of SCENE GRAPH -- //
   mRootNode->addChild( mNavigationDCS );
   mNavigationDCS->addChild(mWorldDCS);
   mWorldDCS->addChild(mCollidableModelGroup);

   mRootNode->addChild(mSun);

   // Configure the Navigator DCS node:
   vjMatrix initial_nav;              // Initial navigation position
   initial_nav.setTrans(mInitialNavPos);

   mVelNavDrive = new velocityNav;
   mVelNavDrive->setHomePosition(initial_nav);
   mVelNavDrive->setCurPos(initial_nav);
   mVelNavDrive->setMode(velocityNav::DRIVE);

   // --- COLLISION DETECTORS --- //
   // Terrain collider
   //planeCollider* collide = new planeCollider;
   pfPogoCollider*  ride_collide = new pfPogoCollider(mCollidableModelGroup);
   mVelNavDrive->addCollider(ride_collide);

   // Set the navigator's collider.
   pfBoxCollider* correction_collide = new pfBoxCollider( mCollidableModelGroup );
   mVelNavDrive->addCollider( correction_collide );

   mNavigationDCS->setNavigator(mVelNavDrive);

   // load these files into perfly to see just what your scenegraph
   // looked like. . . . .useful for debugging.
   cout << "Saving entire scene into lastscene.pfb, COULD TAKE A WHILE!\n" << flush;
   pfuTravPrintNodes( mRootNode, "lastscene.out" );
   pfdStoreFile( mRootNode, "lastscene.pfb" );
}

#endif

