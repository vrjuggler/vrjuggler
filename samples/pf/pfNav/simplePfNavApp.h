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

#ifndef _SIMPLE_PF_NAV_APP_H_
#define _SIMPLE_PF_NAV_APP_H_

#include <iostream>
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

#include <Utils/vjFileIO.h>

#include <Sound/vjSoundManager.h>
#include <Sound/pf/pfSoundNode.h> //performer-juggler sound node.
#include <Sound/pf/pfSoundTraverser.h>

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

#include <Performer/pf/pfTraverser.h>

int AppNotifyPreTrav(pfTraverser* trav, void* data);
int AppNotifyPostTrav(pfTraverser* trav, void* data);

// Declare my application class
class simplePfNavApp : public vjPfApp
{
public: // Model and sound member classes
   class Model
   {
   public:
      Model() : description( "no description" ),
         filename( "no name" ),
         scale( 1.0f ),
         pos( 0.0f, 0.0f, 0.0f ),
         rot( 0.0f, 0.0f, 0.0f ),
         isCollidable( true ),
         modelDCS(NULL),
         modelNode(NULL)
      {
      }

      Model(std::string desc, std::string file_name, float s, vjVec3 position, vjVec3 rotation, bool collidable)
      {
         description = desc;
         filename = file_name;
         scale = s;
         pos = position;
         rot = rotation;
         isCollidable = collidable;
      }

      // Config parameters
      std::string description;
      std::string filename;
      float       scale;
      vjVec3      pos;
      vjVec3      rot;
      bool        isCollidable;

      // Run-time information
      // --- modelDCS -- modelNode --- (Model info)
      pfDCS*      modelDCS;
      pfNode*     modelNode;
   };

   class Sound
   {
    public:
      Sound() : name( "no name" ),
          alias( "no alias set" ),
          positional( "false" ),
          pos( 0.0f, 0.0f, 0.0f )
      {
      }

      Sound(std::string sound_name, std::string alias_name, bool isPositional, vjVec3 position)
      {
         name = sound_name;
         alias = alias_name;
         positional = isPositional;
         pos = position;
      }

      std::string name;
      std::string alias;
      bool positional;
      vjVec3 pos;
   };

public:
   simplePfNavApp() : mStatusMessageEmitCount(0),// mWorldDcsTrans( 0.0f, 0.0f, 0.0f ),
      mInitialNavPos( 0.0f, 0.0f, 0.0f ),
      mBoundingSize(0.0f),
      mCurNavIndex(0),
      mLightGroup( NULL ),
      mConfiguredNoCollideModels( NULL ),
      mSoundNodes( NULL ),
      mUnCollidableModelGroup( NULL )
   {
      mSun = NULL;
      mRootNode = NULL;
      mConfiguredCollideModels = NULL;
      //mWorldDCS = NULL;
      mCollidableModelGroup = NULL;

      mNavigationDCS = NULL;

      //enableStats();
   }

   // Empty virtual destructor to make GCC happy.
   virtual ~simplePfNavApp (void) {
      /* Do nothing. */ ;
   }

   virtual void init()
   {
      //vjDEBUG(vjDBG_ALL, 1) << "simplePfNavApp::init\n" << vjDEBUG_FLUSH;
      /* vjProjection::setNearFar( 0.4f, 200000 ); XXXX: */

      mStats.setToggleButton("VJButton5");
      mNavCycleButton.init(std::string("VJButton3"));
   }

   virtual void apiInit()
   {
      //vjDEBUG(vjDBG_ALL,1) << "simplePfNavApp::apiInit\n" << vjDEBUG_FLUSH;
   }

   virtual void preForkInit()
   {
      // Initialize type system
      vjDEBUG(vjDBG_ALL,1) << "simplePfNavApp::preForkInit: Initializing new types.\n" << vjDEBUG_FLUSH;

      // Initialize loaders
      for (unsigned int x = 0; x < mModelList.size(); ++x)
      {
         std::cout << "simplePfNavAPP: Initializing performer file loaders for types like: "
                   << mModelList[x].filename.c_str() << "\n" << std::flush;

         if (!mModelList[x].filename.empty())
            pfdInitConverter( mModelList[x].filename.c_str() );
      }

      pfNavDCS::init();
      if(mUseStats && haveFocus())
         mStats.preForkInit();

      pfSoundTraverser::preForkInit();
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
      if(mUseStats && haveFocus())
         mStats.appChanFunc(chan);
   }

   // Function called by the DEFAULT drawChan function before clearing the channel
   // and drawing the next frame (pfFrame())
   virtual void preDrawChan(pfChannel* chan, void* chandata)
   {
      //pfDisable( PFEN_TEXTURE );
      //pfOverride(PFSTATE_ENTEXTURE, PF_OFF);
      if(mUseStats && haveFocus())
         mStats.preDrawChan(chan,chandata);
   }

   /// Function called after pfSync and before pfDraw
   virtual void preFrame()
   {
      // Deal with focus based changes
      mNavigationDCS->setActive(haveFocus());

      if(haveFocus() && (mNavCycleButton->getData() == vjDigital::TOGGLE_ON))
      {cycleNavigator();}

      if(haveFocus())
      {
         // Emit cur position
         if (0 == (mStatusMessageEmitCount++ % 60))
         {
            vjVec3 cur_pos;
            cur_pos = mNavigationDCS->getNavigator()->getCurPos().getTrans();
            std::cout << "Cur pos:" << cur_pos << std::endl;
         }
      }

      if(mUseStats && haveFocus())
         mStats.preFrame();
   }

   /// Function called after pfDraw
   virtual void intraFrame()
   {
      //vjDEBUG(vjDBG_ALL,0) << "-------- simplePfNavApp::intraFrame -------\n" << vjDEBUG_FLUSH;
   }

   //: Reset the application to initial state
   virtual void reset()
   {
      mNavigationDCS->reset();        // Reset navigation
   }

   //: Called when the focus state changes
   // If an application has focus:
   // - The user may be attempting to interact with it, so the app should process input
   // If not,
   // - The user is not interating with it, so ignore all input
   // - BUT, the user may still be viewing it, so render and update any animations, etc.
   //
   // This is akin to the way a user can only interact with a GUI window that has focus
   // (ie.The mouse is over the window)
   //
   // NOTE: This could be called before init() or even before the kernel has recieved the application
   /*
   virtual void focusChanged()
   {
      vjDEBUG(vjDBG_ALL,0) << clrOutNORM(clrCYAN,"simplePfNavApp::focusChanged") << "Focus now: " << haveFocus() << std::endl << vjDEBUG_FLUSH;

      if(mNavigationDCS != NULL)
      {
         if(haveFocus())
         { mNavigationDCS->setActive(true); }
         else
         { mNavigationDCS->setActive(false); }
      }
      else
         vjDEBUG(vjDBG_ALL,0) << clrOutNORM(clrCYAN,"   focusChanged:NavDCS == NULL") << std::endl << vjDEBUG_FLUSH;
   }
   */

public:  // Configure the application
   // These must be set before the kernel starts calling the application
   void addModelFile( const std::string filename )
   {
      vjDEBUG(vjDBG_ALL,0) << "Adding model file: " << filename.c_str() <<"\n"
                           << vjDEBUG_FLUSH;
      Model m;
      m.filename = filename;
      m.description = filename;
      addModel(m);
   }

   // Add a model to the model list
   void addModel(Model m)
   { mModelList.push_back(m); }

   void addSound(Sound s)
   { mSoundList.push_back(s); }

   void addFilePath(const std::string path)
   {
      mFilePath += path;
   }
   void setFilePath( const std::string path )
   {
      mFilePath = path;
   }
   void setInitialNavPos(const vjVec3 initialPos)
   {
      mInitialNavPos = initialPos;

      // if the navigator is already created then initScene has been called,
      // so we need to set the home pos in the nav, not just the member var.
      // FIXME: some code duplication here.
      for (unsigned int i=0;i<mNavigators.size();i++)
      {
	      vjMatrix initial_nav;
         vjDEBUG(vjDBG_ALL,0) << "setting pos\n" << vjDEBUG_FLUSH;
         initial_nav.setTrans( mInitialNavPos );
         
         mNavigators[i]->setHomePosition(initial_nav);
         mNavigators[i]->setCurPos(initial_nav);
      }
   }

   // Go to the next navigator
   void cycleNavigator()
   {
      unsigned new_nav_index = mCurNavIndex + 1;
      if(new_nav_index >= mNavigators.size())
         new_nav_index = 0;
      setNavigator(new_nav_index);
   }

   void setNavigator(unsigned new_index)
   {
      vjASSERT(index < mNavigators.size());

      if(new_index != mCurNavIndex)
      {
            // Copy cur position to new navigator
         mNavigators[new_index]->setCurPos(mNavigators[mCurNavIndex]->getCurPos());

         // Switch em
         mCurNavIndex = new_index;
         mNavigationDCS->setNavigator(mNavigators[mCurNavIndex]);
         vjDEBUG(vjDBG_ALL,0) << "simplePfNavApp: Navigation switched to: "
                              << clrSetNORM(clrGREEN)
                              << mNavigators[mCurNavIndex]->getName().c_str()
                              << clrRESET << std::endl << vjDEBUG_FLUSH;
      }
   }

   void enableStats() { mUseStats = true;}
   void disableStats() {mUseStats = false;}

// models and sounds.
public:
   std::vector< Model > mModelList;
   std::vector< Sound > mSoundList;

public:
   // CONFIG PARAMS
   std::string    mFilePath;
   vjVec3         mInitialNavPos;
   float          mBoundingSize;       // XXX: This is a hack and should be refactored

   int            mStatusMessageEmitCount;

   bool           mUseStats;
   vjPfAppStats   mStats;

   // navigation objects.
   std::vector<navigator*>    mNavigators;      // A list of the navigators in the system
   unsigned                   mCurNavIndex;     // Index of the current navigator
   pfNavDCS*                  mNavigationDCS;
   vjDigitalInterface         mNavCycleButton;  // Button to cycle the navigation

   // SCENE GRAPH NODES
   pfGroup*       mLightGroup;
   pfGroup*       mNoNav;
   pfLightSource* mSun;                      // Sun to light the environment
   pfGroup*       mRootNode;                 // The root of the scene graph
   pfGroup*       mConfiguredCollideModels;               // Collidable model of the world
   pfGroup*       mConfiguredNoCollideModels;                 // UnCollidable model of the world
   pfGroup*       mSoundNodes;               // The sounds of the world
   pfGroup*       mCollidableModelGroup;     // Part of the world that is collidable
   pfGroup*       mUnCollidableModelGroup;   // Part of the world that is collidable
};

// ------- SCENE GRAPH ----
// a standard organized interface for derived applications:
//
//                            /-- mLightGroup -- mSun
// mRootNode -- mNavigationDCS -- mCollidableModelGroup -- mConfiguredCollideModels -- loaded stuff...
//          \-- mNoNav        \-- mUnCollidableModelGroup -- mConfiguredNoCollideModels -- loaded stuff...
//                                                       \-- mSoundNodes -- loaded stuff...
void simplePfNavApp::initializeModels()
{
   if(NULL == mRootNode)      // If we haven't had initScene called yet
   {
      return;
   }

   // set some common paths first...
   pfFileIO::setFilePath( ".:./data:/usr/share/Performer/data:/usr/share/Performer/data/town:");
   // set the file paths...
   pfFileIO::addFilePath( mFilePath );
   vjDEBUG( vjDBG_ALL, 0 ) << clrOutNORM(clrCYAN,"setFilePath: ")
                           << mFilePath.c_str() << "\n" << vjDEBUG_FLUSH;


   // ----------- DESTROY OLD -------- //
   // REMOVE old Models
   if(mConfiguredCollideModels != NULL)
   {
      mCollidableModelGroup->removeChild(mConfiguredCollideModels);
   }
   if (mConfiguredNoCollideModels != NULL)
   {
      mUnCollidableModelGroup->removeChild(mConfiguredNoCollideModels);
   }

   // FIXME: Destory any old data
   // XXX: Delete old stuff

   mConfiguredCollideModels = new pfGroup;
   mConfiguredNoCollideModels = new pfGroup;

   for (unsigned int x = 0; x < mModelList.size(); ++x)
   {
      mModelList[x].modelDCS = new pfDCS();

      // set trans
      pfVec3 pf_nextModelDcsTrans;
      pfVec3 pf_model_trans = vjGetPfVec( mModelList[x].pos );
      mModelList[x].modelDCS->setTrans( pf_model_trans[0],
                                          pf_model_trans[1],
                                          pf_model_trans[2]);

      // FIXME: do rotation...
      mModelList[x].modelDCS->setScale( mModelList[x].scale );

      vjDEBUG(vjDBG_ALL,0) << "pfNavApp: Adding "
                           << mModelList[x].filename.c_str()
                           << "\n" << vjDEBUG_FLUSH;
      mModelList[x].modelNode = pfFileIO::autoloadFile( mModelList[x].filename, pfFileIO::NOCONVERT );

      assert( mModelList[x].modelNode != NULL );

      mModelList[x].modelDCS->addChild( mModelList[x].modelNode );      // Add model to the DCS
      if (mModelList[x].isCollidable == true)
         mConfiguredCollideModels->addChild( mModelList[x].modelDCS );
      else
         mConfiguredNoCollideModels->addChild( mModelList[x].modelDCS );
   }

   // --- CONSTRUCT SCENE GRAPH --- //
   mCollidableModelGroup->addChild( mConfiguredCollideModels );
   mUnCollidableModelGroup->addChild( mConfiguredNoCollideModels );

   // replace all nodes with _Sound_ with pfSoundNodes...
   std::string extension = "_Sound_";
   pfSoundTraverser::replace( mConfiguredCollideModels, extension );
   pfSoundTraverser::replace( mConfiguredNoCollideModels, extension );
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

   for (unsigned int x = 0; x < mSoundList.size(); ++x)
   {
      pfDCS* nextSoundDCS = new pfDCS();

      // set trans
      pfVec3 pf_nextSoundDcsTrans;
      pf_nextSoundDcsTrans = vjGetPfVec( mSoundList[x].pos );
      nextSoundDCS->setTrans( pf_nextSoundDcsTrans[0],
            pf_nextSoundDcsTrans[1],
            pf_nextSoundDcsTrans[2]);

      vjSound* vjs = vjSoundManager::instance()->getHandle( mSoundList[x].alias.c_str() );
      pfSoundNode* nextSound = new pfSoundNode( vjs, mSoundList[x].positional );
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
   mRootNode->setName("simplePfNavApp::mRootNode");
   mNavigationDCS        = new pfNavDCS;      // DCS to navigate with
   mCollidableModelGroup = new pfGroup;
   mUnCollidableModelGroup = new pfGroup;
   mNoNav                  = new pfGroup;
   //mWorldDCS             = new pfDCS;

   // Add callback for testing to the root node
   //mRootNode->setTravFuncs(PFTRAV_DRAW, AppNotifyPreTrav, AppNotifyPostTrav);

   mRootNode->addChild( mNoNav );
   /*  SHOWS The feet and the head...
   pfNode* no = pfdLoadFile( "ball.flt" ) ;  // ball is at 0.0.0
   pfDCS* noo = new pfDCS;
   mRootNode->addChild( no );
   mRootNode->addChild( noo );
   noo->setTrans( 0.0f, 0.0f, 6.0f );
   noo->addChild( no );
   */

   // Create the SUN
   mLightGroup = new pfGroup;
   mSun = new pfLightSource;
   mLightGroup->addChild( mSun );
   mSun->setPos( 0.3f, 0.0f, 0.3f, 0.0f );
   mSun->setColor( PFLT_DIFFUSE,1.0f,1.0f,1.0f );
   mSun->setColor( PFLT_AMBIENT,0.3f,0.3f,0.3f );
   mSun->setColor( PFLT_SPECULAR, 1.0f, 1.0f, 1.0f );
   mSun->on();



   // --- CONSTRUCT STATIC Structure of SCENE GRAPH -- //
   mRootNode->addChild( mNavigationDCS );
   mNavigationDCS->addChild(mLightGroup);
   mNavigationDCS->addChild(mCollidableModelGroup);
   mNavigationDCS->addChild(mUnCollidableModelGroup);
   //mNavigationDCS->addChild( pfFileIO::autoloadFile( "terrain.flt", pfFileIO::FEET ) );

   // --- Load the model (if it is configured) --- //
   initializeModels();
   initializeSounds();

   // Configure the Navigator DCS node:
   vjMatrix initial_nav;              // Initial navigation position
   initial_nav.setTrans( mInitialNavPos );


   // --- CREATE VELOCITY navigator --- //
   velocityNav* vel_nav_drive = new velocityNav;
   vel_nav_drive->setHomePosition(initial_nav);
   vel_nav_drive->setCurPos(initial_nav);
   vel_nav_drive->setMode(velocityNav::DRIVE);
   vel_nav_drive->setName("VelocityNav: Drive mode");

   // --- CREATE FLY navigator --- //
   velocityNav* vel_nav_fly = new velocityNav;
   vel_nav_fly->setHomePosition(initial_nav);
   vel_nav_fly->setCurPos(initial_nav);
   vel_nav_fly->setMode(velocityNav::FLY);
   vel_nav_fly->setName("VelocityNav: Fly mode");

   // --- COLLISION DETECTORS --- //
   // Terrain collider
   //planeCollider* collide = new planeCollider;
   pfPogoCollider*  ride_collide = new pfPogoCollider(mCollidableModelGroup);
   vel_nav_drive->addCollider(ride_collide);

   // Set the navigator's collider.
   pfBoxCollider* correction_collide = new pfBoxCollider( mCollidableModelGroup );
   vel_nav_drive->addCollider( correction_collide );
   vel_nav_fly->addCollider( correction_collide );

   // -- ADD NAVERS to list --- //
   mNavigators.push_back(vel_nav_drive);
   mNavigators.push_back(vel_nav_fly);

   // Set initial navigator to use
   mCurNavIndex = 0;
   mNavigationDCS->setNavigator(mNavigators[mCurNavIndex]);

   // make sure config is processed, before doing sound replace traversal.
   this->configProcessPending();

   // replace all nodes with _Sound_ with pfSoundNodes...
   //std::string extension = "_Sound_";
   //pfSoundTraverser::replace( mRootNode, extension );

   // load these files into perfly to see just what your scenegraph
   // looked like. . . . .useful for debugging.
   //std::cout<<"[pfNav] Saving entire scene into lastscene.pfb, COULD TAKE A WHILE!\n"<<std::flush;
   /*
   pfuTravPrintNodes( mRootNode, "lastscene.out" );
   pfdStoreFile( mRootNode, "lastscene.pfb" );
   */
}

int AppNotifyPreTrav(pfTraverser* trav, void* data)
{
   vjDEBUG(vjDBG_ALL,0) << "Traversing app (pre): "
                        << "chan: " << (void*)trav->getChan()
                        << " node:" << (void*)trav->getNode() << ": "
                        << trav->getNode()->getName() << std::endl
                        << vjDEBUG_FLUSH;
   return PFTRAV_CONT;
}

int AppNotifyPostTrav(pfTraverser* trav, void* data)
{
   vjDEBUG(vjDBG_ALL,0) << "Traversing app (post): "
                        << "chan: " << (void*)trav->getChan()
                        << " node:" << (void*)trav->getNode() << ": "
                        << trav->getNode()->getName() << std::endl
                        << vjDEBUG_FLUSH;
   return PFTRAV_CONT;
}

#endif

