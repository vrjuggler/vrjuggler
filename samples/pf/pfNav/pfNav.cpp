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


// nav includes
#include <CaveNavigator.h>
#include <collider.h>
#include <planeCollider.h>
#include <pfPogoCollider.h>
#include <pfRayCollider.h>
#include <pfBoxCollider.h>

char* filename = NULL;

#include "pfFileIO.h" // handy fileloading/caching functions

// Declare my application class
class pfNavJugglerApplication : public vjPfApp
{
public:
   pfNavJugglerApplication( vjKernel* kern ) : mFpsEmitCount(0), vjPfApp( kern )
   {
      // get a valid time on the stopwatch...
      stopWatch.stop();
      stopWatch.start();
   }

   virtual void init()
   {
      vjDEBUG(vjDBG_ALL, 1) << "app::init\n" << vjDEBUG_FLUSH;
      vjProjection::setNearFar(0.5, 1000);

      mWand.init( "VJWand" );
      mHead.init( "VJHead" );
      mActionButton.init( "VJButton0" );
      mActionButton2.init( "VJButton1" );
      mModeChangeButton.init( "VJButton2" );
   }

   virtual void apiInit()
   {
      vjDEBUG(vjDBG_ALL,1) << "app::apiInit\n" << vjDEBUG_FLUSH;
   }

   virtual void preForkInit()
   {
      // Initialize type system
      vjDEBUG(vjDBG_ALL,1) << "app::preForkInit: Initializing new types.\n" << vjDEBUG_FLUSH;

      // Initialize loaders
      pfdInitConverter("terrain.flt");
   }

   inline void textures( bool state ) const
   {
      if (state == true)
      {
         pfEnable( PFEN_TEXTURE );
         pfOverride(PFSTATE_ENTEXTURE, PF_ON);
      }

      else
      {
         pfDisable( PFEN_TEXTURE );
         pfOverride(PFSTATE_ENTEXTURE, PF_OFF);
      }
   }

   /// Initialize the scene graph
   virtual void initScene()
   {
      // Load the scene
      vjDEBUG(vjDBG_ALL, 0) << "app::initScene\n" << vjDEBUG_FLUSH;

      // Allocate all the nodes needed
      rootNode                = new pfGroup;          // Root of our graph
      mNavigationDCS          = new pfDCS;      // DCS to navigate with
      pfNode* world_model;
      pfDCS*  world_model_dcs = new pfDCS;

      // CONFIG PARAMS
      std::string    pf_file_path("");
      const float    world_dcs_scale(1.0f);
      const pfVec3   world_dcs_trans(0.0,5.0,0.0f);
      vjVec3         initial_nav_pos(0,6,0);
      bool           use_gravity(true);

      // Create the SUN
      sun1 = new pfLightSource;
      sun1->setPos(0.3f, 0.0f, 0.3f, 0.0f);
      sun1->setColor(PFLT_DIFFUSE,0.3f,0.0f,0.95f);
      sun1->setColor(PFLT_AMBIENT,0.4f,0.4f,0.4f);
      sun1->setColor(PFLT_SPECULAR, 1.0f, 1.0f, 1.0f);
      sun1->on();

      /// Load SIMPLE geometry
      pfFileIO::addFilePath(".:/usr/share/Performer/data:/usr/share/Performer/data/town");
      pfFileIO::addFilePath(pf_file_path);

      // LOAD file
      world_model = pfFileIO::autoloadFile( filename );

      // Construct scene graph
      //                           /-- sun1
      // rootNode -- mNavigationDCS -- world_model_dcs -- world_model
      //
      rootNode->addChild( mNavigationDCS );
      world_model_dcs->addChild(world_model);
      world_model_dcs->setScale(world_dcs_scale);
      world_model_dcs->setTrans(world_dcs_trans[0], world_dcs_trans[1], world_dcs_trans[2]);
      mNavigationDCS->addChild(sun1);
      mNavigationDCS->addChild(world_model_dcs);

      // Configure the Navigator DCS node:
      vjMatrix initial_nav;              // Initial navigation position
      initial_nav.setTrans(initial_nav_pos);
      mNavigator.getNavigator()->setCurPos(initial_nav);
      if(use_gravity)
         mNavigator.setGravity(CaveNavigator::ON);
      else
         mNavigator.setGravity(CaveNavigator::OFF);

      // --- COLLISION DETECTORS --- //
      // Terrain collider
      //planeCollider* collide = new planeCollider;
      pfPogoCollider*  ride_collide = new pfPogoCollider(world_model_dcs);
      mNavigator.getNavigator()->addCollider(ride_collide);

      // Set the navigator's collider.
      pfBoxCollider* correction_collide = new pfBoxCollider( world_model_dcs );
      mNavigator.getNavigator()->addCollider( correction_collide );

      // load these files into perfly to see just what your scenegraph
      // looked like. . . . .useful for debugging.
      cout<<"Saving entire scene into lastscene.pfb, COULD TAKE A WHILE!\n"<<flush;
      pfuTravPrintNodes( rootNode, "lastscene.out" );
      pfdStoreFile( rootNode, "lastscene.pfb" );
   }

   /// Return the current scene graph
   virtual pfGroup* getScene()
   {
      vjDEBUG(vjDBG_ALL, 0) << "app::getScene\n" << vjDEBUG_FLUSH;
      return rootNode;
   }

    // Function called by the DEFAULT drawChan function before clearing the channel
   // and drawing the next frame (pfFrame())
   virtual void preDrawChan(pfChannel* chan, void* chandata)
   {
      this->textures( true );     // Override texturing to turn it on;
   }


   /// Function called before pfSync
   virtual void preSync()
   {
      //vjDEBUG(vjDBG_ALL, 1) << "app::preSync\n" << vjDEBUG_FLUSH;
   }

   // Update the navigation based on user input
   virtual void updateNavigation()
   {
      if (mActionButton2->getData() == vjDigital::TOGGLE_ON)
         cout<<"Brake\n"<<flush;
      if (mActionButton->getData() == vjDigital::TOGGLE_ON)
         cout<<"Accelerate\n"<<flush;

      // let the navigator collect some instructions from input devices...
      mNavigator.accelerate( mActionButton->getData() == vjDigital::ON);
      mNavigator.brake( mActionButton2->getData() == vjDigital::ON);
      mNavigator.rotate( mActionButton2->getData() == vjDigital::OFF );

      // Update the navigators rotation information
      vjMatrix rotMatrix = *(mWand->getData());;
      rotMatrix.setTrans(0, 0, 0);
      mNavigator.setMatrix( rotMatrix );

      // tell the navigator to update itself with any new instructions just given to it.
      mNavigator.update();

      // Output current position in environment
      vjVec3 cur_pos = mNavigator.getTrans();
      cout << "Cur pos:" << cur_pos << endl;
   }

   /// Function called after pfSync and before pfDraw
   virtual void preFrame()
   {
      // Keep time, for FPS measurments...
      stopWatch.stop();
      stopWatch.start();

      // Do any NAVIGATION
      updateNavigation();

      // Set the navigation DCS to the new navigation matrix
      pfMatrix mNavigator_pf = vjGetPfMatrix( mNavigator );
      mNavigationDCS->setMat( mNavigator_pf );

      // output the FPS so the team artist can get metrics on their model
      ++mFpsEmitCount;
      if (mFpsEmitCount >= 15)
      {
         cout<<"FPS: "<<stopWatch.fpsAverage<<"\n"<<flush;
         mFpsEmitCount = 0;
      }
   }

   int mFpsEmitCount;
   StopWatch stopWatch;

   /// Function called after pfDraw
   virtual void intraFrame()
   {
      //vjDEBUG(vjDBG_ALL,1) << "app::intraFrame\n" << vjDEBUG_FLUSH;
   }

public:
   // the sun
   pfLightSource* sun1;

   // navigation objects.
   CaveNavigator  mNavigator;
   pfDCS*         mNavigationDCS;

   // juggler device interface objects
   vjPosInterface          mWand;      // the Wand
   vjPosInterface          mHead;      // the Head
   vjDigitalInterface      mActionButton;
   vjDigitalInterface      mActionButton2;
   vjDigitalInterface      mModeChangeButton;

   // scene's root (as far as we're concerned here)
   pfGroup*   rootNode;
};


float transSpeed = 0.1;


int main(int argc, char* argv[])
{
    vjKernel* kernel = vjKernel::instance(); // Declare a new Kernel
    pfNavJugglerApplication* application = new pfNavJugglerApplication(kernel);  // Delcare an instance of my application

    cout<<"Usage: "<<argv[0]<<" modelfile vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n"<<flush;
    cout<<"\n"<<flush;

    if (argc < 2)
    {
       cout<<"\n\n[ERROR!!!] you must supply a model database (then config files)\n\n"<<flush;
       return 0;
    }

    filename = argv[1];

    if (argc <= 2)
    {
       cout<<"\n\n[ERROR!!!] you must supply config files after the model file...\n\n"<<flush;
       return 0;
    }

    for ( int i = 2; i < argc; i++ ) {
        kernel->loadConfigFile(argv[i]);
    }

    kernel->start();

    kernel->setApplication(application);    // Set up the kernel

    //while(!kernel->done())
    while(1)
     {;}
}


