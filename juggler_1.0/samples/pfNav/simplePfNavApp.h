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

#include <Sound/vjSoundManager.h>
#include <Sound/pf/pjSoundNode.h> //performer-juggler sound node.
#include <Sound/pf/pjSoundReplaceTrav.h>

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
   simplePfNavApp() : mStatusMessageEmitCount(0),// mWorldDcsTrans( 0.0f, 0.0f, 0.0f ),
      mInitialNavPos( 0.0f, 0.0f, 0.0f ),
      mUseDriveMode( true ),
      mUCmodels( NULL ), 
      mSoundNodes( NULL ),
      mUnCollidableModelGroup( NULL )
   {
      mSun = NULL;
      mRootNode = NULL;
      mWorldModel = NULL;
      //mWorldDCS = NULL;
      mCollidableModelGroup = NULL;
      mUseDriveMode = true;

      mVelNavDrive = NULL;
      mNavigationDCS = NULL;

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
      for (int x = 0; x < mModelList.size(); ++x)
      {
         cout<<"initing converter for types like: "<<mModelList[x].filename<<"\n"<<flush;
      
         if (!mModelList[x].filename.empty())
            pfdInitConverter( mModelList[x].filename.c_str() );
      }
            
      pfNavDCS::init();
      if(mUseStats)
         mStats.preForkInit();
      
      pjSoundReplaceTrav::preForkInit();
   }

   /// Initialize the scene graph
   virtual void initScene();

   // load the model into the scene graph
   // If another model is already in the scene graph, we destroy it and load the newly configured one.
   // The model loaded is based on the configuration information that we currently have
   // This may be called multiple times
   virtual void initializeModels();
   virtual void initializeSounds();

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

      if(mUseStats)
         mStats.preFrame();
   }

   /// Function called after pfDraw
   virtual void intraFrame()
   {;}

public:  // Configure the application
   // These must be set before the kernel starts calling the application
   void addModel( const std::string filename ) 
   { 
      cout<<"Adding model: "<<filename<<"\n"<<flush;
      model m;
      m.filename = filename;
      m.description = filename;
      mModelList.push_back( m );
   }
   void setFilePath(const std::string path)     { mFilePath = path;}
   //void setWorldDcsScale(const float scale)     {mWorldDcsScale = scale;}
   //void setWorldDcsTrans(const vjVec3 trans)       {mWorldDcsTrans = trans;}
   void setInitialNavPos(const vjVec3 initialPos)  { mInitialNavPos = initialPos; }
   void setUseDriveMode(const bool val=true)  { mUseDriveMode = val;}

   void enableStats() { mUseStats = true;}
   void disableStats() {mUseStats = false;}

// models and sounds.
public:
   class model
   {
    public:
      model() : description( "no description" ),
         filename( "no name" ),
         scale( 1.0f ),
         pos( 0.0f, 0.0f, 0.0f ),
         rot( 0.0f, 0.0f, 0.0f ),
         isCollidable( true )
      {
      }    
      std::string description;
      std::string filename;
      float scale;
      vjVec3 pos;
      vjVec3 rot;
      bool isCollidable;
   };
   std::vector< model > mModelList;
   class sound
   {
    public:
      sound() : name( "no name" ),
          alias( "no alias set" ),
          positional( "false" ),
          pos( 0.0f, 0.0f, 0.0f )
      {
      }    
      std::string name;
      std::string alias;
      bool positional;
      vjVec3 pos;
   };
   std::vector< sound > mSoundList;

public:
   // CONFIG PARAMS
   //std::string    mModelFileName;
   std::string    mFilePath;
   //float          mWorldDcsScale;
   //vjVec3         mWorldDcsTrans;
   vjVec3         mInitialNavPos;
   bool           mUseDriveMode;

   int mStatusMessageEmitCount;

   bool           mUseStats;
   vjPfAppStats   mStats;

   // navigation objects.
   velocityNav*   mVelNavDrive;
   pfNavDCS*      mNavigationDCS;

   // SCENE GRAPH NODES
   pfLightSource* mSun;                      // Sun to light the environment
   pfGroup*       mRootNode;                 // The root of the scene graph
   pfGroup*        mWorldModel;               // The model of the world
   pfGroup*        mUCmodels;               // The model of the world
   pfGroup*        mSoundNodes;               // The sounds of the world
   //pfDCS*         mWorldDCS;                 // DCS to transform the world
   pfGroup*       mCollidableModelGroup;     // Part of the world that is collidable
   pfGroup*       mUnCollidableModelGroup;     // Part of the world that is collidable
};

// ------- SCENE GRAPH ----
//
//                           /-- sun1
// rootNode -- mNavigationDCS -- mCollidableModelGroup -- mWorldModel -- loaded stuff...
//                           \-- mUnCollidableModelGroup - mUCmodels
//                                                      \-- mSoundNodes
void simplePfNavApp::initializeModels()
{
   if(NULL == mRootNode)      // If we haven't had initScene called yet
   {
      return;
   }

   // ----------- DESTROY OLD -------- //
   // REMOVE old Models
   if(mWorldModel != NULL)
   {
      mCollidableModelGroup->removeChild(mWorldModel);  
   }
   if (mUCmodels != NULL)
   {
      mUnCollidableModelGroup->removeChild(mUCmodels);
   }   

   // FIXME: Destory any old data
   // XXX: Delete old stuff

   // ------------- LOAD NEW --------- //
   // add some common file paths.
   pfFileIO::setFilePath( ".:./data:/usr/share/Performer/data:/usr/share/Performer/data/town:");
   pfFileIO::addFilePath( mFilePath );

   mWorldModel = new pfGroup;
   mUCmodels = new pfGroup;
   
   for (int x = 0; x < mModelList.size(); ++x)
   {
      pfDCS* nextModelDCS = new pfDCS();

      // set trans
      pfVec3 pf_nextModelDcsTrans;
      pf_nextModelDcsTrans = vjGetPfVec( mModelList[x].pos );
      nextModelDCS->setTrans( pf_nextModelDcsTrans[0], 
            pf_nextModelDcsTrans[1], 
            pf_nextModelDcsTrans[2]);
      
      // FIXME: do rotation...
      
      nextModelDCS->setScale( mModelList[x].scale );
      cout<<"Adding "<<mModelList[x].filename<<"\n"<<flush;
      pfNode* nextModel = pfFileIO::autoloadFile( mModelList[x].filename, pfFileIO::NOCONVERT );
      
      assert( nextModel != NULL );
      
      nextModelDCS->addChild( nextModel );
      if (mModelList[x].isCollidable == true)
         mWorldModel->addChild( nextModelDCS );
      else
         mUCmodels->addChild( nextModelDCS );
   }   
   
   // --- CONSTRUCT SCENE GRAPH --- //
   mCollidableModelGroup->addChild( mWorldModel );
   mUnCollidableModelGroup->addChild( mUCmodels );
}

// func needs to destroy all previous pf nodes associated with sound
// and then reading mSoundList, create them all again.
void simplePfNavApp::initializeSounds()
{
   if(NULL == mRootNode)      // If we haven't had initScene called yet
   {
      return;
   }

   // ----------- DESTROY OLD -------- //
   // REMOVE old sounds
   if(mSoundNodes != NULL)
   {
      mUnCollidableModelGroup->removeChild( mSoundNodes );  
   }
   
   mSoundNodes = new pfGroup();
   
   for (int x = 0; x < mSoundList.size(); ++x)
   {
      pfDCS* nextSoundDCS = new pfDCS();

      // set trans
      pfVec3 pf_nextSoundDcsTrans;
      pf_nextSoundDcsTrans = vjGetPfVec( mSoundList[x].pos );
      nextSoundDCS->setTrans( pf_nextSoundDcsTrans[0], 
            pf_nextSoundDcsTrans[1], 
            pf_nextSoundDcsTrans[2]);
      
      vjSound* vjs = vjSoundManager::instance()->getHandle( mSoundList[x].alias.c_str() );
      pjSoundNode* nextSound = new pjSoundNode( vjs, mSoundList[x].positional );
      vjs->trigger();  //make sure it's playing..
      
      nextSoundDCS->addChild( nextSound );
      mSoundNodes->addChild( nextSoundDCS );
   }
   
   mUnCollidableModelGroup->addChild( mSoundNodes );
}

void simplePfNavApp::initScene()
{
   // Load the scene
   vjDEBUG(vjDBG_ALL, 0) << "pfNavJugglerApplication::initScene\n" << vjDEBUG_FLUSH;

   // Allocate all the nodes needed
   mRootNode             = new pfGroup;       // Root of our graph
   mNavigationDCS        = new pfNavDCS;      // DCS to navigate with
   mCollidableModelGroup = new pfGroup;
   mUnCollidableModelGroup = new pfGroup;
   //mWorldDCS             = new pfDCS;

   // Create the SUN
   mSun = new pfLightSource;
   mSun->setPos( 0.3f, 0.0f, 0.3f, 0.0f );
   mSun->setColor( PFLT_DIFFUSE,0.3f,0.0f,0.95f );
   mSun->setColor( PFLT_AMBIENT,0.4f,0.4f,0.4f );
   mSun->setColor( PFLT_SPECULAR, 1.0f, 1.0f, 1.0f );
   mSun->on();

   

   // --- CONSTRUCT STATIC Structure of SCENE GRAPH -- //
   mRootNode->addChild( mNavigationDCS );
   mNavigationDCS->addChild(mSun);
   mNavigationDCS->addChild(mCollidableModelGroup);
   mNavigationDCS->addChild(mUnCollidableModelGroup);
   //mNavigationDCS->addChild( pfFileIO::autoloadFile( "terrain.flt", pfFileIO::FEET ) );
   
   // --- Load the model (if it is configured) --- //
   initializeModels();
   initializeSounds();
      
   // Configure the Navigator DCS node:
   vjMatrix initial_nav;              // Initial navigation position
   initial_nav.setTrans(mInitialNavPos);

   mVelNavDrive = new velocityNav;
   mVelNavDrive->setHomePosition(initial_nav);
   mVelNavDrive->setCurPos(initial_nav);
   if(mUseDriveMode)
      mVelNavDrive->setMode(velocityNav::DRIVE);
   else
      mVelNavDrive->setMode(velocityNav::FLY);

   // --- COLLISION DETECTORS --- //
   // Terrain collider
   //planeCollider* collide = new planeCollider;
   pfPogoCollider*  ride_collide = new pfPogoCollider(mCollidableModelGroup);
   mVelNavDrive->addCollider(ride_collide);

   // Set the navigator's collider.
   pfBoxCollider* correction_collide = new pfBoxCollider( mCollidableModelGroup );
   mVelNavDrive->addCollider( correction_collide );

   mNavigationDCS->setNavigator(mVelNavDrive);

   // make sure config is processed, before doing sound replace traversal.
   this->configProcessPending();
   
   // replace all nodes with _Sound_ with pjSoundNodes...
   std::string extension = "_Sound_";
   pjSoundReplaceTrav::traverse( mRootNode, extension );
   

   // load these files into perfly to see just what your scenegraph
   // looked like. . . . .useful for debugging.
   //cout<<"[pfNav] Saving entire scene into lastscene.pfb, COULD TAKE A WHILE!\n"<<flush;
   pfuTravPrintNodes( mRootNode, "lastscene.out" );
   pfdStoreFile( mRootNode, "lastscene.pfb" );
}

#endif

