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

#include "SoundFactory.h"
#include "fileIO.h"
#include "pjSoundNode.h" //performer-juggler sound node.
#include "pjSoundReplaceTrav.h"

SoundEngine* gSoundEngine = NULL;


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
      // get a valid time on the stopwatch...
   }

   virtual void init()
   {
      vjDEBUG(vjDBG_ALL, 1) << "app::init\n" << vjDEBUG_FLUSH;
      vjProjection::setNearFar( 0.4f, 200000 );

      mStats.setToggleButton("VJButton5");
   }

   virtual void apiInit()
   {
      vjDEBUG(vjDBG_ALL,1) << "pfNavJugglerApplication::apiInit\n" << vjDEBUG_FLUSH;
   }

   virtual void preForkInit()
   {
      // Initialize type system
      vjDEBUG(vjDBG_ALL,1) << "pfNavJugglerApplication::preForkInit: Initializing new types.\n" << vjDEBUG_FLUSH;

      // Initialize loaders
      //pfdInitConverter("terrain.flt");

      pfNavDCS::init();
      mStats.preForkInit();
   }

   /// Initialize the scene graph
   virtual void initScene();

   /// Return the current scene graph
   virtual pfGroup* getScene()
   {
      vjDEBUG(vjDBG_ALL, 0) << "pfNavJugglerApplication::getScene\n" << vjDEBUG_FLUSH;
      return rootNode;
   }

   //: Function called in application process for each active channel each frame
   // Called immediately before draw (pfFrame())
   virtual void appChanFunc(pfChannel* chan)
   {
      mStats.appChanFunc(chan);
   }

   // Function called by the DEFAULT drawChan function before clearing the channel
   // and drawing the next frame (pfFrame())
   virtual void preDrawChan(pfChannel* chan, void* chandata)
   {
      //pfDisable( PFEN_TEXTURE );
      //pfOverride(PFSTATE_ENTEXTURE, PF_OFF);
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

public:
   // CONFIG PARAMS
   std::string    mModelFileName;
   std::string    mFilePath;
   float          mWorldDcsScale;
   vjVec3         mWorldDcsTrans;
   vjVec3         mInitialNavPos;


   int mStatusMessageEmitCount;

   // navigation objects.
   velocityNav*   mVelNavDrive;
   pfNavDCS*      mNavigationDCS;

   // scene's root (as far as we're concerned here)
   pfGroup*       rootNode;

   vjPfAppStats   mStats;
};

void simplePfNavApp::initScene()
{
   // Load the scene
   vjDEBUG(vjDBG_ALL, 0) << "pfNavJugglerApplication::initScene\n" << vjDEBUG_FLUSH;

   // Allocate all the nodes needed
   rootNode                = new pfGroup;          // Root of our graph
   mNavigationDCS          = new pfNavDCS;      // DCS to navigate with
   pfNode* world_model;
   pfDCS*   world_model_dcs = new pfDCS;
   pfGroup* collidable_modelGroup = new pfGroup;

   // Create the SUN
   pfLightSource* sun1 = new pfLightSource;
   sun1->setPos( 0.3f, 0.0f, 0.3f, 0.0f );
   sun1->setColor( PFLT_DIFFUSE,0.3f,0.0f,0.95f );
   sun1->setColor( PFLT_AMBIENT,0.4f,0.4f,0.4f );
   sun1->setColor( PFLT_SPECULAR, 1.0f, 1.0f, 1.0f );
   sun1->on();

   /// Load SIMPLE geometry
   // add some common file paths.
   pfFileIO::addFilePath( ".:./data:/usr/share/Performer/data:/usr/share/Performer/data/town");
   pfFileIO::addFilePath( mFilePath );

   // LOAD file
   world_model = pfFileIO::autoloadFile( mModelFileName, pfFileIO::FEET );

   // --- CONSTRUCT SCENE GRAPH --- //
   //                           /-- sun1
   // rootNode -- mNavigationDCS -- collidable_modelGroup -- world_model
   //                           \-- noncollidable_modelGroup -- other_models
   pfVec3 pf_worldDcsTrans;
   pf_worldDcsTrans = vjGetPfVec( mWorldDcsTrans );
   
   rootNode->addChild( mNavigationDCS );
   mNavigationDCS->setScale( mWorldDcsScale );
   mNavigationDCS->setTrans( pf_worldDcsTrans[0], pf_worldDcsTrans[1], pf_worldDcsTrans[2]);
   mNavigationDCS->addChild( sun1 );
   mNavigationDCS->addChild( collidable_modelGroup );
   collidable_modelGroup->addChild(world_model);

   // Configure the Navigator DCS node:
   vjMatrix initial_nav;              // Initial navigation position
   initial_nav.setTrans(mInitialNavPos);

   mVelNavDrive = new velocityNav;
   mVelNavDrive->setHomePosition(initial_nav);
   mVelNavDrive->setCurPos(initial_nav);
   mVelNavDrive->setMode(velocityNav::DRIVE);

      //mVelNavDrive->setMode(velocityNav::FLY);

   // --- COLLISION DETECTORS --- //
   // Terrain collider
   //planeCollider* collide = new planeCollider;
   pfPogoCollider*  ride_collide = new pfPogoCollider(collidable_modelGroup);
   mVelNavDrive->addCollider(ride_collide);

   // Set the navigator's collider.
   pfBoxCollider* correction_collide = new pfBoxCollider( collidable_modelGroup );
   mVelNavDrive->addCollider( correction_collide );

   mNavigationDCS->setNavigator(mVelNavDrive);

   // replace all nodes with _Sound_ with pjSoundNodes...
   //pjSoundReplaceTrav::traverse( collidable_modelGroup, gSoundEngine, "_Sound_" );
   ///assert( NULL);

   // load these files into perfly to see just what your scenegraph
   // looked like. . . . .useful for debugging.
   cout<<"Saving entire scene into lastscene.pfb, COULD TAKE A WHILE!\n"<<flush;
   pfuTravPrintNodes( rootNode, "lastscene.out" );
   pfdStoreFile( rootNode, "lastscene.pfb" );
}

#endif

